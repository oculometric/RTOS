#pragma once

#include <stdint.h>

struct global_descriptor_entry
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t access;
    uint8_t attributes;
    uint8_t base_high;
};

#define GDT_SIZE sizeof(global_descriptor_entry)

enum gdt_privilege
{
    RING_0 = 0b00000000,
    RING_1 = 0b00100000,
    RING_2 = 0b01000000,
    RING_3 = 0b01100000
};

enum gdt_access_type
{
    SYSTEM                      = 0b00000001,
    CODE_CONFORMING             = 0b00011011,
    CODE_CONFORMING_HIDDEN      = 0b00011001,
    CODE_NONCONFORMING          = 0b00011111,
    CODE_NONCONFORMING_HIDDEN   = 0b00011101,
    DATA_GROWUP                 = 0b00010011,
    DATA_GROWUP_READONLY        = 0b00010001,
    DATA_GROWDOWN               = 0b00010111,
    DATA_GROWDOWN_READONLY      = 0b00010101
};

enum gdt_flags
{
    PROTECTED_16_BYTE = 0b00000000,
    PROTECTED_32_BYTE = 0b01000000,
    PROTECTED_16_PAGE = 0b10000000,
    PROTECTED_32_PAGE = 0b11000000,
    LONG_PAGE         = 0b10100000
};

void populate_gdt_entry(global_descriptor_entry&, uint32_t, uint32_t, gdt_privilege, gdt_access_type, gdt_flags);
void init_flat_gdt(uint8_t*, uint8_t*);