#include <stream.h>
#include <string.h>

namespace nov
{
namespace stream
{

/**
 * pushes a character onto the stream buffer
 * 
 * @param c character to send
 * **/
void Stream::send(char c)
{
    buffer[buffer_index] = c;
    buffer_index++;
    if (buffer_index >= STREAM_BUFFER_SIZE) flush();
}

/**
 * flushes all characters from the stream buffer, clearing the buffer
 * and outputting all characters sequentially according to `flush_char`
 * **/
void Stream::flush()
{
    for (uint8_t i = 0; i < buffer_index; i++)
    {
        flush_char(buffer[i]);
    }
    buffer_index = 0;
}

Stream& operator<<(Stream& stream, char c)
{
    stream.send(c);
    return stream;
}

Stream& operator<<(Stream& stream, char* s)
{
    uint32_t i = 0;
    while (s[i] != 0) stream.send(s[i++]);
    return stream;
}

Stream& operator<<(Stream& stream, const char* s)
{
    uint32_t i = 0;
    while (s[i] != 0) stream.send(s[i++]);
    return stream;
}

Stream& operator<<(Stream& stream, uint32_t u)
{
    char buf[36] = { 0 };
    switch (stream.uint_mode)
    {
    case Mode::DEC: nov::intToString(u, 10, buf); break;
    case Mode::BIN: stream << "0b"; nov::intToString(u, 2, buf); break;
    case Mode::HEX: stream << "0x"; nov::intToString(u, 16, buf); break;
    default: return stream;
    }
    return stream << buf;
}

Stream& operator<<(Stream& stream, int32_t i)
{
    char buf[16] = { 0 };
    if (i < 0)
    {
        buf[0] = '-';
        nov::intToString(-i, 10, buf+1);
        return stream << buf;
    }
    else
    {
        return stream << (uint32_t)i;
    }
}

Stream& operator<<(Stream& stream, float f)
{
    char buf[128] = { 0 };
    nov::floatToString(f, buf, 3);
    return stream << buf;
}

Stream& operator<<(Stream& stream, Mode m)
{
    stream.uint_mode = m;
    return stream;
}

}

static const char* itos_chars = "0123456789abcdefghijklmnopqrstuvwxyz";

static inline uint8_t value_for_char(const char c)
{
    if (c < '0') return 0;
    if (c <= '9') return c - '0';
    if (c < 'A') return 0;
    if (c <= 'Z') return (c - 'A') + 10;
    if (c < 'a') return 0;
    if (c <= 'z') return (c - 'a') + 10;
    return 0;
}

/**
 * convert an integer type into a string description, in a given base, and place the result into a buffer. please make sure you allocated enough buffer
 * @param i integer to convert
 * @param base numerical base to convert to
 * @param buffer pointer to the buffer where the result should be placed
 * @param padding minimum number of digits to output
 * **/
uint16_t intToString(const uint32_t i, const uint8_t base, char* buffer, const uint8_t padding)
{
    if (base > 36 || base < 2)
    {
        buffer[0] = 'E';
        buffer[1] = 'R';
        buffer[2] = 'R'; // probably shouldn't do this but oh well
        return 0;
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

    return digits;
}

/**
 * convert a float into a string description, placing the result in a buffer. make sure you allocate
 * enough space in the buffer! whole digits + decimal places (default 3) + 4
 * 
 * @param f floating point value to convert
 * @param buffer pointer to the buffer into which the result is written
 * @param dps decimal places to output
 * **/
void floatToString(const float f, char* buffer, const uint8_t dps)
{
    uint16_t buffer_offset = 0;
    float real_f = f;
    if (f < 0)
    {
        buffer[buffer_offset] = '-';
        real_f *= -1.0f;
        buffer_offset++;
    }

    uint32_t whole = (uint32_t)real_f;
    float fract_f = real_f - (float)whole;
    for (uint8_t i = 0; i < dps; i++) fract_f *= 10;
    uint32_t fract = (uint32_t)fract_f;

    buffer_offset += intToString(whole, 10, buffer + buffer_offset);
    buffer[buffer_offset] = '.';
    buffer_offset++;
    uint8_t real_dps = intToString(fract, 10, buffer + buffer_offset);
    buffer_offset += real_dps;
    
    for (uint8_t i = real_dps; i < dps; i++)
    {
        buffer[buffer_offset] = '0';
        buffer_offset++;
    }
}

int32_t stringToInt(const char* str, uint8_t base)
{
    uint32_t length = 0;
    while (str[length] != 0x0) length++;

    int32_t num = 0;
    bool positive = true;
    uint32_t i = 1;
    uint32_t f = 1;
    uint8_t v = value_for_char(0);
    while(i <= length)
    {
        v = value_for_char(str[length-i]);
        if (str[length-i] == '-')
        {
            if (length-i == 0) positive = false;
            else return 0; 
        }
        else if (str[length-i] == '.') { num = 0; f = 1; }
        else if (str[length-i] == ' ') break;
        else if (v < base && str[length-i] >= '0')
        {
            num += v * f;
            f *= base;
        } else return 0;

        i++;
    }
    return positive ? num : -num;
}

float stringToFloat(const char* str)
{
    float f = 0.1f;
    uint32_t i = 0;
    while (str[i] != '.') { f *= 10; i++; }
    float num = 0.0f;
    bool positive = true;
    uint8_t v = value_for_char(0);
    i = 0;
    while (str[i] != 0x0)
    {
        v = value_for_char(str[i]);
        if (str[i] == '-')
        {
            if (i == 0) positive = false;
            else return 0.0f;
        }
        else if (str[i] == '.') { }
        else if (str[i] == ' ') break;
        else if (v < 10 && str[i] >= '0')
        {
            num += f * v;
            f /= 10;
        } else return 0.0f;

        i++;
    }

    return positive ? num : num * -0.1f;
}

}
