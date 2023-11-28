PREFIX		= /opt/cross/x86_64-elf/bin/x86_64-elf-
CC			= $(PREFIX)gcc
AS			= $(PREFIX)as
LK			= $(PREFIX)gcc
OC			= $(PREFIX)objcopy

GNU_EFI_INC_DIR = /usr/include/efi
GNU_EFI_DIR = ../gnu-efi

CC_FLAGS 	= -ffreestanding -fpic -fno-stack-protector -fshort-wchar -mgeneral-regs-only -Wall -Wextra -Wpedantic -mno-red-zone -mabi=ms

LK_DOC		= $(GNU_EFI_DIR)/gnuefi/elf_x86_64_efi.lds
LK_DOC		= linker.ld
LK_FLAGS	= -nostdlib -shared -Wl,-T,$(LK_DOC) -Wl,-Bsymbolic -Wl,-znocombreloc
LIB_SEARCH	= -L$(GNU_EFI_DIR)/x86_64/lib -L$(GNU_EFI_DIR)/x86_64/gnuefi

CC_DIR		= src/c
INC_DIR		= src/h
AS_DIR		= src/s
OBJ_DIR		= bin/o

CC_FILES_IN	:= $(wildcard $(CC_DIR)/*.c)
AS_FILES_IN	:= $(wildcard $(AS_DIR)/*.s)

CC_FILES_OUT=$(patsubst $(CC_DIR)/%.c, $(OBJ_DIR)/%.o, $(CC_FILES_IN))
AS_FILES_OUT=$(patsubst $(AS_DIR)/%.s, $(OBJ_DIR)/%.o, $(AS_FILES_IN))

BIN 		= rtos
BIN_OUT		= bin/$(BIN)
EFI_OUT		= bin/BOOTX64.EFI

ISO			= $(BIN_OUT).iso
FAT_IMG		= $(BIN_OUT).img

OVMF_DIR	= OVMF

QEMU_DRIVER = $(OVMF_DIR)/OVMF-pure-efi.fd
OVMF_CODE	= $(OVMF_DIR)/OVMF_CODE-pure-efi.fd
OVMF_VARS	= $(OVMF_DIR)/OVMF_VARS-pure-efi.fd

obj_dir:
	@[ -d $(OBJ_DIR) ] || mkdir $(OBJ_DIR) -p

$(OBJ_DIR)/%.o: $(CC_DIR)/%.c obj_dir
	@echo Compiling $<...
	@$(CC) $(CC_FLAGS) -I$(INC_DIR) -I$(GNU_EFI_INC_DIR) -o $@ -c $<

$(OBJ_DIR)/%.o: $(AS_DIR)/%.s obj_dir
	@echo Compiling $<...
	@$(AS) $< -o $@

build: $(CC_FILES_OUT) $(AS_FILES_OUT) obj_dir
	@echo Linking...
	@$(LK) $(LK_FLAGS) $(LIB_SEARCH) -o $(BIN_OUT).elf $(AS_FILES_OUT) $(CC_FILES_OUT) -lgnuefi -lefi -lgcc
	@$(OC) -I elf64-x86-64 -O efi-app-x86_64 $(BIN_OUT).elf $(EFI_OUT)
	@echo Link success.


fat: build
	@echo Building FAT image...
	@dd if=/dev/zero of=$(FAT_IMG) bs=1k count=1440
	@mformat -i $(FAT_IMG) -f 1440 ::
	@mmd -i $(FAT_IMG) ::/EFI
	@mmd -i $(FAT_IMG) ::/EFI/BOOT
	@mcopy -i $(FAT_IMG) $(EFI_OUT) ::/EFI/BOOT
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
	@rm -f $(BIN).bin
	@echo Cleaned.

emulate: fat
	#qemu-system-i386 -bios $(OVMF_FLASH) -hda $(BIN).bin -serial file:serial.log

	#qemu-system-i386 -machine q35 -m 256 -smp 2 -net none \
	#-global driver=$(QEMU_DRIVER),property=secure,value=on \
	#-drive if=pflash,format=raw,unit=0,file=$(OVMF_CODE),readonly=on \
    #-drive if=pflash,format=raw,unit=1,file=$(OVMF_VARS) \
    #-drive if=ide,format=raw,file=$(BIN).img

	#qemu-system-x86_64 -machine q35 -m 256 -smp 2 -net none \
    #-global driver=$(QEMU_DRIVER),property=secure,value=on \
    #-drive if=pflash,format=raw,unit=0,file=$(OVMF_CODE),readonly=on \
    #-drive if=pflash,format=raw,unit=1,file=$(OVMF_VARS) \
    #-drive if=ide,format=raw,file=$(BIN).img

	qemu-system-x86_64 -pflash $(QEMU_DRIVER) -drive if=ide,format=raw,file=$(BIN).img -net none -serial file:serial.log