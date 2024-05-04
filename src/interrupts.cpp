#define INTERRUPTS_INNER
#include <interrupts.h>

#include <memory.h>
#include <serial.h>

namespace nov
{
namespace interrupts
{

// TODO: align for performance
static ProtectedIDTEntry protected_idt[256];
static ProtectedIDTDescriptor protected_idtr;

extern "C" void interruptReintegrator(uint8_t interrupt)
{
    com_1 << "interrupt " << stream::Mode::HEX << interrupt << " was just called" << stream::endl;
    if (interrupt == 0x09)
    {
        com_1 << "apparently the keyboard has something to say!" << stream::endl;
        com_1 << stream::Mode::HEX << inb(0x60) << stream::endl;
        outb(0x20, 0x20); // this tells the PIC that we listened to what it had to say
    }
    com_1.flush();
}

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
    protected_idtr.base = (uint32_t)(&protected_idt);
    protected_idtr.limit = (uint16_t)((sizeof(ProtectedIDTEntry) * 256) - 1);
    com_1 << "IDTR assigned" << stream::endl;
    com_1 << "interrupt handler size: " << interruptHandlerSize << stream::endl;

    for (int i = 0; i < 32; i++)
        configureInternalInterruptVector(i, (void*)((uint32_t)(&interruptHandlerASM) + (interruptHandlerSize * i)), GateType::INTERRUPT_32, Privilege::LEVEL_0);
    com_1 << "internal interrupt vectors assigned, handler is located at " << (uint32_t)(&interruptHandlerASM) << stream::endl;
    asm ("lidt %0" : : "m"(protected_idtr));
    com_1 << "IDT loaded" << stream::endl;
    // mask ALL IRQs from the PIC. TODO: reenable and configure these later
    outb(0x21,0xff);
    outb(0xa1,0xff);
    enableInterrupts();
    com_1 << "interrupts enabled" << stream::endl;
}

}
}