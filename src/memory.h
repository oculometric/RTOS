#pragma once

#include <stdint.h>

inline void memcpy(void* src, void* dest, uint32_t length)
{
    uint32_t offset = 0;
    while (offset < length)
    {
        ((uint8_t*)dest)[offset] = ((uint8_t*)src)[offset];
        offset++;
    }
}