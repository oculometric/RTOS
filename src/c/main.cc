#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "serial.h"

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
		//serial_output(COM1, (uint8_t*)text);

		i++;
		if (i >= 25*80)
		{
			i = 0;
			c++;
		}
		terminal_buffer[i] = (uint16_t)c | ((uint16_t) 0 << 8) | ((uint16_t) 4 << 12);
		if (c == 255) break;
	}
	serial_output(COM1, (uint8_t*)"now i am going to try and perform an interrupt.\n");

	uint16_t kb = 0;
	asm volatile ("clc");
	asm volatile ("int $0x12");
	asm volatile ("mov %0, %%ax" : "=a"(kb));

	serial_output(COM1, (kb >> 8) & 0b11111111);
	serial_output(COM1, kb & 0b11111111);
}