[bits 32]
[extern main]
section .text.prekernel
call main
jmp $

times 510-($-$$) db 0xFF; fill with zeros
dw 0xcc22               ; define special signature byte