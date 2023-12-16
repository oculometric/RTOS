#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "serial.h"
#include "segments.h"

extern "C"
void kernel_main()
{

    uint16_t* terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < 25; y++) {
		for (size_t x = 0; x < 80; x++) {
			const size_t index = (y * 80) + x;
			terminal_buffer[index] = (uint16_t)'X' | ((uint16_t) 0 << 8) | ((uint16_t) 4 << 12);
		}
	}

	initialise_serial(COM1, 38400);
	serial_output(COM1, (uint8_t*)"hello! i am alive.\n");
	serial_output(COM1, (uint8_t*)"all work no play makes jack a dull boy\n");

	uint8_t c = 'a';
	int i = 0;
    while (true)
	{
		i++;
		if (i >= 25*80)
		{
			i = 0;
			c++;
		}
		terminal_buffer[i] = (uint16_t)c | ((uint16_t) 0 << 8) | ((uint16_t) 4 << 12);
		if (c == 255) break;
	}

	serial_output(COM1, (uint8_t*)"initialising GDT...\n");

	global_descriptor_entry gdt_address[4] = { NULL };
	init_flat_gdt((uint8_t *)gdt_address, NULL);

	serial_output(COM1, (uint8_t*)"GDT placed at ");
	serial_output_u32(COM1, (uint32_t)gdt_address);
	serial_output(COM1, (uint8_t*)" size ");
	serial_output_u16(COM1, (4*sizeof(global_descriptor_entry))-1);
	serial_output(COM1, '\n');

	serial_output(COM1, (uint8_t*)"checking current CS:IP...\n");

	serial_output(COM1, (uint8_t*)"now i am going to try and perform an interrupt.\n");

	uint16_t kb = 0;
	asm volatile ("clc");
	asm volatile ("int $0x12");
	asm volatile ("mov %0, %%ax" : "=r"(kb));

	serial_output(COM1, (uint8_t*)"woo yaay!");

	serial_output_u16(COM1, kb);
}