ARCH 			= x64
SUBSYSTEM		= 10

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
	#CROSS_COMPILE	= $(GCC_ARCH)-$(MINGW_HOST)-mingw32-
	CROSS_COMPILE	= 
	EP_PREFIX		=
	C_FLAGS			= -m64 -mno-red-zone
	LD_FLAGS		= -Wl,-dll #-Wl,--subsystem,$(SUBSYSTEM)
endif

CC			:= $(CROSS_COMPILE)gcc
AS			:= $(CROSS_COMPILE)as
LD			:= $(CROSS_COMPILE)gcc
OC			:= $(CROSS_COMPILE)objcopy

CC_DIR		= src/arch/$(GCC_ARCH)/c
INC_DIR		= src/arch/$(GCC_ARCH)/h
AS_DIR		= src/arch/$(GCC_ARCH)/s
OBJ_DIR		= bin/arch/$(GCC_ARCH)/o

GNUEFI_DIR = ../gnu-efi

C_FLAGS		+= -fno-stack-protector -Wshadow -Wall -Wunused -Wextra -Wpedantic -Werror-implicit-function-declaration -ffreestanding
C_INC		= -I$(GNUEFI_DIR)/inc -I$(GNUEFI_DIR)/inc/$(GNUEFI_ARCH) -I$(GNUEFI_DIR)/inc/protocol
C_DEFS		= -DCONFIG_$(GNUEFI_ARCH) -D__MAKEWITH_GNUEFI -DGNU_EFI_USE_MS_ABI

LD_FLAGS	+= -s -Wl,-Bsymbolic -nostdlib -shared
LD_LIB		+= -L$(GNUEFI_DIR)/$(GNUEFI_ARCH)/lib -e $(EP_PREFIX)efi_main
LIBS		= -lefi -lgnuefi

QEMU		= qemu-system-$(QEMU_ARCH)

#LK_DOC		= $(GNU_EFI_DIR)/gnuefi/elf_x86_64_efi.lds
#LK_DOC		= linker.ld
#LK_FLAGS	= -nostdlib -shared,-T,$(LK_DOC) -Wl,-Bsymbolic -Wl,-znocombreloc
#LIB_SEARCH	= -L$(GNU_EFI_DIR)/x86_64/lib -L$(GNU_EFI_DIR)/x86_64/gnuefi

CC_FILES_IN		:= $(wildcard $(CC_DIR)/*.c)
AS_FILES_IN		:= $(wildcard $(AS_DIR)/*.s)

CC_FILES_OUT	= $(patsubst $(CC_DIR)/%.c, $(OBJ_DIR)/%.o, $(CC_FILES_IN))
AS_FILES_OUT	= $(patsubst $(AS_DIR)/%.s, $(OBJ_DIR)/%.o, $(AS_FILES_IN))

BIN 			= novos_$(ARCH)
BIN_OUT			= bin/$(BIN)
EFI_OUT			= bin/arch/$(GCC_ARCH)/BOOT$(ARCH).EFI

ISO				= $(BIN_OUT).iso
FAT_IMG			= $(BIN_OUT).img

OVMF_DIR		= OVMF
OVMF_FIRMWARE 	= $(OVMF_DIR)/OVMF-pure-efi.fd

all: $(EFI_OUT)

obj_dir:
	@[ -d $(OBJ_DIR) ] || mkdir $(OBJ_DIR) -p

$(OBJ_DIR)/%.o: $(CC_DIR)/%.c obj_dir
	@echo Compiling $<...
	@$(CC) $(C_FLAGS) $(C_INC) $(C_DEFS) -o $@ -c $<

$(EFI_OUT): $(CC_FILES_OUT) $(AS_FILES_OUT) obj_dir
	@echo Linking...
	@$(LD) $(LD_FLAGS) $(LD_LIB) -o $(BIN_OUT).elf $(AS_FILES_OUT) $(CC_FILES_OUT) $(LIBS)
	#@$(OC) -I elf64-x86-64 -O efi-app-x86_64 $(BIN_OUT).elf $(EFI_OUT)
	@mkdir -p bin/arch/$(GCC_ARCH)
	@$(OC) -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel* -j .rela* -j .reloc -j .eh_frame -O binary $(BIN_OUT).elf $(EFI_OUT)
	@echo Link success.

$(FAT_IMG): $(EFI_OUT)
	@echo Building FAT image...
	@dd if=/dev/zero of=$@ bs=1k count=1440
	@mformat -i $@ -f 1440 ::
	@mmd -i $@ ::/EFI
	@mmd -i $@ ::/EFI/BOOT
	@mcopy -i $@ $(EFI_OUT) ::/EFI/BOOT
	@cp $@ $(BIN).img

	@mkdir -p image/efi/boot
	@cp -f $(EFI_OUT) image/efi/boot/BOOT$(ARCH).EFI

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

emulate: $(FAT_IMG)
	$(QEMU) $(QEMU_FLAGS) -bios $(OVMF_FIRMWARE) -net none -hda fat:rw:image -serial file:serial.log