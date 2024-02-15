#pragma once

#include <stdint.h>
#include <serial.h>
#include <panic.h>

namespace nov
{
namespace memory
{

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

/**
 * configure the memory manager by initialising the head frame to cover the memory
 * area specified. do not call this more than once unless you want to totally
 * corrupt the memory map.
 * 
 * @param block_start pointer to the start of the available block
 * @param size size of the available block
 * **/
void minit(void* block_start, uint32_t size);

/**
 * allocate some memory of a specified size. guaranteed to fail
 * if `init_memory_manager` hasn't been called
 * 
 * @param size size of memory to be allocated, in bytes
 * @return pointer to the newly allocated memory, if allocation was successful, or NULL if it failed
 * **/
void* malloc(uint32_t size);

/**
 * free some memory starting at a given pointer. attempts to merge the newly freed block
 * with the following block if the following block is already free.
 * 
 * @param ptr pointer to be freed
 * @attention kind of tries to check if the pointer is valid but this is pretty likely
 * to fail. please don't call this with an invalid pointer (i.e. not something generated
 * by malloc) otherwise bad things will almost certainly happen
 * 
 * **/
void mfree(void* ptr);

/**
 * consolidate the memory map to merge adjacent free blocks.
 * 
 * **/
void mconsolidate();

/**
 * send a summary of the memory map to the serial console
 * 
 * **/
void mview();

}
}