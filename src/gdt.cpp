#include <gdt.h>

#include <exceptions.h>
#include <serial.h>

extern "C" void setProtectedGDT(uint16_t size, uint32_t offset);
extern "C" void reloadSegments(uint16_t code_segment_offset, uint16_t data_segment_offset);

namespace nov
{
namespace gdt
{

ProtectedGDTEntry createProtectedGDTEntry(uint32_t base, uint32_t limit, Privilege privilege, SegmentConfig config, Flags flags)
{
    ProtectedGDTEntry entry;

    if (limit > 0xFFFFF)
    {
        com_1 << "invalid limit for GDT entry!" << stream::endl;
        exception::handleGeneralProtectionFault();
    }

    if ((flags & 0b10) && !(config & 0b100))
    {
        com_1 << "64 bit code segments must have a code configuration" << stream::endl;
        exception::handleGeneralProtectionFault();
    }

    if (privilege > 3)
    {
        com_1 << "invalid privilege level" << stream::endl;
        exception::handleGeneralProtectionFault();
    }

    if (config > 0b111)
    {
        com_1 << "invalid segment config" << stream::endl;
        exception::handleGeneralProtectionFault();
    }

    if ((flags & 0b0001) || (flags > 0b1100))
    {
        com_1 << "invalid segment flags" << stream::endl;
        exception::handleGeneralProtectionFault();
    }
    
    entry.limit_low = limit & 0xFFFF;
    entry.base_low = base & 0xFFFF;
    entry.base_mid = (base >> 16) & 0xFF;
    entry.access = 0b10010001 | (privilege << 5) | (config << 1);
    entry.limit_and_flags = ((limit >> 16) & 0xF) | (flags << 4);
    entry.base_high = (base >> 24) & 0xFF;

    return entry;    
}

void loadGDT(const ProtectedGDTEntry* first_entry, uint16_t num_entries, uint16_t code_segment_index, uint16_t data_segment_index)
{
    if (num_entries > 8192)
    {
        com_1 << "unable to load GDT with more than 8192 entries" << stream::endl;
        exception::handleGeneralProtectionFault();
    }

    if (num_entries == 0)
    {
        com_1 << "unable to load GDT with zero entries" << stream::endl;
        exception::handleGeneralProtectionFault();
    }

    if (code_segment_index >= num_entries || data_segment_index >= num_entries)
    {
        com_1 << "invalid data/code segment" << stream::endl;
        exception::handleGeneralProtectionFault();
    }

    // TODO: handle non-zero current data segment base
    setProtectedGDT((num_entries * sizeof(ProtectedGDTEntry) - 1), (uint32_t)first_entry);
    reloadSegments(code_segment_index * sizeof(ProtectedGDTEntry), data_segment_index * sizeof(ProtectedGDTEntry));
}

}
}
