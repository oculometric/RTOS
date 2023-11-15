PREFIX		= $(HOME)/opt/cross/bin/
CC			= $(PREFIX)i686-elf-g++
CC_FLAGS 	= -ffreestanding -O1 -Wall -Wextra -fno-exceptions -fno-rtti
AS			= $(PREFIX)i686-elf-as
LK			= $(PREFIX)i686-elf-gcc
LK_FLAGS	= -ffreestanding -O1 -nostdlib -lgcc

CC_DIR		= src/c
INC_DIR		= src/h
AS_DIR		= src/s
OBJ_DIR		= bin/o

CC_FILES_IN	:= $(wildcard $(CC_DIR)/*.cc)
AS_FILES_IN	:= $(wildcard $(AS_DIR)/*.s)

CC_FILES_OUT=$(patsubst $(CC_DIR)/%.cc, $(OBJ_DIR)/%.o, $(CC_FILES_IN))
AS_FILES_OUT=$(patsubst $(AS_DIR)/%.s, $(OBJ_DIR)/%.o, $(AS_FILES_IN))

LD			= src/linker.ld

BIN			= rtos.bin
BIN_OUT		= bin/$(BIN)

ISO			= bin/rtos.iso

obj_dir:
	@[ -d $(OBJ_DIR) ] || mkdir $(OBJ_DIR) -p

$(OBJ_DIR)/%.o: $(CC_DIR)/%.cc obj_dir
	@echo Compiling $<...
	@$(CC) -c $< -o $@ $(CC_FLAGS) -I$(INC_DIR)

$(OBJ_DIR)/%.o: $(AS_DIR)/%.s obj_dir
	@echo Compiling $<...
	@$(AS) $< -o $@

build: $(CC_FILES_OUT) $(AS_FILES_OUT) obj_dir
	@echo Linking...
	@$(LK) -T $(LD) -o $(BIN_OUT) $(LK_FLAGS) $(AS_FILES_OUT) $(CC_FILES_OUT) -I$(INC_DIR)
	@echo Link success.
	
	@echo Verifying multiboot2...
	@grub-file --is-x86-multiboot $(BIN_OUT)
	@echo Verified.

	@cp $(BIN_OUT) $(BIN)

	@echo Building ISO...
	@mkdir -p bin/isodir/boot/grub
	@cp $(BIN_OUT) bin/isodir/boot/$(BIN)
	@cp src/grub.cfg bin/isodir/boot/grub.cfg
	@grub-mkrescue /usr/lib/grub/i386-pc -o $(ISO) bin/isodir
	@echo Done.

clean:
	@rm -fr bin
	@rm -f $(BIN)
	@echo Cleaned.

emulate: build
	qemu-system-i386 -kernel $(BIN_OUT) -serial file:serial.log -machine type=pc-i440fx-3.1 --no-shutdown --no-reboot