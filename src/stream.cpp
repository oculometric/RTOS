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
}
