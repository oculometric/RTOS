#include <stdint.h>

void init_serial();

void serial_print(char);
void serial_print(char*);
void serial_print(char*, uint32_t);
void serial_print_dec(uint32_t);
void serial_print_hex(uint32_t);
void serial_print_bin(uint32_t);

void serial_println();

void serial_println(char);
void serial_println(char*);
void serial_println(char*, uint32_t);
void serial_println_dec(uint32_t);
void serial_println_hex(uint32_t);
void serial_println_bin(uint32_t);

void serial_dump_byte(void*, uint32_t, uint8_t);
void serial_dump_word(void*, uint32_t, uint8_t);
void serial_dump_dwrd(void*, uint32_t, uint8_t);