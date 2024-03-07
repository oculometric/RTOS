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

stream::nov_stream& nov::operator<<(stream::nov_stream& stream, const nov_string& s)
{
    for (uint32_t i = 0; i < s.get_length(); i++)
    {
        stream << s[i];
    }
    return stream;
}
