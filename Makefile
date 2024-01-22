build:
	nasm -f bin bootloader.asm -o bootloader.o
	nasm -f elf secondsector.asm -o secondsector.o
	i386-elf-gcc -ffreestanding -m32 -g -c kernel_main.cpp -o kernel_main.o
	i386-elf-ld -T linker.ld -o kernel.bin kernel_entry.o kernel.o
	cat bootloader.o kernel.bin > boot.bin

emulate:
	qemu-system-x86_64 boot.bin -monitor stdio

disassemble:
	objdump -m i8086 -M intel -b binary -D boot.bin

dump:
	hd boot.bin