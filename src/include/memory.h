#pragma once

#include <stdint.h>

namespace nov
{
namespace memory
{

template <typename T>
inline void memCpy(T* src, T* dest, uint32_t length)
{
    uint32_t offset = 0;
    while (offset < length)
    {
        dest[offset] = src[offset];
        offset++;
    }
}

template <typename T>
inline void memSet(T value, T* dest, uint32_t length)
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
void mInit(void* block_start, uint32_t size);

/**
 * allocate some memory of a specified size. guaranteed to fail
 * if `init_memory_manager` hasn't been called
 * 
 * @param size size of memory to be allocated, in bytes
 * @return pointer to the newly allocated memory, if allocation was successful, or NULL if it failed
 * **/
void* mAlloc(uint32_t size);

/**
 * free some memory starting at a given pointer. attempts to merge the newly freed block
 * with the following block if the following block is already free.
 * 
 * @param ptr pointer to be freed
 * @attention kind of tries to check if the pointer is valid but this is pretty likely
 * to fail. please don't call this with an invalid pointer (i.e. not something generated
 * by mAlloc) otherwise bad things will almost certainly happen
 * 
 * **/
void mFree(void* ptr);

/**
 * consolidate the memory map to merge adjacent free blocks.
 * 
 * **/
void mConsolidate();

/**
 * send a summary of the memory map to the serial console
 * 
 * **/
void mView();

/**
 * represents a block of heap memory which may be either free or occupied.
 * 
 * the size of a block can be calculated from its distance between it and
 * its `next` pointer. if `next` is `NULL` then the block should be considered
 * as being size zero.
 * 
 * `is_free` represents whether or not the block is empty and able to be allocated within
 * **/
#pragma pack (1)
struct MemoryFrame
{
    MemoryFrame* next = 0x0;
    uint16_t signature = 0x4a6b;
    uint8_t signature_end = 0x79;
    bool is_free = false;
};
#pragma pack (0)

struct MemoryInformation
{
    void* head = 0x0;
    uint32_t size = 0;
};

extern MemoryInformation memory_information;

}
}