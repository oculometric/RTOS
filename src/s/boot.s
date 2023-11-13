// here is the multiboot header, this tells the bootloader that we are a multiboot
// kernel
.set ALIGN,     1<<0
.set MEMINFO,   1<<1
.set FLAGS,     ALIGN | MEMINFO
.set MAGIC,     0x1BADB002 // this is the magic number for legacy GRUB. change it
.set CHECKSUM,  -(MAGIC + FLAGS)

// here we actually place the multiboot header. we need to use the linker.ld to ensure 
// that this goes in the first 8kb
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

// now we declare the stack
.section .bss
.align 16
stack_bottom:
    .skip 16384
stack_top:


// this is where we end up when the bootloader is done
.section .text
.global _start
.type _start, @function
_start:
    // first we need to configure the stack. the esp register is set to
    // the top of the stack, since the stack grows down on x86
    mov $stack_top, %esp

    // jump into the C main
    //call kernel_main

    // disable interrupts and lock up the computer
    cli
1:  
    hlt
    jmp 1b

// this tells us the size of _start
.size _start, . - _start
    