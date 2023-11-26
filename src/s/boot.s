// --------- THIS IS A VALID MULTIBOOT 1 HEADER. ------- //
.set ALIGN,     1<<0
.set MEMINFO,   1<<1
.set FLAGS,     ALIGN | MEMINFO
.set MAGIC,     0x1BADB002 // this is the magic number for legacy GRUB. change it
.set CHECKSUM,  -(MAGIC + FLAGS)

// here we actually place the multiboot header. we need to use the linker.ld to ensure 
// that this goes in the first 8kb
.section .multiboot_header
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

// --- THE ABOVE CAUSE QEMU TO CRASH OUT WHEN LOADING FROM A CD ROM ----- //

.section .data
.align 4

gdt:

// gdt_null is a null descriptor
gdt_null:
    .long 0
    .long 0

// gdt_code is a kernel code descriptor, running from 0x0 and length 0xFFFFF, ring 0, conforming code, in protected 32-bit paged
gdt_code:
    .int 0xFFFF
    .int 0x0000
    .byte 0x00
    .byte 0b10011010
    .byte 0b11001111
    .byte 0x00

// gdt_data is a kernel data descriptor, running from 0x0 and length 0xFFFFF, ring 0, up-growing data, in protected 32-bit paged
gdt_data:
    .int 0xFFFF
    .int 0x0000
    .byte 0x00
    .byte 0b10010010
    .byte 0b11001111
    .byte 0x00

gdt_end:

.set GDT_LIMIT, (gdt_end-gdt)-1

gdt_descriptor:
    .int GDT_LIMIT
    .long gdt-0x7C00


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
    // load the new GDT
    lgdt (gdt_descriptor)

    //int $0x10

    //cli
//4:
    //hlt
    //jmp 4b

    // far jump to update CS, offset to the first GDT entry
    jmpl $0x08, $post_start

post_start:
    cli
4:
    hlt
    jmp 4b

    // update other descriptor registers to be the second GDT entry
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    // first we need to configure the stack. the esp register is set to
    // the top of the stack, since the stack grows down on x86
    mov $stack_top, %esp

    // pass arguments to kernel_main
    //push %eax
    //push %ebx

    // jump into the C main
    //call kernel_main

    // disable interrupts and lock up the computer
    cli
1:  
    hlt
    jmp 1b

// this tells us the size of _start
.size _start, . - _start
    