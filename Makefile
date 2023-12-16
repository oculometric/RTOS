ARCH 		= x86
SUBSYSTEM	= 10

ifeq ($(shell uname -m),x86_64)
  MINGW_HOST    = w64
else
  MINGW_HOST    = w32
endif

ifeq ($(ARCH),x64)
	GNUEFI_ARCH		= x86_64
	GCC_ARCH		= x86_64
	QEMU_ARCH		= x86_64
	FW_BASE			= OVMF
	CROSS_COMPILE	= $(GCC_ARCH)-$(MINGW_HOST)-mingw32-
	EP_PREFIX		=
	C_FLAGS			= -m64 -mno-red-zone
	LD_FLAGS		= -Wl,-dll -Wl,--subsystem,$(SUBSYSTEM)
endif

CC			:= $(CROSS_COMPILE)gcc
OBJCOPY		:= $(CROSS_COMPILE)objcopy
AS			:= $(CROSS_COMPILE)as
LK			:= $(CROSS_COMPILE)gcc
OC			:= $(CROSS_COMPILE)objcopy

CC_DIR		= src/arch/$(ARCH)/c
INC_DIR		= src/arch/$(ARCH)/h
AS_DIR		= src/arch/$(ARCH)/s
OBJ_DIR		= bin/arch/$(ARCH)/o

GNUEFI_DIR = ../gnu-efi

C_FLAGS		+= -fno-stack-protector -Wshadow -Wall -Wunused -Wextra -Wpedantic -Werror-implicit-function-declaration -ffreestanding
C_INC		= -I$(GNUEFI_DIR)/inc -I$(GNUEFI_DIR)/inc/$(GNUEFI_ARCH) -I$(GNUEFI_DIR)/inc/protocol
C_DEFS		= -DCONFIG_$(GNUEFI_ARCH) -D__MAKEWITH_GNUEFI -DGNU_EFI_USE_MS_ABI

LD_FLAGS	+= -s -Wl,-Bsymbolic -nostdlib -shared
LD_LIB		+= -L$(GNUEFI_DIR)/$(GNUEFI_ARCH)/lib -e $(EP_PREFIX)efi_main
LIBS		= -lefi $(CRT0_LIBS)



LK_DOC		= $(GNU_EFI_DIR)/gnuefi/elf_x86_64_efi.lds
LK_DOC		= linker.ld
LK_FLAGS	= -nostdlib -shared,-T,$(LK_DOC) -Wl,-Bsymbolic -Wl,-znocombreloc
LIB_SEARCH	= -L$(GNU_EFI_DIR)/x86_64/lib -L$(GNU_EFI_DIR)/x86_64/gnuefi

CC_FILES_IN	:= $(wildcard $(CC_DIR)/*.c)
AS_FILES_IN	:= $(wildcard $(AS_DIR)/*.s)

CC_FILES_OUT=$(patsubst $(CC_DIR)/%.c, $(OBJ_DIR)/%.o, $(CC_FILES_IN))
AS_FILES_OUT=$(patsubst $(AS_DIR)/%.s, $(OBJ_DIR)/%.o, $(AS_FILES_IN))

BIN 		= novos
BIN_OUT		= bin/$(BIN)
EFI_OUT		= bin/arch/$(ARCH)/BOOTX64.EFI

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
	@$(CC) $(C_FLAGS) $(C_INC) $(C_DEFS) -o $@ -c $<

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