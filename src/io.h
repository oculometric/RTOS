#pragma once

#include <stdint.h>

static inline void outb(uint32_t p, uint8_t b) { asm volatile ("outb %0, %1" : : "dN" (p), "a" (b)); }
static inline uint8_t inb(uint32_t p) { uint8_t r; asm volatile ("inb %0, %1" : "=a"(r) : "Nd"(p)); return r; }