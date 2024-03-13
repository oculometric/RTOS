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
    nov_string(nov_string&& str);

    nov_string& operator=(const nov_string&) = delete;
    nov_string& operator=(nov_string&&) = delete;

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

    const char* const_str() const;

    uint32_t get_length() const;
    uint32_t get_capacity() const;

    int32_t find(char c, uint32_t start = 0) const;
    nov_string substring(uint32_t start, uint32_t end = -1) const;
    void split (nov_array<nov_string>& out_array, char c) const; // TODO: string splitting

    ~nov_string();
};

uint32_t find_next_byte(char* addr, char target);

uint16_t int_to_string(const uint32_t i, const uint8_t base, char* buffer, const uint8_t padding = 0);
void float_to_string(const float f, char* buffer, const uint8_t dps = 3);

nov_string int_to_string(const uint32_t i, const uint8_t base, const uint8_t padding = 0);
nov_string float_to_string(const float f, const uint8_t dps = 3);

int32_t string_to_int(const char* str, uint8_t base);
float string_to_float(const char* str);

int32_t string_to_int(const nov_string& str, uint8_t base);
float string_to_float(const nov_string& str);

stream::nov_stream& operator<<(stream::nov_stream& stream, const nov_string& s);

}