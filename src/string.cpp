#include <string.h>
#include <panic.h>

using namespace nov;

nov_string::nov_string()
{
    com_1 << "base constructor" << stream::endl;
    backing.resize(8);
}

nov_string::nov_string(const nov_string& str)
{
    com_1 << "copy constructor from " << stream::mode::HEX << (uint32_t)(&str) << stream::endl;

    backing.resize(str.get_length());
    for (uint32_t i = 0; i < str.get_length(); i++)
    {
        backing.push(str[i]);
    }
}

nov_string::nov_string(const char* chrs)
{
    com_1 << "char* constructor from " << stream::mode::HEX << (uint32_t)chrs << stream::endl;

    uint32_t chr_len = 0;
    while (chrs[chr_len] != 0x0)
        chr_len++;
    
    backing.resize(chr_len);

    for (uint32_t i = 0; i < chr_len; i++) backing.push(chrs[i]);
}

nov_string::nov_string(char* chrs)
{
    com_1 << "char* constructor from " << stream::mode::HEX << (uint32_t)chrs << stream::endl;

    uint32_t chr_len = 0;
    while (chrs[chr_len] != 0x0)
        chr_len++;
    
    backing.resize(chr_len);
    for (uint32_t i = 0; i < chr_len; i++) backing.push(chrs[i]);
}

nov_string::nov_string(uint32_t initial_capacity)
{
    com_1 << "initial capacity constructor sized " << stream::mode::DEC << initial_capacity << stream::endl;

    backing.resize(initial_capacity);
}

nov_string::nov_string(nov_string&& str)
{
    com_1 << "move constructor from " << stream::mode::HEX << (uint32_t)(&str) << stream::endl;

    backing.resize(str.get_length());
    for (uint32_t i = 0; i < str.get_length(); i++)
    {
        backing.push(str[i]);
    }
}

nov_string& nov::nov_string::operator=(const char* chrs)
{
    clear();
    append(chrs);
    return *this;
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

uint32_t nov_string::get_capacity() const
{
    return backing.get_capacity();
}

const char* nov_string::const_str() const
{
    uint32_t len = get_length();
    char* copy_buf = new char[len+1];
    memory::memset((char)0, copy_buf, len+1);
    for (uint32_t i = 0; i < len; i++) copy_buf[i] = (*this)[i];
    return copy_buf;
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
    if (end <= start) { return nov_string(); }
    if (start >= backing.get_length()) { return nov_string(); }
    nov_string substr((end < backing.get_length() ? end : backing.get_length()) - start);
    for (uint32_t i = start; i < end && i < backing.get_length(); i++)
        substr.append((*this)[i]);
    return substr;
}

nov_string::~nov_string()
{
    com_1 << "destructor" << stream::endl;
}

uint32_t nov::find_next_byte(char* addr, char target)
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
uint16_t nov::int_to_string(const uint32_t i, const uint8_t base, char* buffer, const uint8_t padding)
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

nov_string nov::int_to_string(const uint32_t i, const uint8_t base, const uint8_t padding)
{
    uint16_t size = padding > 33 ? padding : 33;
    char* buffer = new char[size];
    memory::memset((char)0, buffer, size);
    int_to_string(i, base, buffer, padding);
    nov_string str(buffer);
    delete[] buffer;
    return str;
}

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

int32_t nov::string_to_int(const char* str, uint8_t base)
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

float nov::string_to_float(const char* str)
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

int32_t nov::string_to_int(const nov_string& str, uint8_t base)
{
    return string_to_int(str.const_str(), base);
}

float nov::string_to_float(const nov_string& str)
{
    return string_to_float(str.const_str());
}

nov_string nov::float_to_string(const float f, const uint8_t dps)
{
    uint16_t size = 64 + dps + 3;
    char* buffer = new char[size];
    memory::memset((char)0, buffer, size);
    float_to_string(f, buffer, dps);
    nov_string str(buffer);
    delete[] buffer;
    return str;
}

stream::nov_stream& nov::operator<<(stream::nov_stream& stream, const nov_string& s)
{
    for (uint32_t i = 0; i < s.get_length(); i++)
    {
        stream << s[i];
    }
    return stream;
}
