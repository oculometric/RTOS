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
    CODE_CONFORMING_READABLE = 0b111,       // code which can only be reached from privilege levels equal to or lower than it, readable
    CODE_CONFORMING_UNREADABLE = 0b110,     // code which can only be reached from privilege levels equal to or lower than it, non-readable
    CODE_NONCONFORMING_READABLE = 0b101,    // code which can only be reached from privilege levels equal to it, readable
    CODE_NONCONFORMING_UNREADABLE = 0b100,  // code which can only be reached from privilege levels equal to it, non-readable
    DATA_DOWNWARD_WRITEABLE = 0b011,        // data which grows downward (offset > limit), writeable
    DATA_DOWNWARD_READONLY = 0b010,         // data which grows downward (offset > limit), read-only
    DATA_UPWARD_WRITEABLE = 0b001,          // data which grows upward, writeable
    DATA_UPWARD_READNLY = 0b000             // data which grows upward, read-only
};

// defines possible flags for segments. long mode flags should only be used for code segments
enum Flags
{
    LONG_MODE_BYTE = 0b0010,    // 64-bit code only, with byte granularity
    LONG_MODE_PAGE = 0b1010,    // 64-bit code only, with page granularity
    PROT_MODE_BYTE = 0b0100,    // 32-bit, with byte granularity
    PROT_MODE_PAGE = 0b1100,    // 32-bit, with page granularity
    REAL_MODE_BYTE = 0b0000,    // 16-bit, with byte granularity
    REAL_MODE_PAGE = 0b1000     // 16-bit, with page granularity
};

enum SystemSegmentConfig
{
    TSS_16_AVAIL = 0x1,     // available 16-bit task segment
    LDT = 0x2,              // LDT segment
    TSS_16_BUSY = 0x3,      // busy 16-bit task segment
    TSS_32_64_AVAIL = 0x9,  // available non-16-bit task segment
    TSS_32_64_BUSY = 0xB    // busy non-16-bit task segment
};

// TODO: implement task-segment cosntructor

/**
 * generates a Descriptor object for the GDT with the specified properties. this function is the only
 * correct way to initialise a Descriptor
 * 
 * @param base offset of the start of the segment in memory
 * @param limit furthest accessible address from the start of the segment (i.e. size of segment - 1)
 * @param config segment type and properties
 * @param flags segment format flags
 * 
 * @returns a valid protected-mode GDT entry
 * 
 * **/
ProtectedGDTEntry createProtectedGDTEntry(uint32_t base, uint32_t limit, Privilege privilege, SegmentConfig config, Flags flags);

/**
 * loads an array of protected mode GDT entries (hopefully representing a functional, valid GDT if
 * you did it properly) into the GDT register. performs some error checking. the first entry should
 * always be a null entry, followed by the actual meaningful entries
 * 
 * @param first_entry pointer to the first entry in the GDT (the null entry)
 * @param num_entries total number of entries in the array, including the null entry
 * @param code_segment_index index of the segment in the new GDT that should be jumped to reload CS (Code Segment)
 * @param data_segment_index index of the segment in the new GDT that should be used to reload DS, ES, FS, GS, SS
 * 
 * **/
void loadGDT(const ProtectedGDTEntry* first_entry, uint16_t num_entries, uint16_t code_segment_index, uint16_t data_segment_index);

}
}
