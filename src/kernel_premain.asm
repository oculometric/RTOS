[bits 32]
[extern main]
section .prekernel

dd 0x4a6b7900

[global _start]
_start:
    call main
    jmp $

times 510-($-$$) db 0xFF; fill with zeros
dw 0xcc22               ; define special signature byte