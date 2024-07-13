#pragma once

#include <array.h>
#include <stream.h>

namespace nov
{

class String
{
private:
    Array<char> backing;
public:
    String();
    String(const String& str);
    String(const char* chrs);
    String(char* chrs);
    String(uint32_t initial_capacity);
    String(String&& str);

    String& operator=(const char* chrs);
    
    String& operator=(const String&) = delete;
    String& operator=(String&&) = delete;

    char& operator[](uint32_t index);
    char operator[](uint32_t index) const;
    void append(char c);
    void append(char* c);
    void append(const char* c);
    void append(const String& str);
    void operator+=(char c);
    void operator+=(char* c);
    void operator+=(const char* c);
    void operator+=(const String& str);

    void resize(uint32_t new_capacity);
    void clear();
    void pop(uint32_t num = 1);

    const char* constStr() const;

    uint32_t getLength() const;
    uint32_t getCapacity() const;

    int32_t find(char c, uint32_t start = 0) const;
    String substring(uint32_t start, uint32_t end = -1) const;
    void split (Array<String>& out_array, char c) const; // TODO: string splitting

    ~String();
};

uint32_t findNextByte(char* addr, char target);

String intToString(const uint32_t i, const uint8_t base, const uint8_t padding = 0);
String floatToString(const float f, const uint8_t dps = 3);

int32_t stringToInt(const String& str, uint8_t base);
float stringToFloat(const String& str);

bool strcmp(char* a, char* b, uint32_t len = -1);

stream::Stream& operator<<(stream::Stream& stream, const String& s);

}