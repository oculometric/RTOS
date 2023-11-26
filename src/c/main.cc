#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

extern "C"
void kernel_main(void* mbs, unsigned int magic)
{
    uint16_t* terminal_buffer = (uint16_t*) 0xB8000;

	if (magic != 0x2BADB002)
	{
		terminal_buffer[0] = (uint16_t)'X' | ((uint16_t) 0 << 8) | ((uint16_t) 4 << 12);
		return;
	}




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
		if (c > 'z') c = 'a';
	}
}