assemble:
	nasm -f bin bootloader.asm -o boot.bin

emulate:
	qemu-system-x86_64 boot.bin -monitor stdio

disassemble:
	objdump -m i8086 -M intel -b binary -D boot.bin

dump:
	hd boot.bin