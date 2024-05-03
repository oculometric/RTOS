#pragma once

#include <stdint.h>

#define STREAM_BUFFER_SIZE 16

namespace nov
{
namespace stream
{

enum Mode
{
    DEC = 1,
    HEX,
    BIN
};

static const char endl = '\n';

class Stream
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
    Mode uint_mode = Mode::DEC;

    void send(char c);
    void flush();
    
    constexpr Stream(void (*flush_function)(char c)) : flush_char(flush_function) { }
};

Stream& operator<<(Stream& stream, char c);
Stream& operator<<(Stream& stream, char* s);
Stream& operator<<(Stream& stream, const char* s);
Stream& operator<<(Stream& stream, uint32_t u);
Stream& operator<<(Stream& stream, int32_t i);
inline Stream& operator<<(Stream& stream, int i) { return stream << (int32_t)i; }
Stream& operator<<(Stream& stream, float f);
Stream& operator<<(Stream& stream, Mode m);

}
}