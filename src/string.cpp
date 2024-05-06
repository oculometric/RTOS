#include <string.h>
#include <panic.h>

using namespace nov;

String::String()
{
    com_1 << "base constructor" << stream::endl;
    backing.resize(8);
}

String::String(const String& str)
{
    com_1 << "copy constructor from " << stream::Mode::HEX << (uint32_t)(&str) << stream::endl;

    backing.resize(str.getLength());
    for (uint32_t i = 0; i < str.getLength(); i++)
    {
        backing.push(str[i]);
    }
}

String::String(const char* chrs)
{
    com_1 << "char* constructor from " << stream::Mode::HEX << (uint32_t)chrs << stream::endl;

    uint32_t chr_len = 0;
    while (chrs[chr_len] != 0x0)
        chr_len++;
    
    backing.resize(chr_len);

    for (uint32_t i = 0; i < chr_len; i++) backing.push(chrs[i]);
}

String::String(char* chrs)
{
    com_1 << "char* constructor from " << stream::Mode::HEX << (uint32_t)chrs << stream::endl;

    uint32_t chr_len = 0;
    while (chrs[chr_len] != 0x0)
        chr_len++;
    
    backing.resize(chr_len);
    for (uint32_t i = 0; i < chr_len; i++) backing.push(chrs[i]);
}

String::String(uint32_t initial_capacity)
{
    com_1 << "initial capacity constructor sized " << stream::Mode::DEC << initial_capacity << stream::endl;

    backing.resize(initial_capacity);
}

String::String(String&& str)
{
    com_1 << "move constructor from " << stream::Mode::HEX << (uint32_t)(&str) << stream::endl;

    backing.resize(str.getLength());
    for (uint32_t i = 0; i < str.getLength(); i++)
    {
        backing.push(str[i]);
    }
}

String& nov::String::operator=(const char* chrs)
{
    clear();
    append(chrs);
    return *this;
}

char& String::operator[](uint32_t index)
{
    if (index >= backing.getLength()) panic("index out of range");
    return backing[index];
}

char String::operator[](uint32_t index) const
{
    if (index >= backing.getLength()) panic("index out of range");
    return backing[index];
}

void String::append(char c)
{
    backing.push(c);
}

void nov::String::append(char* c)
{
    uint32_t size = 0;
    for (size = 0; c[size] != 0x0; size++) { }
    backing.resize(backing.getCapacity() + size);
    for (uint32_t i = 0; i < size; i++)
        backing.push(c[i]);
}

void nov::String::append(const char* c)
{
    uint32_t size = 0;
    for (size = 0; c[size] != 0x0; size++) { }
    backing.resize(backing.getCapacity() + size);
    for (uint32_t i = 0; i < size; i++)
        backing.push(c[i]);
}

void nov::String::append(const String& str)
{
    backing.resize(backing.getCapacity() + str.getLength());
    for (uint32_t i = 0; i < str.getLength(); i++)
        backing.push(str[i]);
}

void String::operator+=(char c)
{
    append(c);
}

void String::operator+=(char* c)
{
    append(c);
}

void String::operator+=(const char* c)
{
    append(c);
}

void String::operator+=(const String& str)
{
    append(str);
}

void String::resize(uint32_t new_capacity)
{
    backing.resize(new_capacity);
}

void String::clear()
{
    backing.clear();
}

uint32_t String::getLength() const
{
    return backing.getLength();
}

uint32_t String::getCapacity() const
{
    return backing.getCapacity();
}

const char* String::constStr() const
{
    uint32_t len = getLength();
    char* copy_buf = new char[len+1];
    memory::memSet((char)0, copy_buf, len+1);
    for (uint32_t i = 0; i < len; i++) copy_buf[i] = (*this)[i];
    return copy_buf;
}

int32_t String::find(char c, uint32_t start) const
{
    if (start >= backing.getLength()) return -1;
    for (uint32_t i = start; i < backing.getLength(); i++)
        if (backing[i] == c) return i;
    
    return -1;
}

String String::substring(uint32_t start, uint32_t end) const
{
    if (end <= start) { return String(); }
    if (start >= backing.getLength()) { return String(); }
    String substr((end < backing.getLength() ? end : backing.getLength()) - start);
    for (uint32_t i = start; i < end && i < backing.getLength(); i++)
        substr.append((*this)[i]);
    return substr;
}

String::~String()
{
    com_1 << "destructor" << stream::endl;
}

uint32_t nov::findNextByte(char* addr, char target)
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

String nov::intToString(const uint32_t i, const uint8_t base, const uint8_t padding)
{
    uint16_t size = padding > 33 ? padding : 33;
    char* buffer = new char[size];
    memory::memSet((char)0, buffer, size);
    intToString(i, base, buffer, padding);
    String str(buffer);
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

int32_t nov::stringToInt(const String& str, uint8_t base)
{
    return stringToInt(str.constStr(), base);
}

float nov::stringToFloat(const String& str)
{
    return stringToFloat(str.constStr());
}

String nov::floatToString(const float f, const uint8_t dps)
{
    uint16_t size = 64 + dps + 3;
    char* buffer = new char[size];
    memory::memSet((char)0, buffer, size);
    floatToString(f, buffer, dps);
    String str(buffer);
    delete[] buffer;
    return str;
}

stream::Stream& nov::operator<<(stream::Stream& stream, const String& s)
{
    for (uint32_t i = 0; i < s.getLength(); i++)
    {
        stream << s[i];
    }
    return stream;
}
