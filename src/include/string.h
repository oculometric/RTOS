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

uint16_t int_to_string(uint32_t i, uint8_t base, char* buffer, uint8_t padding = 0);
void float_to_string(const float f, char* buffer, const uint8_t dps = 3);
void hex_to_string(uint32_t i, char* buffer, uint8_t padding = 0);
void bin_to_string(uint32_t i, char* buffer, uint8_t padding = 0);
void dec_to_string(uint32_t i, char* buffer, uint8_t padding = 0);

int32_t string_to_int(const nov_string& str, uint8_t base);
float string_to_float(const nov_string& str);

stream::nov_stream& operator<<(stream::nov_stream& stream, const nov_string& s);

}