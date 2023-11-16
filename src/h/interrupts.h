#pragma once

#include <stdint.h>

struct interrupt_descriptor_entry
{
    uint16_t offset_low = 0;
    uint16_t segment_selector = 0;
    uint8_t blank = 0;
    uint8_t attributes = 0;
    uint16_t offset_high = 0;
};

enum idt_gate_type
{
    TASK = 0x05,            // this is scary
    INTERRUPT_16 = 0x06,    // means performing an ISR
    TRAP_16 = 0x07,         // should handle exceptions
    INTERRUPT_32 = 0x0E,
    TRAP_32 = 0x0F
};

enum idt_privilege
{
    RING_0 = 0b00000000,
    RING_1 = 0b00100000,
    RING_2 = 0b01000000,
    RING_3 = 0b01100000
};

void populate_idt_entry(interrupt_descriptor_entry& entry, uint32_t offset, uint16_t segment, idt_gate_type gate, idt_privilege dpl)
{
    entry.offset_low = offset & 0xFFFF;
    entry.offset_high = (offset >> 16) & 0xFFFF;
    entry.segment_selector = segment;
    entry.blank = 0;
    entry.attributes = 0b10000000 | dpl | gate;
}

