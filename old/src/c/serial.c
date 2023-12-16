#include <serial.h>

void serial_output_c(uint32_t port, uint8_t c)
{
    // wait for line to be clear
    while (!(read_COM_register(port, LINE_STATUS) & 0x20));
    // write the character
    write_COM_register(port, DATA, c);
}

uint8_t serial_input(uint32_t port)
{
    // wait for data to be available
    while (!(read_COM_register(port, LINE_STATUS) & 0x01));
    // read the character
    return read_COM_register(port, DATA);
}

void serial_output(uint32_t port, uint8_t* s)
{
    // write the character
    for (size_t i = 0; s[i] != '\0'; i++)
        serial_output_c(port, s[i]);
}

void serial_output_length(uint32_t port, uint8_t* s, size_t n)
{
    // write the character
    for (size_t i = 0; i < n; i++)
        serial_output_c(port, s[i]);
}

void serial_output_u8(uint32_t port, uint8_t value)
{
    serial_output_c(port, '0');
    serial_output_c(port, 'x');
    serial_output_c(port, CHARS_FOR_ITOS[(value >> 4) & 0xF]);
    serial_output_c(port, CHARS_FOR_ITOS[value & 0xF]);
}

void serial_output_u16(uint32_t port, uint16_t value)
{
    serial_output_c(port, '0');
    serial_output_c(port, 'x');
    serial_output_c(port, CHARS_FOR_ITOS[(value >> 12) & 0xF]);
    serial_output_c(port, CHARS_FOR_ITOS[(value >> 8) & 0xF]);
    serial_output_c(port, CHARS_FOR_ITOS[(value >> 4) & 0xF]);
    serial_output_c(port, CHARS_FOR_ITOS[value & 0xF]);
}

void serial_output_u32(uint32_t port, uint32_t value)
{
    serial_output_c(port, '0');
    serial_output_c(port, 'x');
    serial_output_c(port, CHARS_FOR_ITOS[(value >> 28) & 0xF]);
    serial_output_c(port, CHARS_FOR_ITOS[(value >> 24) & 0xF]);
    serial_output_c(port, CHARS_FOR_ITOS[(value >> 20) & 0xF]);
    serial_output_c(port, CHARS_FOR_ITOS[(value >> 16) & 0xF]);
    serial_output_c(port, CHARS_FOR_ITOS[(value >> 12) & 0xF]);
    serial_output_c(port, CHARS_FOR_ITOS[(value >> 8) & 0xF]);
    serial_output_c(port, CHARS_FOR_ITOS[(value >> 4) & 0xF]);
    serial_output_c(port, CHARS_FOR_ITOS[value & 0xF]);
}

int initialise_serial(uint32_t port, uint16_t baud)
{
    // line config:
    // 8 bits:      0bxxxxxx11
    // 1 stop bit:  0bxxxxx0xx
    // no parity:   0bxx000xxx

    // disable interrupts
    write_COM_register(port, INTERRUPT_ENABLE, 0x00);
    // set DLAB
    write_COM_register(port, LINE_CONTROL, 0x80);
    // set baud rate
    uint16_t baud_rate_divisor = 115200/baud;
    write_COM_register(port, DATA, baud_rate_divisor & 0xFF);
    write_COM_register(port, INTERRUPT_ENABLE, (baud_rate_divisor >> 8) & 0xFF);
    // set line config, and clear DLAB
    write_COM_register(port, LINE_CONTROL, 0x03);
    // enable FIFO, clear them, with 14 bit threshold (why do we do this? nobody knows!)
    write_COM_register(port, INTERRUPT_ID, 0xC7);
    // enable OUT2 pin/IRQ, request to send pin (RTS), and data terminal ready (DTR)
    write_COM_register(port, MODEM_CONTROL, 0x0B);

    // perform loopback test
    // set loop, OUT1, OUT2, RTS
    write_COM_register(port, MODEM_CONTROL, 0x1E);
    // send test byte
    write_COM_register(port, DATA, 0xAE);

    if (read_COM_register(port, DATA) != 0xAE)
        return 1; // serial port faulty or i fucked something up

    // enter normal operation
    write_COM_register(port, MODEM_CONTROL, 0x0F);
    return 0;
}