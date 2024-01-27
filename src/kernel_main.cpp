#include "boot.h"
#include "serial.h"
#include "itos.h"
#include "vbe.h"


// TODO: serial
// TODO: keyboard
// TODO: pixel graphics

extern "C" void main(os_hint_table* os_hint_table_address)
{
    const char* greeting = "Hello, World!\0";


    *(char*)0xb8000 = 'R';
    char* pointer = (char*)0xb8000;
    int i = 0;
    while (greeting[i] != '\0')
    {
        pointer[i*2] = greeting[i];
        i++;
    }

    init_serial(COM1);
    serial_println((char*)"hello from kernel main.", COM1);

    serial_println((char*)"reading high memory map...", COM1);

    for (int m = 0; m < os_hint_table_address->memory_map_table_entries; m++)
    {
        serial_print((char*)"memory map entry ", COM1); serial_print_dec(m, COM1); serial_println((char*)":", COM1);

        uint64_t size = os_hint_table_address->memory_map_table_address[m].region_size;
        const char* size_scale = "B)\0\0";
        if (size > 4096) { size /= 1024; size_scale = "KiB)"; }
        if (size > 4096) { size /= 1024; size_scale = "MiB)"; }
        if (size > 4096) { size /= 1024; size_scale = "GiB)"; }

        serial_print((char*)"   base address low  : ", COM1); serial_println_hex(os_hint_table_address->memory_map_table_address[m].region_base & 0xFFFFFFFF, COM1);
        serial_print((char*)"   base address high : ", COM1); serial_println_hex(os_hint_table_address->memory_map_table_address[m].region_base >> 32, COM1);
        serial_print((char*)"   region size low   : ", COM1); serial_println_hex(os_hint_table_address->memory_map_table_address[m].region_size & 0xFFFFFFFF, COM1);
        serial_print((char*)"   region size high  : ", COM1); serial_print_hex(os_hint_table_address->memory_map_table_address[m].region_size >> 32, COM1);
        serial_print((char*)" (", COM1); serial_print_dec(size, COM1); serial_println((char*)size_scale, COM1);
        serial_print((char*)"   region type       : ", COM1); serial_println_dec(os_hint_table_address->memory_map_table_address[m].region_type, COM1);
        serial_print((char*)"   acpi data         : ", COM1); serial_println_bin(os_hint_table_address->memory_map_table_address[m].region_acpi, COM1);
    }

    serial_println(COM1);
    serial_println((char*)"starting search for PMID block", COM1);
    void* result = find_pmid_block((void*)0, 0xFFFFFFF0);
    serial_println_hex((uint32_t)result, COM1);
    serial_println(COM1);
    serial_dump_hex_byte(result, 20, COM1, 4);

    serial_println(COM1);

    serial_println_dec(os_hint_table_address->vbe_mode_info_block->x_resolution, COM1);
    serial_println_dec(os_hint_table_address->vbe_mode_info_block->y_resolution, COM1);
    serial_println_hex(os_hint_table_address->vbe_mode_info_block->flat_framebuffer_address, COM1);

    return;
}