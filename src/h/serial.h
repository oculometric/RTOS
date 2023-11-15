#pragma once

#include <stdint.h>
#include <stddef.h>

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
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port+reg));
    return ret;
}

inline void write_COM_register(uint32_t port, COM reg, uint8_t value)
{
    asm volatile("outb %1, %0" : : "dN" (port+reg), "a" (value));
}

inline void serial_output(uint32_t port, uint8_t c)
{
    // wait for line to be clear
    while (!(read_COM_register(port, COM::LINE_STATUS) & 0b00100000));
    // write the character
    write_COM_register(port, COM::DATA, c);
}

inline uint8_t serial_input(uint32_t port)
{
    // wait for data to be available
    while (!(read_COM_register(port, COM::LINE_STATUS) & 0b00000001));
    // read the character
    return read_COM_register(port, COM::DATA);
}

inline void serial_output(uint32_t port, uint8_t* s)
{
    // write the character
    for (size_t i = 0; s[i] != '\0'; i++)
        serial_output(port, s[i]);
}

inline void serial_output(uint32_t port, uint8_t* s, size_t n)
{
    // write the character
    for (size_t i = 0; i < n; i++)
        serial_output(port, s[i]);
}

int initialise_serial(uint32_t port, uint16_t baud)
{
    // line config:
    // 8 bits:      0bxxxxxx11
    // 1 stop bit:  0bxxxxx0xx
    // no parity:   0bxx000xxx

    // disable interrupts
    write_COM_register(port, COM::INTERRUPT_ENABLE, 0x00);
    // set DLAB
    write_COM_register(port, COM::LINE_CONTROL, 0b10000000);
    // set baud rate
    uint16_t baud_rate_divisor = 115200/baud;
    write_COM_register(port, COM::DATA, baud_rate_divisor & 0b11111111);
    write_COM_register(port, COM::INTERRUPT_ENABLE, (baud_rate_divisor >> 8) & 0b11111111);
    // set line config, and clear DLAB
    write_COM_register(port, COM::LINE_CONTROL, 0b00000011);
    // enable FIFO, clear them, with 14 bit threshold (why do we do this? nobody knows!)
    write_COM_register(port, COM::INTERRUPT_ID, 0xC7);
    // enable OUT2 pin/IRQ, request to send pin (RTS), and data terminal ready (DTR)
    write_COM_register(port, COM::MODEM_CONTROL, 0b00001011);

    // perform loopback test
    // set loop, OUT1, OUT2, RTS
    write_COM_register(port, COM::MODEM_CONTROL, 0b00011110);
    // send test byte
    write_COM_register(port, COM::DATA, 0xAE);

    if (read_COM_register(port, COM::DATA) != 0xAE)
        return 1; // serial port faulty or i fucked something up

    // enter normal operation
    write_COM_register(port, COM::MODEM_CONTROL, 0b00001111);
    return 0;
}