#pragma once

#include <stdint.h>

namespace nov
{
namespace interrupts
{

struct InterruptFrame;

enum GateType
{
    TASK = 0x05,
    INTERRUPT_16 = 0x06,
    TRAP_16 = 0x07,
    INTERRUPT_32 = 0x0E,
    TRAP_32 = 0x0F
};

enum Privilege
{
    LEVEL_0 = 0,
    LEVEL_1 = 1,
    LEVEL_2 = 2,
    LEVEL_3 = 3
};

void configureIRQs(uint8_t interrupt_base);
void configureIDT();
void configureInterruptVector(uint8_t interrupt, void(* handler)(InterruptFrame* frame), GateType gate, Privilege priv);

inline void acknowledgePICInterrupt()
{
    asm(
        "mov al,20h \n\t\
        out 20h,al  \n\t\
        "
        );
}

inline void enableInterrupts() { asm("sti"); }
inline void disableInterrupts() { asm("cli"); }

#ifdef INTERRUPTS_INNER

#pragma pack(1)
struct ProtectedIDTEntry
{
    uint16_t isr_low_word;
    uint16_t segment_selector;
    uint8_t reserved_for_zero;
    uint8_t attributes;
    uint16_t isr_high_word;
};

struct ProtectedIDTDescriptor
{
    uint16_t limit;
    uint32_t base;
};

struct InterruptFrame
{
    uint32_t ip;
    uint32_t cs;
    uint32_t flags;
    uint32_t sp;
    uint32_t ss;
};
#pragma pack(0)


#endif

}
}

#ifdef INTERRUPTS_INNER

extern void* interruptHandlerASM; // FIXME: this is not how you define functions
extern uint32_t interruptHandlerSize;

#endif