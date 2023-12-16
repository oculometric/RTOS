#include <efi.h>
#include <efilib.h>

#include <serial.h>
 
EFI_STATUS
EFIAPI
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
	//while (1) {}
	InitializeLib(ImageHandle, SystemTable);
	AsciiPrint((const unsigned char*)"heyya\n");

	initialise_serial(COM1, 38400);
	serial_output(COM1, (uint8_t*)"heyyo!");
	while (1) {}
	return EFI_SUCCESS;
}