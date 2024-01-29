TOOLPATH 	:= $(HOME)/opt/cross/bin/
PREFIX 		:= $(TOOLPATH)i386-elf-

AS			:= nasm
CC			:= $(PREFIX)g++
LD			:= $(PREFIX)ld

AS_FLAGS	:= -f elf
CC_FLAGS	:= -ffreestanding -m32 -g -masm=intel -Wall -Wextra -Wpedantic
LD_FLAGS	:= -T linker.ld

BIN			:= bin
SRC			:= src

BOOT_OUT	:= $(BIN)/boot.bin

build:
	mkdir -p bin
	$(AS) -f bin $(SRC)/bootloader.asm -o $(BIN)/bootloader.bin
	$(AS) -f bin $(SRC)/kernelloader.asm -o $(BIN)/kernelloader.bin
	$(AS) $(AS_FLAGS) $(SRC)/kernel_premain.asm -o $(BIN)/kernel_premain.o
	$(CC) $(CC_FLAGS) -c $(SRC)/kernel_main.cpp -o $(BIN)/kernel_main.o
	$(CC) $(CC_FLAGS) -c $(SRC)/serial.cpp -o $(BIN)/serial.o
	$(CC) $(CC_FLAGS) -c $(SRC)/graphics.cpp -o $(BIN)/graphics.o
	$(CC) $(CC_FLAGS) -c $(SRC)/colour.cpp -o $(BIN)/colour.o
	$(LD) $(LD_FLAGS) -o $(BIN)/kernel.bin $(BIN)/kernel_premain.o $(BIN)/kernel_main.o $(BIN)/serial.o $(BIN)/graphics.o $(BIN)/colour.o
	cat $(BIN)/bootloader.bin $(BIN)/kernelloader.bin $(BIN)/kernel.bin > $(BOOT_OUT)

emulate:
	qemu-system-x86_64 -m 2G -drive file=$(BOOT_OUT),format=raw,index=0,media=disk -monitor stdio -serial file:output.log

disassemble:
	objdump -m i8086 -M intel -b binary -D $(BOOT_OUT)

dump:
	hd $(BOOT_OUT)