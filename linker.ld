SECTIONS
{
    . = 0x7c00;
    .text.boot: 
    {
        *(.text.boot)
    }
    .text.prekernel:
    {
        *(.text.prekernel)
    }
    .text:
    {
        *(.text)
    }
}