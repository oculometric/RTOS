#pragma once

#include <stddef.h>

template <typename T>
class string
{
private:
    T* _str;
    size_t _length;
    size_t _capacity;
public:
    size_t length();
    size_t capacity();
    void push_back(T);
    void append(T*);
    void append(string);
    void resize(size_t);
    T* pointer();

    string();
    string(size_t);
    string(T*);

    T operator[](size_t s);
};