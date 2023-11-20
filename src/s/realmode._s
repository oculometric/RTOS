.section .text
.code16

idt_real:
    .int 0x3FF
    .long 0

save_cr0:
    .long 0

enter_real_mode:
    // disable interrupts
    cli

    // do something with the protected mode GDT
    mov DATASEL16, %eax
    mov %eax, %ds
    mov %eax, %es
    mov %eax, %fs
    mov %eax, %gs
    mov %eax, %ss

    // disable paging
    mov %cr0, %eax
    mov %eax, (save_cr0)
    // disable paging and pmode bits
    and $0x7FFFFFFe, %eax
    mov %eax, %cr0

    ljmp $0, $real_mode_begin
    
real_mode_begin:
    // set stack pointer
    mov $0x8000, %sp
    // clear registers
    mov $0, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    // load IDT
    lidt (idt_real)
    // enable interrupts
    sti

    // TODO: do something here