[org 0x10000]
[bits 32]

; hopefully this will end up at 0x10000
kernelloader_pickup:
    mov eax, 0x4F4F4F4F
    pop ebx
    hlt
    jmp $