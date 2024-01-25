#include "boot.h"

const char * hex_chars = "0123456789abcdef";

void dword_hex_to_string(unsigned int v, char* buffer)
{
    unsigned int buffer_offset = 0;
    buffer[0] = '0';
    buffer[2] = 'x';
    buffer_offset += 2;

    unsigned int digits = 0;
    unsigned int tmp = v;
    while (tmp > 0) { digits++; tmp /= 16; }

    if (digits == 0) { buffer[4] = '0'; return; }

    buffer_offset += digits - 1;

    tmp = v;
    while (tmp > 0)
    {
        unsigned int remainder = tmp % 16;
        buffer[buffer_offset*2] = hex_chars[remainder];
        buffer_offset--;
        tmp /= 16;
    }
    return;
}

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

    dword_hex_to_string((unsigned int)os_hint_table_address, pointer);
    dword_hex_to_string(os_hint_table_address->checksum, pointer);
    return;
}