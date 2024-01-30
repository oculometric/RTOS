ENTRY(_start)

SECTIONS
{
    . = 1M;
    .text :
    {
        *(.prekernel)
        *(.text)
    }

    .data :
    {
        *(.data)
    }
}