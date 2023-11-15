#pragma once

#include <stddef.h>
#include <stdint.h>

#include "math.h"

template <typename T>
class basic_string
{
private:
    T* _str = NULL;
    size_t _length = 0;
    size_t _capacity = 0;
public:
    size_t length();
    size_t capacity();
    void push_back(T);
    void append(T*);
    void append(basic_string<T>);
    void resize(size_t);
    T* pointer();

    basic_string();
    basic_string(size_t);
    basic_string(T*);

    T& operator[](size_t s);
};

typedef basic_string<uint8_t> string;
#define CHARS_FOR_ITOS "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"

string int_to_string(int, uint8_t);

template <typename T>
inline size_t basic_string<T>::length()
{
    return length;
}

template <typename T>
inline size_t basic_string<T>::capacity()
{
    return capacity;
}

template <typename T>
inline void basic_string<T>::push_back(T c)
{
    if (length == capacity)
    {
        // TODO: reallocate and copy across
    }
    _str[length] = c;
    length++;
}

template <typename T>
inline void basic_string<T>::append(T* oprnd)
{
    size_t len = strlen(oprnd);
    if (length+len >= capacity)
    {
        // TODO: reallocate and copy across
    }
    for (int i = 0; i < len; i++)
    {
        _str[length+i] = oprnd[i];
    }
}

template <typename T>
inline T* basic_string<T>::pointer()
{
    return _str;
}