#pragma once

#include <stdint.h>
#include <io.h>

namespace nov
{
namespace interrupts
{

// type of gate which an interrupt represents
enum GateType
{
    TASK = 0x05,
    INTERRUPT_16 = 0x06,
    TRAP_16 = 0x07,
    INTERRUPT_32 = 0x0E,
    TRAP_32 = 0x0F
};

// privilege level an interrupt should execute at. lower is more privileged
enum Privilege
{
    LEVEL_0 = 0,
    LEVEL_1 = 1,
    LEVEL_2 = 2,
    LEVEL_3 = 3
};

/**
 * configures the interrupt descriptor table, and relevant cascading tables,
 * handlers, etc. enables interrupts when it's done. you should remap the PIC with
 * `configureIRQs` before calling this.
 * 
 * interrupts are handled by a set of micro-ISRs in assembly, which simply capture the 
 * interrupt number before passing it onto a routine which protects the stack and register
 * states, then calls into the interrupt reintegrator in C++, passing in the interrupt number
 * 
 * **/
void configureIDT();

/**
 * configures an interrupt to be assigned a particular handler. `configureIDT` **MUST** be called first
 * 
 * @param interrupt index of the interrupt to assign
 * @param handler pointer to a function which will handle the interrupt
 * @param gate type of gate this interrupt should behave as
 * @param priv privilege level the interrupt should execute as
 * 
 * **/
void configureInterruptHandler(uint8_t interrupt, void(* handler)(), GateType gate, Privilege priv);

/**
 * configures the PIC and remaps where interrupts are sent to start at `interrupt_base`.
 * this means that IRQ0 will arrive on interrupt vector `interrupt_base`, IRQ1 on `interrupt_base+1`, 
 * etc. also masks all IRQs so they can be enabled individually later
 * 
 * @param interrupt_base starting interrupt index to map IRQs to
 * 
 * **/
void configureIRQs(uint8_t interrupt_base);

/**
 * enables or disables an IRQ
 * 
 * @param irq index of the IRQ to toggle, must be less than 16
 * @param enabled whether the IRQ should be enabled or disabled
 * 
 * **/
void setIRQEnabled(uint8_t irq, bool enabled);

/**
 * gets the enabled status of an IRQ
 * 
 * @param irq index of the IRQ to check
 * @returns whether the interrupt is currently enabled or not
 * 
 * **/
bool getIRQEnabled(uint8_t irq);

/**
 * assigns a handler function to handle a numbered IRQ
 * @param irq index of the IRQ to hook
 * @param handler pointer to function which will be called when the IRQ fires
 * 
 * **/
void configureIRQHandler(uint8_t irq, void(* handler)());

/**
 * enables interrupts
 * 
 * **/
inline void enableInterrupts() { asm("sti"); }
/**
 * disables interrupts
 * **/
inline void disableInterrupts() { asm("cli"); }

#ifdef INTERRUPTS_INNER

#pragma pack(1)
// structure representing an entry in the IDT for protected mode
struct ProtectedIDTEntry
{
    uint16_t isr_low_word;
    uint16_t segment_selector;
    uint8_t reserved_for_zero;
    uint8_t attributes;
    uint16_t isr_high_word;
};

// structure representing the IDT descriptor
struct ProtectedIDTDescriptor
{
    uint16_t limit;
    uint32_t base;
};
#pragma pack(0)

// ports of PIC registers
enum PICRegister
{
    PIC1_COMMAND = 0x20,
    PIC1_DATA,
    PIC2_COMMAND = 0xa0,
    PIC2_DATA
};

// commands which can be sent to the PICs
enum PICCommand
{
    END_OF_INTERRUPT = 0x20
};

// codewords which are used to initialise the PICs
enum PICInitCodeWords1
{
    ICW4_PRESENT =  0x01,
    SINGLE_MODE =   0x02,   // aka cascade mode
    INTERVAL4 =     0x04,   // call address inverval 4 (8) // what the fuck does this mean?
    LEVEL_MODE =    0x08,	// aka edge triggered mode
    INIT =          0x10
};

// codewords which are used to give extra configuration information to the PICs
enum PICInitCodeWords4
{
    MODE_8086 =     0x01,   // 8086 mode
    EOI_AUTO =      0x02,
    BUFFERED_SLAVE =0x08,
    BUFFERED_MASTER=0x0c,
    NOT_FULLY_NEST =0x10    // disables special fully nested mode
};

/**
 * tell the PIC we heard what it had to say
 * 
 * @param irq index of the IRQ acknowledged, so we can tell the
 * secondary PIC as well if necessary
 * 
 * **/
inline void acknowledgePICInterrupt(uint8_t irq)
{
    if (irq >= 8)
        outb(PICRegister::PIC2_COMMAND, PICCommand::END_OF_INTERRUPT);
    outb(PICRegister::PIC1_COMMAND, PICCommand::END_OF_INTERRUPT);
}

#endif

}
}

#ifdef INTERRUPTS_INNER

/** represents the start of the micro-ISR table from the assmebly file. you need
 * to get a reference to this variable instead of just using it, for some reason
 * **/
extern uint8_t interruptHandlerASM;
// size of each micro-ISR
extern uint32_t interruptHandlerSize;

#endif