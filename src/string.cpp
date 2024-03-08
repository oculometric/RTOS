#include <string.h>
#include <panic.h>

using namespace nov;

nov_string::nov_string()
{
}

nov_string::nov_string(const nov_string& str)
{
    backing.resize(str.get_length());
    for (uint32_t i = 0; i < str.get_length(); i++)
    {
        backing.push(str[i]);
    }
}

nov_string::nov_string(const char* chrs)
{
    uint32_t chr_len = 0;
    while (chrs[chr_len] != 0x0)
        chr_len++;
    
    backing.resize(chr_len);

    for (uint32_t i = 0; i < chr_len; i++) backing.push(chrs[i]);
}

nov_string::nov_string(char* chrs)
{
    uint32_t chr_len = 0;
    while (chrs[chr_len] != 0x0)
        chr_len++;
    
    backing.resize(chr_len);
    for (uint32_t i = 0; i < chr_len; i++) backing.push(chrs[i]);
}

nov_string::nov_string(uint32_t initial_capacity)
{
    backing.resize(initial_capacity);
}

char& nov_string::operator[](uint32_t index)
{
    if (index >= backing.get_length()) panic();
    return backing[index];
}

char nov_string::operator[](uint32_t index) const
{
    if (index >= backing.get_length()) panic();
    return backing[index];
}

void nov_string::append(char c)
{
    backing.push(c);
}

void nov::nov_string::append(char* c)
{
    uint32_t size = 0;
    for (size = 0; c[size] != 0x0; size++)
    backing.resize(backing.get_capacity() + size);
    for (uint32_t i = 0; i < size; i++)
        backing.push(c[i]);
}

void nov::nov_string::append(const char* c)
{
    uint32_t size = 0;
    for (size = 0; c[size] != 0x0; size++)
    backing.resize(backing.get_capacity() + size);
    for (uint32_t i = 0; i < size; i++)
        backing.push(c[i]);
}

void nov::nov_string::append(const nov_string& str)
{
    backing.resize(backing.get_capacity() + str.get_length());
    for (uint32_t i = 0; i < str.get_length(); i++)
        backing.push(str[i]);
}

void nov_string::operator+=(char c)
{
    append(c);
}

void nov_string::operator+=(char* c)
{
    append(c);
}

void nov_string::operator+=(const char* c)
{
    append(c);
}

void nov_string::operator+=(const nov_string& str)
{
    append(str);
}

void nov_string::resize(uint32_t new_capacity)
{
    backing.resize(new_capacity);
}

void nov_string::clear()
{
    backing.clear();
}

uint32_t nov_string::get_length() const
{
    return backing.get_length();
}

int32_t nov_string::find(char c, uint32_t start) const
{
    if (start >= backing.get_length()) return -1;
    for (uint32_t i = start; i < backing.get_length(); i++)
        if (backing[i] == c) return i;
    
    return -1;
}

nov_string nov_string::substring(uint32_t start, uint32_t end) const
{
    if (end <= start) return nov_string();
    if (start >= backing.get_length()) return nov_string();
    nov_string substr(end-start);
    for (uint32_t i = start; i < end; i++)
        substr.append((*this)[i]);
    return substr;
}

nov_string::~nov_string()
{
    backing.~nov_array();
}

uint32_t nov::find_next_byte(char *addr, char target)
{
    uint32_t ind = 0;
    while (true)
    {
        if (addr[ind] == target) return ind;
        if (addr[ind] == 0x0) return -1;
        if (ind == 0xffffffff) return -1;
        ind++;
    }

    return -1;
}

static const char* itos_chars = "0123456789abcdefghijklmnopqrstuvwxyz";

/**
 * convert an integer type into a string description, in a given base, and place the result into a buffer. please make sure you allocated enough buffer
 * @param i integer to convert
 * @param base numerical base to convert to
 * @param buffer pointer to the buffer where the result should be placed
 * @param padding minimum number of digits to output
 * **/
uint16_t nov::int_to_string(uint32_t i, uint8_t base, char* buffer, uint8_t padding)
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
void nov::float_to_string(const float f, char* buffer, const uint8_t dps)
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

    buffer_offset += int_to_string(whole, 10, buffer + buffer_offset);
    buffer[buffer_offset] = '.';
    buffer_offset++;
    uint8_t real_dps = int_to_string(fract, 10, buffer + buffer_offset);
    buffer_offset += real_dps;
    
    for (uint8_t i = real_dps; i < dps; i++)
    {
        buffer[buffer_offset] = '0';
        buffer_offset++;
    }
}

/**
 * converts an integer to a hexadecimal (base 16) string and places it in the provied buffer. make sure you allocate enough space
 * @param i integer to convert
 * @param buffer pointer to the buffer where the result will be placed
 * @param padding minimum number of digits to output
 * **/
void nov::hex_to_string(uint32_t i, char* buffer, uint8_t padding)
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
void nov::bin_to_string(uint32_t i, char* buffer, uint8_t padding)
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
void nov::dec_to_string(uint32_t i, char* buffer, uint8_t padding)
{
    int_to_string(i, 10, buffer, padding);
}

stream::nov_stream& nov::operator<<(stream::nov_stream& stream, const nov_string& s)
{
    for (uint32_t i = 0; i < s.get_length(); i++)
    {
        stream << s[i];
    }
    return stream;
}
