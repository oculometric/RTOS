#pragma once

#include <stdint.h>
#include <stddef.h>

#include "io.h"

#define COM1 0x3F8
#define COM2 0x2F8

enum COM
{
    DATA = 0,
    INTERRUPT_ENABLE,
    INTERRUPT_ID,
    LINE_CONTROL,
    MODEM_CONTROL,
    LINE_STATUS,
    MODEM_STATUS,
    SCRATCH
};

inline uint8_t read_COM_register(uint32_t port, COM reg)
{
    return inb(port+reg);
}

inline void write_COM_register(uint32_t port, COM reg, uint8_t value)
{
    return outb(port+reg, value);
}

void serial_output(uint32_t, uint8_t);

uint8_t serial_input(uint32_t);

void serial_output(uint32_t, uint8_t*);

void serial_output(uint32_t, uint8_t*, size_t);

void serial_output_u8(uint32_t, uint8_t);
void serial_output_u16(uint32_t, uint16_t);
void serial_output_u32(uint32_t, uint32_t);

int initialise_serial(uint32_t, uint16_t);