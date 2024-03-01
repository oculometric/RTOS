#pragma once

#include <stdint.h>

#define STREAM_BUFFER_SIZE 16

namespace nov
{
namespace stream
{

enum mode
{
    DEC = 1,
    HEX,
    BIN
};

static const char endl = '\n';

class nov_stream
{
private:
    // buffer to hold characters so they can be written as a single block later
    char buffer[STREAM_BUFFER_SIZE] = { 0 };
    // index into the buffer where the next character should be placed
    uint8_t buffer_index = 0;

    /**
     * internal method which performs the actual writing of characters from the buffer
     * to the intended output. this should be overwritten by inheriting classes to perform
     * the actual outputting of data, and does not need to handle management of the buffer
     * 
     * @param c character to flush
     * **/
    void (*flush_char)(char c) = 0x0;
public:
    // mode in which to output numbers
    mode uint_mode = mode::DEC;

    void send(char c);
    void flush();
    
    constexpr nov_stream(void (*flush_function)(char c)) : flush_char(flush_function) { }
};

nov_stream& operator<<(nov_stream& stream, char c);
nov_stream& operator<<(nov_stream& stream, char* s);
nov_stream& operator<<(nov_stream& stream, const char* s);
nov_stream& operator<<(nov_stream& stream, uint32_t u);
nov_stream& operator<<(nov_stream& stream, int32_t i);
inline nov_stream& operator<<(nov_stream& stream, int i) { return stream << (int32_t)i; }
nov_stream& operator<<(nov_stream& stream, float f);
nov_stream& operator<<(nov_stream& stream, mode m);

}
}