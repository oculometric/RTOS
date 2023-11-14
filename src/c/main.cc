#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

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

    while (true);
}