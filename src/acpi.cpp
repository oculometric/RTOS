#include <acpi.h>
#include <string.h>
#include <serial.h>

namespace nov
{
namespace acpi
{

static RootSystemDescriptorPointer rsdp;
static ExtendedSystemDescriptorPointer xsdp;
bool use_extended_sdp = false;

// FIXME: can't do acpi right now because the OS is overwriting it

ExtendedSystemDescriptorPointer findACPIRootDescriptor()
{
    ExtendedSystemDescriptorPointer* test = (ExtendedSystemDescriptorPointer*)0x80000;
    while (!strcmp(test->signature, (char*)"RSD PTR ", 8))
    {
        test = (ExtendedSystemDescriptorPointer*)((uint32_t)test + 16);
        if ((uint32_t)test > 0xFFFFF) return ExtendedSystemDescriptorPointer{ };
    }
    serial::com_1 << "ACPI system descriptor pointer located at " << stream::Mode::HEX << (uint32_t)test << stream::endl;
    return *test;
}

void initACPI()
{
    ExtendedSystemDescriptorPointer result = findACPIRootDescriptor();
    if (!strcmp(result.signature, (char*)"RSD PTR ", 8))
        panic("UH OH! no ACPI for you babyyy");

    serial::com_1 << "found ACPI root system descriptor pointer:" << stream::endl;
    serial::com_1 << "    signature          : " << result.signature << stream::endl;
    serial::com_1 << "    checksum           : " << stream::Mode::HEX << result.checksum << stream::endl;
    serial::com_1 << "    OEM ID             : " << result.oem_id << stream::endl;
    serial::com_1 << "    revision           : " << stream::Mode::DEC << result.revision << stream::endl;
    serial::com_1 << "    RSDT address       : " << stream::Mode::HEX << result.rsdt_address << stream::endl;
    
    if (result.revision == 0)
    {
        uint8_t checksum = 0x0;

        uint8_t* addr = (uint8_t*)(&result);
        for (uint32_t i = 0; i < sizeof(ExtendedSystemDescriptorPointer); i++)
            checksum += addr[i];

        serial::com_1 << "    checksum           : " << (checksum == 0 ? "valid" : "invalid") << " (" << checksum << ")" << stream::endl;
        if (checksum != 0) panic("UH OH! ACPI RSDP checksum is invalid :(");

        use_extended_sdp = false;
        rsdp = *(RootSystemDescriptorPointer*)(&result);
    }
    else if (result.revision == 2)
    {
        // TODO: support for XSDP
        panic("XSDP is unsupported");
    }
    
    panic("UH OH! fucked up ACPI system descriptor pointer revision");
}

}
}