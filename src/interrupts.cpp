#define INTERRUPTS_INNER
#include <interrupts.h>
#undef INTERRUPTS_INNER

#include <memory.h>
#include <serial.h>

namespace nov
{
namespace interrupts
{

__attribute__((__aligned__(0x10)))
// interrupt descriptor table for protected mode
static ProtectedIDTEntry protected_idt[256];
__attribute__((__aligned__(0x10)))
// IDT descriptor for protected mode
static ProtectedIDTDescriptor protected_idtr;

// offset of the IRQ vectors in the IDT
static uint8_t irq_interrupt_offset = 0;

// array containing function pointers which handle the first 32 interrupts
static void(* low_interrupt_handlers[32])();

// array containing function pointers which handle IRQs
static void(* irq_interrupt_handlers[16])();

// array containing function pointers which handle interrupts > 32 and not reserved for IRQs
static void(* high_interrupt_handlers[256-32])();

/**
 * called from assembly code when an interrupt arrives. the number of the 
 * interrupt is passed as an argument. this function handles passing that
 * interrupt on as a function call to a relevant handler function.
 * 
 * if the interrupt is one of the 32 reserved by the CPU, it will be passed
 * to the relevant function in the `low_interrupt_handlers` array.
 * 
 * if the interrupt is within the range registered to IRQs from the PIC
 * (configurable using the `configureIRQS` function), the relevant function
 * from the `irq_interrupt_handlers` function will be called (corresponding to
 * the IRQ number, not the interrupt number!)
 * 
 * otherwise, the relevant function from the `high_interrupt_handlers` array
 * will be called (imagine this as the remaining block of available interrupt
 * vectors with a chunk taken out for the IRQ vectors, and a chunk taken out
 * from the reserved vectors)
 * 
 * @param interrupt the interrupt number currently being serviced
 * 
 * **/
extern "C" void interruptReintegrator(uint8_t interrupt)
{
    if (interrupt < 32)
    {
        // if this is a reserved interrupt, fire it from the low vector table
        com_1 << "low interrupt " << stream::Mode::HEX << interrupt << " was just called" << stream::endl;
        low_interrupt_handlers[interrupt]();
    }
    else if (interrupt >= irq_interrupt_offset && interrupt < irq_interrupt_offset + 16)
    {
        // if this is an IRQ, fire it from the IRQ vector table
        com_1 << "IRQ interrupt " << stream::Mode::HEX << interrupt - irq_interrupt_offset << " was just called" << stream::endl;
        (irq_interrupt_handlers[interrupt-irq_interrupt_offset])();
        acknowledgePICInterrupt(interrupt-irq_interrupt_offset);
    }
    else
    {
        // this is just any old random interrupt, so fire it from the high vector table
        uint8_t index = (interrupt < irq_interrupt_offset) ? (interrupt - 32) : (interrupt - (irq_interrupt_offset + 16));
        com_1 << "high interrupt " << stream::Mode::HEX << index << " was just called (" << interrupt << ")" << stream::endl;
        (high_interrupt_handlers[index])();
    }
    
    com_1.flush();
}

void placeholderCPUInterruptHandler()
{
    com_1 << "a cpu interrupt was not handled." << stream::endl;
}

void placeholderIRQHandler()
{
    com_1 << "an irq was not handled." << stream::endl;
}

void placeholderMiscInterruptHandler()
{
    com_1 << "some other interrupt was not handled." << stream::endl;
}

/**
 * performs the real configuration of the actual IDT. this should only be used to set
 * the ISR to point to an assembly routine. in fact, it should only be used inside the
 * `configureIDT` function to perform the initial passthrough initialisation with the
 * micro-ISRs.
 * 
 * @param interrupt index of the interrupt to hook
 * @param handler pointer to bare assembly function which the IDT entry will point to
 * @param gate gate type for the IDT entry
 * @param priv privilege level for the IDT entry
 * 
 * **/
void configureInternalInterruptVector(uint8_t interrupt, void(* handler), GateType gate, Privilege priv)
{
    if (handler == 0x0) { com_1 << "invalid interrupt handler address " << stream::endl; return; }

    protected_idt[interrupt].isr_low_word = (uint32_t)handler & 0xFFFF;
    protected_idt[interrupt].isr_high_word = (uint32_t)handler >> 16;
    protected_idt[interrupt].segment_selector = 0x08; // FIXME: write C-based GDT code and put the kernel offset in here, instead of this magic number
    protected_idt[interrupt].attributes = 0b10000000 | ((priv & 0b11) << 5) | (gate & 0b1111);
    protected_idt[interrupt].reserved_for_zero = 0;
}

void configureIDT()
{
    disableInterrupts();
    // set up IDT descriptor
    protected_idtr.base = (uint32_t)(&protected_idt);
    protected_idtr.limit = (uint16_t)((sizeof(ProtectedIDTEntry) * 64) - 1);
    com_1 << "IDTR assigned" << stream::endl;
    com_1 << "IDT base: " << stream::Mode::HEX << protected_idtr.base << stream::endl;
    com_1 << "IDT limit: " << stream::Mode::HEX << protected_idtr.limit << stream::endl;
    com_1 << "IDT entry size: " << stream::Mode::DEC << (uint32_t)sizeof(ProtectedIDTEntry) << stream::endl;

    void** microISRTablePointer = (void** )(&microISRTable);

    // configure interrupt vectors to point to incremental ISRs in the assembly file interrupt_handler.asm
    com_1 << "first microISR located at: " << stream::Mode::HEX << (uint32_t)microISRTablePointer[0] << stream::endl;
    for (int i = 0; i < 64; i++)
    {
        com_1 << "micro-ISR " << stream::Mode::DEC << i << stream::Mode::HEX << " located at " << (uint32_t)microISRTablePointer[i] << stream::endl;
        com_1 << "value at that address: " << *((uint32_t* )microISRTablePointer[i]) << stream::endl;
        configureInternalInterruptVector(i, microISRTablePointer[i], GateType::INTERRUPT_32, Privilege::LEVEL_0);
    }
    com_1 << "internal interrupt vectors assigned, handlers start at " << (uint32_t)(microISRTablePointer) << stream::endl;
    
    // populate vector tables
    // populate low vector table
    for (int i = 0; i < 32; i++)
        configureInterruptHandler(i, placeholderCPUInterruptHandler, GateType::TRAP_32, Privilege::LEVEL_0);
    configureInterruptHandler(0x02, placeholderCPUInterruptHandler, GateType::INTERRUPT_32, Privilege::LEVEL_0);
    // populate IRQ vector table
    for (int i = 0; i < 16; i++)
        configureIRQHandler(i, placeholderIRQHandler);
    // populate high vector table
    for (int i = 32; i < 256; i++)
        configureInterruptHandler(i, placeholderMiscInterruptHandler, GateType::INTERRUPT_32, Privilege::LEVEL_0);

    // load IDT
    asm ("lidt %0" : : "m"(protected_idtr));
    com_1 << "IDT loaded" << stream::endl;
}

void configureInterruptHandler(uint8_t interrupt, void (*handler)(), GateType gate, Privilege priv)
{
    // set the info of the IDT entry
    protected_idt[interrupt].attributes = 0b10000000 | ((priv & 0b11) << 5) | (gate & 0b1111);
    if (interrupt < 32)
    {
        // point the relevant low interrupt handler to the provided function
        low_interrupt_handlers[interrupt] = handler;
    }
    else
    {
        // if the specified interrupt is currently overlapping with an IRQ, assign it anyway,
        // since the IRQs may be remapped later, unmasking these interrupts
        if (interrupt >= irq_interrupt_offset && interrupt < irq_interrupt_offset + 16)
            com_1 << "this interrupt (" << interrupt << ") is currently in use by IRQ " << stream::Mode::HEX << interrupt - irq_interrupt_offset << " so it will never be called." << stream::endl;
        // point the relevant high interrupt handler to the provided function
        high_interrupt_handlers[interrupt - 32] = handler;
    }
}

void configureIRQs(uint8_t interrupt_base)
{
    // codeword 1 - start initialisation, telling both PICs to listen for additional codeword
    outb(PICRegister::PIC1_COMMAND, PICInitCodeWords1::INIT | PICInitCodeWords1::ICW4_PRESENT);
    outb(PICRegister::PIC2_COMMAND, PICInitCodeWords1::INIT | PICInitCodeWords1::ICW4_PRESENT);
    // codeword 2 - set interrupt vector offset of PICs
    outb(PICRegister::PIC1_DATA, interrupt_base);
    outb(PICRegister::PIC2_DATA, interrupt_base+8);
    irq_interrupt_offset = interrupt_base;
    // codeword 3 - report how primary and secondary are wired
    outb(PICRegister::PIC1_DATA, 0b00000010); // IRQs from secondary are on IRQ2 on the primary
    outb(PICRegister::PIC2_DATA, 2); // secondary identity in cascade
    // codeword 4 - set PICs in 8086 mode
    outb(PICRegister::PIC1_DATA, PICInitCodeWords4::MODE_8086);
    outb(PICRegister::PIC2_DATA, PICInitCodeWords4::MODE_8086);
    // fully set masks to disable all IRQs for now
    outb(PICRegister::PIC1_DATA, 0xff);
    outb(PICRegister::PIC2_DATA, 0xff);
}

// TODO: properly implement task switching
// TODO: reimplement this so it can alter micro-ISRs (or preferably generate its own on the fly) to match type properly

void setIRQEnabled(uint8_t irq, bool enabled)
{
    if (irq >= 16) return;
    // get the current IRQ enable mask
    uint8_t mask = inb(irq < 8 ? PICRegister::PIC1_DATA : PICRegister::PIC2_DATA);
    uint8_t new_mask = (1 << (irq % 8));
    // compute new, setting the bit to disable or clearing it to enable
    mask = enabled ? (mask & ~new_mask) : (mask | new_mask);
    // send back to the PIC
    outb(irq < 8 ? PICRegister::PIC1_DATA : PICRegister::PIC2_DATA, mask);
}

bool getIRQEnabled(uint8_t irq)
{
    if (irq >= 16) return false;
    return inb(irq < 8 ? PICRegister::PIC1_DATA : PICRegister::PIC2_DATA) & (1 << (irq % 8));
}

void configureIRQHandler(uint8_t irq, void(* handler)())
{
    if (irq >= 16) return;
    // make sure this is a 32-bit interrupt gate
    protected_idt[irq + irq_interrupt_offset].attributes = 0x8e;
    irq_interrupt_handlers[irq] = handler;
}

}
}