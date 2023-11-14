#pragma once

#include <stdint.h>

#define COM1 0x3F8
#define COM2 0x2F8

enum COM
{
    DATA,
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
    return ((uint8_t*)port)[reg];
}

inline void write_COM_register(uint32_t port, COM reg, uint8_t value)
{
    ((uint8_t*)port)[reg] = value;
}

inline void set_baud_rate(uint32_t port, uint16_t br)
{
    // set the DLAB
    uint8_t v = read_COM_register(port, COM::LINE_CONTROL);
    write_COM_register(port, COM::LINE_CONTROL, v | 0b10000000);
    // place the high and low bytes
    write_COM_register(port, COM::DATA, br & 0b11111111);
    write_COM_register(port, COM::INTERRUPT_ENABLE, (br >> 8) & 0b11111111);
    // clear DLAB
    write_COM_register(port, COM::LINE_CONTROL, v);
}

inline void serial_output(uint32_t port, uint8_t c);
inline uint8_t serial_input(uint32_t port);