[bits 32]
section .text

p_gdtr:
    dw 0    ; limit
    dd 0    ; base

[global setProtectedGDT]
setProtectedGDT:
    mov ax, [esp + 4]
    mov [p_gdtr], ax
    mov eax, [esp + 8]
    mov [p_gdtr + 2], ax
    lgdt [p_gdtr]
    ret

[global reloadSegments]
reloadSegments:
    push DWORD [esp + 4]
    push reloadCS
    retf
reloadCS:
    mov ax, [esp + 8]
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret