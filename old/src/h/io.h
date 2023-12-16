#pragma once

#include <stdint.h>

static inline uint8_t inb(uint32_t port)
{
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint32_t port, uint8_t value)
{
    asm volatile("outb %1, %0" : : "dN" (port), "a" (value));
}