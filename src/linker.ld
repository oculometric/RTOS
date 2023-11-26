ENTRY(_start)

SECTIONS
{
    . = 0x100000;
    KERNEL_START = .;

    .text BLOCK(4K) : ALIGN(4K)
    {
        . = ALIGN(8);
        KEEP(*(.multiboot_header))
        *(.text16)
        *(.text)
    }

    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }
}