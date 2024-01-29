#pragma once

#include <stdint.h>

#include <io.h>

#define COM1 0x3f8
#define COM2 0x2f8

#define SERIAL_DATA_REG 0
#define SERIAL_INTERRUPT_REG 1
#define SERIAL_INTERRUPTID_REG 2
#define SERIAL_LINECONTROL_REG 3
#define SERIAL_MODEMCONTROL_REG 4
#define SERIAL_LINESTATUS_REG 5
#define SERIAL_MODEMSTATUS_REG 6
#define SERIAL_SCRATCH_REG 7

#define SERIAL_INIT_SUCCESS 0
#define SERIAL_INIT_FAILURE 1

uint8_t init_serial(uint32_t port);

static inline bool transmit_ready(uint32_t port) { return inb(port+SERIAL_LINESTATUS_REG) & 0b00100000; }
static inline bool receive_ready(uint32_t port) { return inb(port+SERIAL_LINESTATUS_REG) & 0b00000001; }

void serial_print(char chr, uint32_t port);
void serial_print(char* str, uint32_t port);
void serial_print(char* str, uint32_t length, uint32_t port);
void serial_print_dec(uint32_t val, uint32_t port);
void serial_print_hex(uint32_t val, uint32_t port);
void serial_print_bin(uint32_t val, uint32_t port);

void serial_println(uint32_t port);

void serial_println(char chr, uint32_t port);
void serial_println(char* str, uint32_t port);
void serial_println(char* str, uint32_t length, uint32_t port);
void serial_println_dec(uint32_t val, uint32_t port);
void serial_println_hex(uint32_t val, uint32_t port);
void serial_println_bin(uint32_t val, uint32_t port);

void serial_dump_byte(void* start, uint32_t length, uint32_t port, uint8_t per_line = 0, char separator = '\0');

void serial_dump_hex_byte(void* start, uint32_t length, uint32_t port, uint8_t per_line = 8);
void serial_dump_hex_word(void* start, uint32_t length, uint32_t port, uint8_t per_line = 8);
void serial_dump_hex_dwrd(void* start, uint32_t length, uint32_t port, uint8_t per_line = 8);