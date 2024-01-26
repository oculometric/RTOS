TOOLPATH 	:= $(HOME)/opt/cross/bin/
PREFIX 		:= $(TOOLPATH)i386-elf-

AS			:= nasm
CC			:= $(PREFIX)gcc
LD			:= $(PREFIX)ld

BIN			:= bin
SRC			:= src

BOOT_OUT	:= $(BIN)/boot.bin

build:
	mkdir -p bin
	$(AS) -f bin $(SRC)/bootloader.asm -o $(BIN)/bootloader.bin
	$(AS) -f bin $(SRC)/kernelloader.asm -o $(BIN)/kernelloader.bin
	$(AS) -f elf $(SRC)/kernel_premain.asm -o $(BIN)/kernel_premain.o
	$(CC) -ffreestanding -m32 -g -c $(SRC)/kernel_main.cpp -o $(BIN)/kernel_main.o
	$(LD) -T linker.ld -o $(BIN)/kernel.bin $(BIN)/kernel_premain.o $(BIN)/kernel_main.o
	cat $(BIN)/bootloader.bin $(BIN)/kernelloader.bin $(BIN)/kernel.bin > $(BOOT_OUT)

emulate:
	qemu-system-x86_64 $(BOOT_OUT) -monitor stdio

disassemble:
	objdump -m i8086 -M intel -b binary -D $(BOOT_OUT)

dump:
	hd $(BOOT_OUT)