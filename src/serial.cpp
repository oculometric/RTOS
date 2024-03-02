#include <serial.h>
#include <itos.h>

// TODO: implement interrupts

/**
 * configure the specified serial port
 * @param port serial port to initialise, specified using the provided #defines (COM1, COM2)
 * **/
uint8_t init_serial(uint32_t port)
{
    outb(port+SERIAL_INTERRUPT_REG, 0);             // disable interrupts
    outb(port+SERIAL_LINECONTROL_REG, 0b10000000);  // enable DLAB
    outb(port+SERIAL_DATA_REG, 4);                  // set baud divisor low byte
    outb(port+SERIAL_INTERRUPT_REG, 0);             // set baud divisor high byte, giving a 28800 baud rate
    outb(port+SERIAL_LINECONTROL_REG, 0b00000011);  // clear DLAB and put the line in 8-bit-no-parity-1-stop-bit mode
    outb(port+SERIAL_INTERRUPTID_REG, 0b11000111);  // enable FIFO, cleared, 14 bit threshold. probably useful at some point
    outb(port+SERIAL_MODEMCONTROL_REG, 0b00011110); // test loopback
    outb(port+SERIAL_DATA_REG, 0xAE);
    if (inb(port+SERIAL_DATA_REG) != 0xAE) return SERIAL_INIT_FAILURE;

    outb(port+SERIAL_MODEMCONTROL_REG, 0b00000111); // set in normal operation mode, no IRQs
    return SERIAL_INIT_SUCCESS;
}

/**
 * output a single character to serial
 * @param chr character to output
 * @param port serial port to direct output to 
 * **/
void serial_print(char chr, uint32_t port)
{
    while(!transmit_ready(port));
    outb(port+SERIAL_DATA_REG, chr);
}

/**
 * dump a region of memory of a specified length in bytes to serial, formatted in hexadecimal
 * @param start address of the first byte
 * @param length number of bytes to output
 * @param port serial port to direct output to
 * @param per_line number of bytes per line; set to zero for all bytes on a single line
 * **/
void serial_dump_hex_byte(void* start, uint32_t length, uint32_t port, uint8_t per_line)
{
    uint8_t* ptr = (uint8_t*)start;
    
    char outbuffer[6] = "0xNN ";
    uint32_t col = 0;               // keep track of column
    uint32_t offset = 0;
    while (offset < length)
    {
        // translate byte
        hex_to_string(ptr[offset], outbuffer, 2);
        for (uint8_t i = 0; i < 6 && outbuffer[i] != 0; i++) serial_print(outbuffer[i], port);
        col++;
        offset++;
        if (per_line != 0 && col >= per_line) { com_1 << port << nov::stream::endl; col = 0; }
    }
}

/**
 * dump a region of memory of a specified length in words to serial, formatted in hexadecimal
 * @param start address of the first word
 * @param length number of words to output
 * @param port serial port to direct output to
 * @param per_line number of words per line; set to zero for all words on a single line
 * **/
void serial_dump_hex_word(void* start, uint32_t length, uint32_t port, uint8_t per_line)
{
    uint16_t* ptr = (uint16_t*)start;
    
    char outbuffer[8] = "0xNNNN ";
    uint32_t col = 0;               // keep track of column
    uint32_t offset = 0;
    while (offset < length)
    {
        // translate byte
        hex_to_string(ptr[offset], outbuffer, 4);
        for (uint8_t i = 0; i < 8 && outbuffer[i] != 0; i++) serial_print(outbuffer[i], port);
        col++;
        offset++;
        if (per_line != 0 && col >= per_line) { com_1 << port << nov::stream::endl; col = 0; }
    }
}

/**
 * dump a region of memory of a specified length in doublewords to serial, formatted in hexadecimal
 * @param start address of the first dword
 * @param length number of dwords to output
 * @param port serial port to direct output to
 * @param per_line number of dwords per line; set to zero for all dwords on a single line
 * **/
void serial_dump_hex_dwrd(void* start, uint32_t length, uint32_t port, uint8_t per_line)
{
    uint32_t* ptr = (uint32_t*)start;
    
    char outbuffer[12] = "0xNNNNNNNN ";
    uint32_t col = 0;               // keep track of column
    uint32_t offset = 0;
    while (offset < length)
    {
        // translate byte
        hex_to_string(ptr[offset], outbuffer, 8);
        for (uint8_t i = 0; i < 12 && outbuffer[i] != 0; i++) serial_print(outbuffer[i], port);
        col++;
        offset++;
        if (per_line != 0 && col >= per_line) { com_1 << port << nov::stream::endl; col = 0; }
    }
}

/**
 * dump a region of memory of a specified length in bytes to serial, as the raw values
 * @param start address of the first byte
 * @param length number of bytes to output
 * @param port serial port to direct output to
 * @param per_line number of bytes per line; set to zero for all bytes on a single line
 * @param separator character to separate data characters, useful for making things more readable; set to null for no separation
 * **/
void serial_dump_byte(void* start, uint32_t length, uint32_t port, uint8_t per_line, char separator)
{
    uint8_t* ptr = (uint8_t*)start;
    uint32_t col = 0;
    uint32_t offset = 0;
    uint8_t chr;
    while (offset < length)
    {
        chr = ptr[offset];
        if (chr < ' ' || chr == 127) chr = '.';
        serial_print(chr, port);
        col++;
        offset++;
        if (separator != '\0') serial_print(separator, port);
        if (per_line != 0 && col >= per_line) { com_1 << port << nov::stream::endl; col = 0; }
    }
}

nov::stream::nov_stream com_1([](char c){ serial_print(c, COM1); });
nov::stream::nov_stream com_2([](char c){ serial_print(c, COM2); });