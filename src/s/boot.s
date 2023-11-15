// here we actually place the multiboot header. we need to use the linker.ld to ensure 
// that this goes in the first 8kb
.section .multiboot_header
multiboot_header:
    .align 8
    .long 0xE85250D6 // magic number
    .long 0 // using i386 arch
    .long (multiboot_header_end - multiboot_header)
    .long -(0xE85250D6 + (multiboot_header_end - multiboot_header))

multiboot_header_end:

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
    call kernel_main

    // disable interrupts and lock up the computer
    cli
1:  
    hlt
    jmp 1b

// this tells us the size of _start
.size _start, . - _start
    