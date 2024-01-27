#include <stdint.h>

void* find_pmid_block(void* start, uint32_t length)
{
    uint8_t* ptr = (uint8_t*)start;

    uint32_t offset = 0;
    while (offset < length)
    {
        if (ptr[offset] == 'P' && ptr[offset+1] == 'M' && ptr[offset+2] == 'I' && ptr[offset+3] == 'D')
        {
            return (void*)(ptr+offset);
        }
        offset++;
    }
    return (void*)0x0;
}