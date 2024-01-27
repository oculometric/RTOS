#include "stdint.h"

static const char * itos_chars = "0123456789abcdefghijklmnopqrstuvwxyz";

/**
 * convert an integer type into a string description, in a given base, and place the result into a buffer. please make sure you allocated enough buffer
 * @param i integer to convert
 * @param base numerical base to convert to
 * @param buffer pointer to the buffer where the result should be placed
 * @param padding minimum number of digits to output
 * **/
inline void int_to_string(uint32_t i, uint8_t base, char* buffer, uint8_t padding = 0)
{
    if (base > 36 || base < 2)
    {
        buffer[0] = 'E';
        buffer[1] = 'R';
        buffer[2] = 'R'; // probably shouldn't do this but oh well
        return;
    }

    uint16_t digits = 1;
    uint32_t tmp = i / base;
    while (tmp > 0) { digits++; tmp /= base; }

    uint16_t buffer_offset = digits;
    if (buffer_offset < padding) buffer_offset = padding;
    buffer_offset--;

    tmp = i;
    while (buffer_offset > 0)
    {
        buffer[buffer_offset] = itos_chars[tmp % base];
        buffer_offset--;
        tmp /= base;
    }
    buffer[buffer_offset] = itos_chars[tmp % base];
}

/**
 * converts an integer to a hexadecimal (base 16) string and places it in the provied buffer. make sure you allocate enough space
 * @param i integer to convert
 * @param buffer pointer to the buffer where the result will be placed
 * @param padding minimum number of digits to output
 * **/
inline void hex_to_string(uint32_t i, char* buffer, uint8_t padding = 0)
{
    buffer[0] = '0';
    buffer[1] = 'x';
    int_to_string(i, 16, buffer+2, padding);
}

/**
 * converts an integer to a binary (base 2) string and places it in the provied buffer. make sure you allocate enough space
 * @param i integer to convert
 * @param buffer pointer to the buffer where the result will be placed
 * @param padding minimum number of digits to output
 * **/
inline void bin_to_string(uint32_t i, char* buffer, uint8_t padding = 0)
{
    buffer[0] = '0';
    buffer[1] = 'b';
    int_to_string(i, 2, buffer+2, padding);
}

/**
 * converts an integer to a denary (base 10) string and places it in the provied buffer. make sure you allocate enough space
 * @param i integer to convert
 * @param buffer pointer to the buffer where the result will be placed
 * @param padding minimum number of digits to output
 * **/
inline void dec_to_string(uint32_t i, char* buffer, uint8_t padding = 0)
{
    int_to_string(i, 10, buffer, padding);
}