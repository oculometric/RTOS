#pragma once

#include <stdint.h>
#include <stddef.h>

#include "io.h"

#define CHARS_FOR_ITOS "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"

#define COM1 0x3F8
#define COM2 0x2F8

#define DATA 0
#define INTERRUPT_ENABLE 1
#define INTERRUPT_ID 2
#define LINE_CONTROL 3
#define MODEM_CONTROL 4
#define LINE_STATUS 5
#define MODEM_STATUS 6
#define SCRATCH 7

static inline uint8_t read_COM_register(uint32_t port, uint8_t reg)
{
    return inb(port+reg);
}

static inline void write_COM_register(uint32_t port, uint8_t reg, uint8_t value)
{
    outb(port+reg, value);
}

void serial_output_c(uint32_t, uint8_t);

uint8_t serial_input(uint32_t);

void serial_output(uint32_t, uint8_t*);

void serial_output_length(uint32_t, uint8_t*, size_t);

void serial_output_u8(uint32_t, uint8_t);
void serial_output_u16(uint32_t, uint16_t);
void serial_output_u32(uint32_t, uint32_t);

int initialise_serial(uint32_t, uint16_t);