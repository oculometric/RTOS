#pragma once

#include <stdint.h>

#define cli() asm volatile ("cli")
#define hlt() asm volatile ("hlt")

inline uint32_t get_eip()
{
    uint32_t eip;
    asm volatile ("mov [%%esp], %0" : "=a" ((uint32_t)eip));
    return eip;
}