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

inline uint8_t read_COM_register(uint32_t port, COM register);
inline void write_COM_register(uint32_t port, COM register);

inline void set_baud_rate(uint32_t port, uint32_t br);
inline void serial_output(uint32_t port, uint8_t c);
inline uint8_t serial_input(uint32_t port);