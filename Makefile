TOOLPATH 	:= $(HOME)/opt/cross/bin/
PREFIX 		:= $(TOOLPATH)i386-elf-

AS			:= nasm
CC			:= $(PREFIX)g++
LD			:= $(PREFIX)ld

AS_FLAGS		:= -f elf
CC_FLAGS		:= -ffreestanding -m32 -g -masm=intel -Wall -Wextra -Wpedantic
LD_FLAGS		:= -T linker.ld

BIN				= bin
SRC				= src

BOOT_OUT		= $(BIN)/boot.bin
KERN_OUT		= $(BIN)/kernel.bin

CC_DIR 			= $(SRC)
AS_DIR			= $(SRC)
BL_DIR			= $(SRC)/boot
OBJ_DIR			= $(BIN)/obj

CC_FILES_IN		= $(wildcard $(CC_DIR)/*.cpp)
CC_FILES_OUT	= $(patsubst $(CC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(CC_FILES_IN))

AS_FILES_IN		= $(wildcard $(AS_DIR)/*.asm)
AS_FILES_OUT	= $(patsubst $(AS_DIR)/%.asm, $(OBJ_DIR)/%.o, $(AS_FILES_IN))

BL_FILES_IN		= $(wildcard $(BL_DIR)/*.asm)
BL_FILES_OUT	= $(patsubst $(BL_DIR)/%.asm, $(OBJ_DIR)/%.bin, $(BL_FILES_IN))

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(CC_DIR)/%.cpp $(OBJ_DIR)
	$(CC) $(CC_FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(AS_DIR)/%.asm $(OBJ_DIR)
	$(AS) $(AS_FLAGS) $< -o $@

$(OBJ_DIR)/%.bin: $(BL_DIR)/%.asm $(OBJ_DIR)
	$(AS) -f bin $< -o $@

build: $(BL_FILES_OUT) $(CC_FILES_OUT) $(AS_FILES_OUT)
	$(LD) $(LD_FLAGS) -o $(KERN_OUT) $(AS_FILES_OUT) $(CC_FILES_OUT)
	cat $(BL_FILES_OUT) $(KERN_OUT) > $(BOOT_OUT)

emulate:
	qemu-system-x86_64 -m 2G -drive file=$(BOOT_OUT),format=raw,index=0,media=disk -monitor stdio -serial file:output.log

disassemble:
	objdump -m i8086 -M intel -b binary -D $(BOOT_OUT)

dump:
	hd $(BOOT_OUT)

clean:
	rm -fr $(BIN)