#pragma once

#include <stdint.h>

inline void bytecpy(uint8_t* src, uint8_t* dest, uint32_t bytes)
{
    uint32_t offset = 0;
    while (offset < bytes)
    {
        dest[offset] = src[offset];
        offset++;
    }
}

inline void wordcpy(uint16_t* src, uint16_t* dest, uint32_t words)
{
    uint32_t offset = 0;
    while (offset < words)
    {
        dest[offset] = src[offset];
        offset++;
    }
}

inline void dwrdcpy(uint32_t* src, uint32_t* dest, uint32_t dwrds)
{
    uint32_t offset = 0;
    while (offset < dwrds)
    {
        dest[offset] = src[offset];
        offset++;
    }
}

inline void byteset(uint8_t value, uint8_t* dest, uint32_t bytes)
{
    uint32_t offset = 0;
    while (offset < bytes)
    {
        dest[offset] = value;
        offset++;
    }
}

inline void wordset(uint16_t value, uint16_t* dest, uint32_t words)
{
    uint32_t offset = 0;
    while (offset < words)
    {
        dest[offset] = value;
        offset++;
    }
}

inline void dwrdset(uint32_t value, uint8_t* dest, uint32_t dwrds)
{
    uint32_t offset = 0;
    while (offset < dwrds)
    {
        dest[offset] = value;
        offset++;
    }
}

template <typename T>
inline void memcpy(T* src, T* dest, uint32_t length)
{
    uint32_t offset = 0;
    while (offset < length)
    {
        dest[offset] = src[offset];
        offset++;
    }
}

template <typename T>
inline void memset(T value, T* dest, uint32_t length)
{
    uint32_t offset = 0;
    while (offset < length)
    {
        dest[offset] = value;
        offset++;
    }
}