[org 0x10000]
[bits 32]

; hopefully this will end up at 0x10000
kernelloader_pickup:
    mov eax, 0x4F4F4F4F
    pop ebx
    mov [OS_HINT_TABLE], ebx
    mov ecx, kernelloader_end
elf_header_find:
    mov DWORD eax, [ecx]
    inc ecx
    cmp eax, 0x464c457f     ; find the signature byte
    jne elf_header_find
    dec ecx
    mov ebp, ecx            ; store the start of the ELF in ebp
    add ecx, 4
    cmp BYTE [ecx], 1       ; check we have a 32 bit ELF
    jne elf_load_error
    inc ecx
    cmp BYTE [ecx], 1       ; check we're in little endian mode (idk what to do if we're not)
    jne elf_load_error
    add ecx, 11
    cmp WORD [ecx], 2       ; check this is an executable
    jne elf_load_error
    add ecx, 2
    cmp WORD [ecx], 0x03    ; check this is an x86 executable
    jne elf_load_error

populate_EHU:
    mov [ELF_HEADER_USEFUL+EHU_FILESTART_OFFSET], ebp
    mov DWORD eax, [ebp+24]
    mov DWORD [ELF_HEADER_USEFUL+EHU_PROGENT_OFFSET], eax
    mov DWORD eax, [ebp+28]
    mov DWORD [ELF_HEADER_USEFUL+EHU_PROGHEADTBL_OFFSET], eax
    mov DWORD eax, [ebp+32]
    mov DWORD [ELF_HEADER_USEFUL+EHU_SECTHEADTBL_OFFSET], eax
    mov WORD ax, [ebp+42]
    mov WORD [ELF_HEADER_USEFUL+EHU_PROGHEADSZ_OFFSET], ax
    mov WORD ax, [ebp+44]
    mov WORD [ELF_HEADER_USEFUL+EHU_PROGHEADNUM_OFFSET], ax
    mov WORD ax, [ebp+46]
    mov WORD [ELF_HEADER_USEFUL+EHU_SECTHEADSZ_OFFSET], ax
    mov WORD ax, [ebp+48]
    mov WORD [ELF_HEADER_USEFUL+EHU_SECTHEADNUM_OFFSET], ax
    mov WORD ax, [ebp+50]
    mov WORD [ELF_HEADER_USEFUL+EHU_SECTHEADNAMESIND_OFFSET], ax
    hlt
    jmp $

elf_load_error:
    mov ebx, 0xffffffff
    mov edx, 0xffffffff
    hlt
    jmp $

OS_HINT_TABLE:
    dd 0x00000000

align 4
EHU_FILESTART_OFFSET equ 0          ; 4-byte memory address where the ELF file starts
EHU_PROGENT_OFFSET equ 4            ; 4-byte memory address where the program entry should end up
EHU_PROGHEADTBL_OFFSET equ 8        ; 4-byte offset into the file where the program header table starts
EHU_SECTHEADTBL_OFFSET equ 12       ; 4-byte offset into the file where the section header table starts
EHU_PROGHEADSZ_OFFSET equ 16        ; 2-byte number describing the size of each program header
EHU_PROGHEADNUM_OFFSET equ 18       ; 2-byte number describing how many program headers to expect in the program header table
EHU_SECTHEADSZ_OFFSET equ 20        ; 2-byte number describing the size of each section header
EHU_SECTHEADNUM_OFFSET equ 22       ; 2-byte number describing how many section headers to expect in the section header table
EHU_SECTHEADNAMESIND_OFFSET equ 24  ; 2-byte something something blah blah blah

ELF_HEADER_USEFUL:
    dd 0
    dd 0
    dd 0
    dd 0
    dw 0
    dw 0
    dw 0
    dw 0
    dw 0

%include "src/os_hint_table.mac"

; this is where we start searching for the ELF header
kernelloader_end:
    dd 0x00000000