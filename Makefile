PREFIX		= /usr/bin/
CC			= $(PREFIX)g++
CC_FLAGS 	= -fpic -ffreestanding -fno-stack-protector -fno-stack-check -fshort-wchar -mno-red-zone -maccumulate-outgoing-args
AS			= $(PREFIX)as
LK			= $(PREFIX)ld

GNU_EFI_INC_DIR = /usr/include/efi
GNU_EFI_DIR = static_gnu_efi
GNU_RELOCATOR = $(GNU_EFI_DIR)/x86_64/gnuefi/crt0-efi-x86_64.o

OVMF_DIR	= ovmf
OVMF_FLASH	= ovmf/RELEASEX64_OVMF.fd

CC_DIR		= src/c
INC_DIR		= src/h
AS_DIR		= src/s
OBJ_DIR		= bin/o

CC_FILES_IN	:= $(wildcard $(CC_DIR)/*.cc)
AS_FILES_IN	:= $(wildcard $(AS_DIR)/*.s)

CC_FILES_OUT=$(patsubst $(CC_DIR)/%.cc, $(OBJ_DIR)/%.o, $(CC_FILES_IN))
AS_FILES_OUT=$(patsubst $(AS_DIR)/%.s, $(OBJ_DIR)/%.o, $(AS_FILES_IN))

LD			= $(GNU_EFI_DIR)/gnuefi/elf_x86_64_efi.lds
LD_LIB		= -shared -Bsymbolic -L$(GNU_EFI_DIR)/x86_64/lib -L$(GNU_EFI_DIR)/x86_64/gnuefi

BIN 		= rtos
BIN_OUT		= bin/$(BIN)

ISO			= $(BIN_OUT).iso
FAT_IMG		= $(BIN_OUT).img

obj_dir:
	@[ -d $(OBJ_DIR) ] || mkdir $(OBJ_DIR) -p

$(OBJ_DIR)/%.o: $(CC_DIR)/%.cc obj_dir
	@echo Compiling $<...
	@$(CC) -c $< -o $@ $(CC_FLAGS) -I$(INC_DIR) -I$(GNU_EFI_INC_DIR)

$(OBJ_DIR)/%.o: $(AS_DIR)/%.s obj_dir
	@echo Compiling $<...
	@$(AS) $< -o $@

build: $(CC_FILES_OUT) $(AS_FILES_OUT) obj_dir
	@echo Linking...
	@$(LK) $(LD_LIB) -T$(LD) $(GNU_RELOCATOR) $(AS_FILES_OUT) $(CC_FILES_OUT) -I$(INC_DIR) -o $(BIN_OUT).so -lgnuefi -lefi
	@echo Link success.

	@objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym  -j .rel -j .rela -j .rel.* -j .rela.* -j .reloc --target efi-app-x86_64 --subsystem=10 $(BIN_OUT).so $(BIN_OUT).efi

fat: build
	@echo Building FAT image...
	@dd if=/dev/zero of=$(FAT_IMG) bs=1k count=1440
	@mformat -i $(FAT_IMG) -f 1440 ::
	@mmd -i $(FAT_IMG) ::/EFI
	@mmd -i $(FAT_IMG) ::/EFI/BOOT
	@cp $(BIN_OUT).efi bin/BOOTX64.EFI
	@mcopy -i $(FAT_IMG) bin/BOOTX64.EFI ::/EFI/BOOT
	@cp $(FAT_IMG) $(BIN).img
	@echo Done.

harddrive: fat
	@echo Making hard drive BIN image...
	@mkgpt -o bin/$(BIN).bin --image-size 4096 --part $(FAT_IMG) --type system
	@cp bin/$(BIN).bin $(BIN).bin
	@echo Done.

clean:
	@rm -fr bin
	@rm -f $(BIN).img
	@echo Cleaned.

emulate: harddrive
	@#qemu-system-i386 -kernel $(BIN_OUT) -serial file:serial.log -machine type=pc-i440fx-3.1 --no-shutdown --no-reboot
	sudo qemu-system-x86_64 -bios $(OVMF_FLASH) -kernel bin/rtos.efi -serial file:serial.log