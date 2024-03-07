#pragma once

#include <array.h>
#include <stream.h>

namespace nov
{

class nov_string
{
private:
    nov_array<char> backing;
public:
    nov_string();
    nov_string(const nov_string& str);
    nov_string(const char* chrs);
    nov_string(char* chrs);
    nov_string(uint32_t initial_capacity);

    char& operator[](uint32_t index);
    char operator[](uint32_t index) const;
    void append(char c);
    void append(char* c);
    void append(const char* c);
    void append(const nov_string& str);
    void operator+=(char c);
    void operator+=(char* c);
    void operator+=(const char* c);
    void operator+=(const nov_string& str);

    void resize(uint32_t new_capacity);
    void clear();
    uint32_t get_length() const;

    int32_t find(char c, uint32_t start = 0) const;
    nov_string substring(uint32_t start, uint32_t end = -1) const;

    ~nov_string();
};

uint32_t find_next_byte(char* addr, char target);

stream::nov_stream& operator<<(stream::nov_stream& stream, const nov_string& s);

}