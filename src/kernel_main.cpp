#include "boot.h"
#include "serial.h"
#include "itos.h"


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

    //dword_hex_to_string((unsigned int)os_hint_table_address, pointer);
    //dword_hex_to_string(os_hint_table_address->checksum, pointer);

    init_serial(COM1);
    serial_println((char)0xcc, COM1);
    serial_println('K', COM1);
    serial_println((char*)greeting, COM1);
    serial_print_hex((uint32_t)os_hint_table_address, COM1);

    serial_println_dec(0, COM1);
    serial_println_hex(0, COM1);
    serial_println_bin(0, COM1);

    char test[16] = {'\0'};
    hex_to_string(0xffccdd, test, 10);
    serial_println(test, 12, COM1);


    return;
}