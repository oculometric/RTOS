#pragma once

#include <stdint.h>

#define cli() asm volatile ("cli")
#define hlt() asm volatile ("hlt")

inline uint32_t getEIP()
{
    uint32_t eip;
    asm volatile ("mov [%%esp], %0" : "=a" ((uint32_t)eip));
    return eip;
}

/**
 * enables interrupts
 * 
 * **/
inline void enableInterrupts() { asm("sti"); }

/**
 * disables interrupts
 * **/
inline void disableInterrupts() { asm("cli"); }