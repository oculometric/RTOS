#include <memory.h>

#include <serial.h>
#include <panic.h>

void* operator new(uint32_t size)
{
    return nov::memory::mAlloc(size);
}

void* operator new[](uint32_t size)
{
    return nov::memory::mAlloc(size);
}

void operator delete(void* ptr)
{
    nov::memory::mFree(ptr);
}

void operator delete[](void* ptr)
{
    nov::memory::mFree(ptr);
}

void operator delete(void* ptr, uint32_t size)
{
    nov::memory::mFree(ptr);
}

void operator delete[](void* ptr, uint32_t size)
{
    nov::memory::mFree(ptr);
}



namespace nov
{
namespace memory
{

/**
 * holds the start of the conceptual linked-list for memory management.
 * 
 * @attention ABSOLUTELY NEVER EVER EVER MODIFY THIS OUTSIDE THE MEMORY INITIALISER
 * 
 * **/
static MemoryFrame* head_frame = 0x0;

void mInit(void* block_start, uint32_t size)
{
    if (size < sizeof(MemoryFrame) || block_start == 0x0) return;

    // place a memory frame at the start of the provided block
    head_frame = (MemoryFrame*)block_start;
    head_frame->is_free = true;
    head_frame->signature = 0x4a6b;
    head_frame->signature_end = 0x79;
    
    // point it to an ending frame
    head_frame->next = (MemoryFrame*)(((uint32_t)block_start+size)-sizeof(MemoryFrame));

    // initialise the end frame to be a blank (size-zero) frame
    head_frame->next->next = 0x0;
    head_frame->next->is_free = false;

    // initialise the memory information struct
    memory_information.head = head_frame;
    memory_information.size = size;
}

void* mAlloc(uint32_t size)
{
    // keep track of the current and next blocks
    MemoryFrame* current_block = head_frame;
    MemoryFrame* next_block;
    uint32_t block_size;
    // calculate the size of the block of memory we actually want. add some padding and also align
    // to a 4-byte boundary (hence left and right shifting)
    uint32_t search_size = ((size+sizeof(MemoryFrame)+sizeof(MemoryFrame)+8) >> 2) << 2;

    while (current_block != 0x0)
    {
        // if this block isnt free, skip it
        if (!current_block->is_free) { current_block = current_block->next; continue; }
        next_block = current_block->next;
        // if the next block is NULL, then current_block is actually the blank end frame
        if (next_block == 0x0) return 0x0;

        // find the block size (i.e. the distance between the current frame and the next)
        block_size = (uint32_t)next_block-(uint32_t)current_block;
        if (block_size >= search_size)
        {
            // we've found a free block big enough to hold the new memory allocation and the necessarry memory frame
            // make a new memory frame delineating the end of this newly allocated block
            MemoryFrame* new_block = (MemoryFrame*)((uint32_t)(current_block+1)+size);
            new_block->is_free = true;
            new_block->signature = 0x4a6b;
            new_block->signature_end = 0x79;
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
    com_1 << size << stream::endl;
    panic("unable to allocate memory!!");
    return 0x0;
}

void mFree(void* ptr)
{
    // step back to the metadata of the block
    MemoryFrame* block = (MemoryFrame*)((uint32_t)ptr-sizeof(MemoryFrame));
    // someone tried to free an invalid block, or the block metadata has been corrupted (oh no)
    if (block->signature != 0x4a6b || block->signature_end != 0x79) { com_1 << "memory free error: " << stream::Mode::HEX << (uint32_t)ptr << stream::endl << (uint32_t)block->signature << stream::endl; panic("memory free error"); }
    // if this block is already free, um, cry
    if (block->is_free) return;
    // mark as free
    block->is_free = true;

    // check next block to see if it is free
    if (block->next == 0x0) return;
    if (!block->next->is_free) return;
    block->next = block->next->next;

    // TODO: add a last pointer? so blocks can be merged backwards
}

void mConsolidate()
{
    // keep track of the current and next blocks
    MemoryFrame* current_block = head_frame;
    MemoryFrame* next_block;

    while (current_block != 0x0)
    {
        next_block = current_block->next;
        if (next_block == 0x0) return;
        if (!current_block->is_free) { current_block = next_block; continue; }

        if (next_block->is_free) current_block->next = next_block->next;
        else current_block = next_block;
    }
}

void mView()
{
    MemoryFrame* current_block = head_frame;
    MemoryFrame* next_block;
    uint32_t block_size;

    com_1 << "=== MMAP START ===" << stream::endl;
    while (current_block != 0x0)
    {
        next_block = current_block->next;
        if (next_block == 0x0) 
        {
            com_1 << "end block found at " << stream::Mode::HEX << (uint32_t)current_block << stream::endl;
            com_1 << "=== MMAP END ===" << stream::endl;
            com_1.flush();
            return;
        }

        block_size = (uint32_t)next_block-(uint32_t)current_block;
        com_1 << "block at     " << stream::Mode::HEX << ((uint32_t)current_block) + sizeof(MemoryFrame) << " (actually " << (uint32_t)current_block << ")"  << stream::endl;
        com_1 << "   size      " << stream::Mode::DEC << block_size-sizeof(MemoryFrame) << '/' << stream::Mode::HEX << (block_size-sizeof(MemoryFrame)) << "(actually " << block_size << ")" << stream::endl;
        com_1 << "   is free?  " << stream::Mode::DEC << current_block->is_free << stream::endl;
    
        current_block = next_block;
    }
    com_1 << "=== MMAP END ===" << stream::endl;
    com_1.flush();
}

MemoryInformation memory_information;

}
}