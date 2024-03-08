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
void nov_stream::send(char c)
{
    buffer[buffer_index] = c;
    buffer_index++;
    if (buffer_index >= STREAM_BUFFER_SIZE) flush();
}

/**
 * flushes all characters from the stream buffer, clearing the buffer
 * and outputting all characters sequentially according to `flush_char`
 * **/
void nov_stream::flush()
{
    for (uint8_t i = 0; i < buffer_index; i++)
    {
        flush_char(buffer[i]);
    }
    buffer_index = 0;
}

nov_stream& operator<<(nov_stream& stream, char c)
{
    stream.send(c);
    return stream;
}

nov_stream& operator<<(nov_stream& stream, char* s)
{
    uint32_t i = 0;
    while (s[i] != 0) stream.send(s[i++]);
    return stream;
}

nov_stream& operator<<(nov_stream& stream, const char* s)
{
    uint32_t i = 0;
    while (s[i] != 0) stream.send(s[i++]);
    return stream;
}

nov_stream& operator<<(nov_stream& stream, uint32_t u)
{
    char buf[36] = { 0 };
    switch (stream.uint_mode)
    {
    case mode::DEC: nov::int_to_string(u, 10, buf); break;
    case mode::BIN: stream << "0b"; nov::int_to_string(u, 2, buf); break;
    case mode::HEX: stream << "0x"; nov::int_to_string(u, 16, buf); break;
    default: return stream;
    }
    return stream << buf;
}

nov_stream& operator<<(nov_stream& stream, int32_t i)
{
    char buf[16] = { 0 };
    if (i < 0)
    {
        buf[0] = '-';
        nov::int_to_string(-i, 10, buf+1);
        return stream << buf;
    }
    else
    {
        return stream << (uint32_t)i;
    }
}

nov_stream& operator<<(nov_stream& stream, float f)
{
    char buf[128] = { 0 };
    nov::float_to_string(f, buf, 3);
    return stream << buf;
}

nov_stream& operator<<(nov_stream& stream, mode m)
{
    stream.uint_mode = m;
    return stream;
}

}
}
