#pragma once

#include <stdint.h>
#include <serial.h>

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
 * represents a block of heap memory which may be either free or occupied.
 * 
 * the size of a block can be calculated from its distance between it and
 * its `next` pointer. if `next` is `NULL` then the block should be considered
 * as being size zero.
 * 
 * `is_free` represents whether or not the block is empty and able to be allocated within
 * **/
#pragma pack (1)
struct nov_memory_frame
{
    nov_memory_frame* next = 0x0;
    bool is_free = false;
};
#pragma pack (0)

/**
 * holds the start of the conceptual linked-list for memory management.
 * 
 * @attention ABSOLUTELY NEVER EVER EVER MODIFY THIS OUTSIDE THE MEMORY INITIALISER
 * 
 * **/
static nov_memory_frame* head_frame = 0x0;

/**
 * configure the memory manager by initialising the head frame to cover the memory
 * area specified. do not call this more than once unless you want to totally
 * corrupt the memory map.
 * 
 * @param block_start pointer to the start of the available block
 * @param size size of the available block
 * **/
inline void minit(void* block_start, uint32_t size)
{
    if (size < sizeof(nov_memory_frame) || block_start == 0x0) return;

    // place a memory frame at the start of the provided block
    head_frame = (nov_memory_frame*)block_start;
    head_frame->is_free = true;
    // point it to an ending frame
    head_frame->next = (nov_memory_frame*)(((uint32_t)block_start+size)-sizeof(nov_memory_frame));

    // initialise the end frame to be a blank (size-zero) frame
    head_frame->next->next = 0x0;
    head_frame->next->is_free = false;
}

/**
 * allocate some memory of a specified size. guaranteed to fail
 * if `init_memory_manager` hasn't been called
 * 
 * @param size size of memory to be allocated, in bytes
 * @return pointer to the newly allocated memory, if allocation was successful, or NULL if it failed
 * **/
inline void* malloc(uint32_t size)
{
    // keep track of the current and next blocks
    nov_memory_frame* current_block = head_frame;
    nov_memory_frame* next_block;
    uint32_t block_size;

    while (current_block != 0x0)
    {
        // if this block isnt free, skip it
        if (!current_block->is_free) { current_block = current_block->next; continue; }
        next_block = current_block->next;
        // if the next block is NULL, then current_block is actually the blank end frame
        if (next_block == 0x0) return 0x0;

        // find the block size (i.e. the distance between the current frame and the next)
        block_size = (uint32_t)next_block-(uint32_t)current_block;
        if (block_size > size+sizeof(nov_memory_frame))
        {
            // we've found a free block big enough to hold the new memory allocation and the necessarry memory frame
            // make a new memory frame delineating the end of this newly allocated block
            nov_memory_frame* new_block = (nov_memory_frame*)((uint32_t)(current_block+1)+size);
            new_block->is_free = true;
            // insert into linked list
            new_block->next = next_block;
            current_block->next = new_block;
            // mark the current block as being occupied
            current_block->is_free = false;
            // return the address of the allocated block
            return (void*)(current_block+1);
        }
        // the block wasn't big enough, so lets move onto the next
        current_block = next_block;
    }

    return 0x0;
}

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
inline void mfree(void* ptr)
{
    // step back to the metadata of the block
    nov_memory_frame* block = (nov_memory_frame*)((uint32_t)ptr-sizeof(nov_memory_frame));
    // if this block is already free, um, cry
    if (block->is_free) return;
    // mark as free
    block->is_free = true;

    // check next block to see if it is free
    if (block->next == 0x0) return;
    if (!block->next->is_free) return;
    block->next = block->next->next;

    // TODO: possibly add a checksum
    // TODO: add a last pointer? so blocks can be merged backwards
}

/**
 * consolidate the memory map to merge adjacent free blocks.
 * 
 * **/
inline void mconsolidate()
{
    // keep track of the current and next blocks
    nov_memory_frame* current_block = head_frame;
    nov_memory_frame* next_block;

    while (current_block != 0x0)
    {
        next_block = current_block->next;
        if (next_block == 0x0) return;
        if (!current_block->is_free) { current_block = next_block; continue; }

        if (next_block->is_free) current_block->next = next_block->next;
        else current_block = next_block;
    }
}

/**
 * send a summary of the memory map to the serial console
 * 
 * **/
inline void mview()
{
    nov_memory_frame* current_block = head_frame;
    nov_memory_frame* next_block;
    uint32_t block_size;

    serial_println((char*)"=== MMAP START ===", COM1);
    while (current_block != 0x0)
    {
        next_block = current_block->next;
        if (next_block == 0x0) { serial_print((char*)"end block found at ", COM1); serial_println_hex((uint32_t)current_block, COM1); serial_println((char*)"=== MMAP END ===", COM1); return; }

        block_size = (uint32_t)next_block-(uint32_t)current_block;
        serial_print((char*)"block at     ", COM1); serial_println_hex((uint32_t)current_block, COM1);
        serial_print((char*)"   size w/h  ", COM1); serial_print_dec(block_size, COM1); serial_print((char*)"/", COM1); serial_println_hex(block_size, COM1);
        serial_print((char*)"   size wo/h ", COM1); serial_print_dec(block_size-sizeof(nov_memory_frame), COM1); serial_print((char*)"/", COM1); serial_println_hex(block_size-sizeof(nov_memory_frame), COM1);
        serial_print((char*)"   next      ", COM1); serial_println_hex((uint32_t)current_block->next, COM1);
        serial_print((char*)"   is free?  ", COM1); serial_println_dec(current_block->is_free, COM1);
    
        current_block = next_block;
    }
    serial_println((char*)"=== MMAP END ===", COM1);
}

}
}