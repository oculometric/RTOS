#pragma once

#include <stdint.h>

namespace nov
{
namespace acpi
{

#pragma pack(1)
struct RootSystemDescriptorPointer
{
    char signature[8];
    uint8_t checksum;
    char oem_id[6];
    uint8_t revision;
    uint32_t rsdt_address;
};

struct ExtendedSystemDescriptorPointer
{
    char signature[8];
    uint8_t checksum;
    char oem_id[6];
    uint8_t revision;
    uint32_t rsdt_address;

    uint32_t length;
    uint64_t xsdt_address;
    uint8_t extended_checksum;
    uint8_t reserved[3];
};
#pragma pack(0)

void initACPI();

}
}