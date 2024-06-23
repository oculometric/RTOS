#pragma once

#include <stdint.h>
#include <system_common.h>

namespace nov
{
namespace gdt
{

#pragma pack(1)
// struct representing a single entry in the GDT. NEVER initialise this directly, use the `createProtectedGDTEntry` function instead
struct ProtectedGDTEntry
{
    uint16_t limit_low = 0;
    uint16_t base_low = 0;
    uint8_t base_mid = 0;
    uint8_t access = 0;
    uint8_t limit_and_flags = 0;
    uint8_t base_high = 0;
};
#pragma pack(0)

// defines configuration for segments. these values correspond to executable, direction/conforming, readable/writeable bits
enum SegmentConfig
{
    CODE_CONFORMING_READABLE = 0b111,
    CODE_CONFORMING_UNREADABLE = 0b110,
    CODE_NONCONFORMING_READABLE = 0b101,
    CODE_NONCONFORMING_UNREADABLE = 0b100,
    DATA_DOWNWARD_WRITEABLE = 0b011,
    DATA_DOWNWARD_READONLY = 0b010,
    DATA_UPWARD_WRITEABLE = 0b001,
    DATA_UPWARD_READNLY = 0b000
};

// defines possible flags for segments. long mode flags should only be used for code segments
enum Flags
{
    LONG_MODE_BYTE = 0b0010,
    LONG_MODE_PAGE = 0b1010,
    PROT_MODE_BYTE = 0b0100,
    PROT_MODE_PAGE = 0b1100,
    REAL_MODE_BYTE = 0b0000,
    REAL_MODE_PAGE = 0b1000
};

enum SystemSegmentConfig
{
    TSS_16_AVAIL = 0x1,
    LDT = 0x2,
    TSS_16_BUSY = 0x3,
    TSS_32_64_AVAIL = 0x9,
    TSS_32_64_BUSY = 0xB
};

// TODO: give more explanation here for segment config and flags
// TODO: implement task-segment cosntructor

/**
 * generates a Descriptor object for the GDT with the specified properties. this function is the only
 * correct way to initialise a Descriptor.
 * 
 * @param base offset of the start of the segment in memory
 * @param limit furthest accessible address from the start of the segment (i.e. size of segment - 1)
 * @param config segment type and properties
 * @param flags segment format flags
 * 
 * **/
ProtectedGDTEntry createProtectedGDTEntry(uint32_t base, uint32_t limit, Privilege privilege, SegmentConfig config, Flags flags);
void loadGDT(const ProtectedGDTEntry* first_entry, uint16_t num_entries, uint16_t code_segment_index, uint16_t data_segment_index);

}
}
