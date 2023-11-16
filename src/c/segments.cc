#include "segments.h"

void populate_gdt_entry(global_descriptor_entry& entry, uint32_t base, uint32_t limit, gdt_privilege dpl, gdt_access_type access, gdt_flags flags)
{
    entry.limit_low = limit & 0xFFFF;
    entry.base_low = base & 0xFFFF;
    entry.base_mid = (base >> 16) & 0xFF;
    entry.access = 0b10000000 | dpl | access;
    entry.attributes = flags | ((limit >> 16) & 0xF);
    entry.base_high = (base >> 24) & 0xFF;
}

void init_flat_gdt(uint8_t* gdt_address, uint8_t* TSS)
{
    global_descriptor_entry* gdt_base = (global_descriptor_entry*)gdt_address;
    global_descriptor_entry working_entry;

    // create null descriptor, first entry
    populate_gdt_entry(working_entry, 0x0, 0x0, (gdt_privilege)0x0, (gdt_access_type)0x0, (gdt_flags)0x0);
    gdt_base[0] = working_entry;

    // note: tutorial is using protected 32 bit pages rather than bytes. do they mean to do this?

    // create kernel code descriptor
    populate_gdt_entry(working_entry, 0x0, 0xFFFFF, gdt_privilege::RING_0, gdt_access_type::CODE_CONFORMING, gdt_flags::PROTECTED_32_BYTE);
    gdt_base[1] = working_entry;

    // create kernel data descriptor
    populate_gdt_entry(working_entry, 0x0, 0xFFFFF, gdt_privilege::RING_0, gdt_access_type::DATA_GROWUP, gdt_flags::PROTECTED_32_BYTE);
    gdt_base[2] = working_entry;

    // create task descriptor
    populate_gdt_entry(working_entry, (uint32_t)TSS, sizeof(*TSS), gdt_privilege::RING_0, (gdt_access_type)0x9, (gdt_flags)0x0);
    gdt_base[3] = working_entry;

    // load GDT into the segment registers TODO:HERE
    asm volatile
    (
        "gdtr:                      \n\t"
        "   .int 0                  \n\t"
        "   .long 0                 \n\t"
        "mov %%limit, (gdtr)        \n\t"
        "mov %%base, (gdtr + 2)     \n\t"
        "lgdt (gdtr)                \n\t"
    );
}