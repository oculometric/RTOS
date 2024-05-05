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

    mov DWORD eax, [OS_HINT_TABLE]
    mov DWORD [eax+OHT_KERN_START_OFFSET], 0x100000
    mov DWORD [eax+OHT_KERN_END_OFFSET], 1

read_program_header_table:
    mov ebp, 0          ; use ebp to keep count of how many program headers we've loaded
    mov edx, [ELF_HEADER_USEFUL+EHU_PROGHEADTBL_OFFSET] ; start at the first entry
    add edx, [ELF_HEADER_USEFUL+EHU_FILESTART_OFFSET]   ; offset it from the start of the file
read_program_header:
    ; check this is a loadable program section
    mov DWORD eax, [edx+EPHT_TYPE_OFFSET]
    cmp eax, 1
    jne step_to_next_program_header
    
    ; we need to copy the whole program segment into the right place in memory
    ; eax will be our working register
    ; ebx will be the place we're copying from
    ; ecx will be the place we're copying to
    ; edx will be where we stop copying

    ; first we clear bytes
    push edx

    mov ebx, [edx+EPHT_OFFSET_OFFSET]
    add ebx, [ELF_HEADER_USEFUL+EHU_FILESTART_OFFSET]
    mov ecx, [edx+EPHT_VADDR_OFFSET]
    mov eax, ecx
    add eax, [edx+EPHT_MEMSZ_OFFSET]
    mov edx, eax

    ; check if this is the furthest point so far
    mov DWORD eax, [OS_HINT_TABLE]
    cmp edx, [eax+OHT_KERN_END_OFFSET]
    jle clear_program_byte
    mov [eax+OHT_KERN_END_OFFSET], edx
clear_program_byte:
    mov BYTE [ecx], 0
    inc ebx
    inc ecx
    cmp ecx, edx
    jne clear_program_byte

    ; then we do the actual copying
    pop edx
    push edx

    mov ebx, [edx+EPHT_OFFSET_OFFSET]
    add ebx, [ELF_HEADER_USEFUL+EHU_FILESTART_OFFSET]
    mov ecx, [edx+EPHT_VADDR_OFFSET]
    mov eax, ecx
    add eax, [edx+EPHT_FILESZ_OFFSET]
    mov edx, eax
copy_program_byte:
    mov BYTE al, [ebx]
    mov BYTE [ecx], al
    inc ebx
    inc ecx
    cmp ecx, edx
    jne copy_program_byte

    pop edx

step_to_next_program_header:
    inc ebp             ; add 1 to the entry counter
    cmp bp, [ELF_HEADER_USEFUL+EHU_PROGHEADNUM_OFFSET] ; exit if we've now seen enough entries
    jge read_section_header_table
    mov eax, 0
    mov ax, [ELF_HEADER_USEFUL+EHU_PROGHEADSZ_OFFSET]
    add edx, eax        ; otherwise, move onto the next entry
    jmp read_program_header

read_section_header_table:
    mov ebp, 0          ; use ebp to keep track of how many sections we've looked at
    mov edx, [ELF_HEADER_USEFUL+EHU_SECTHEADTBL_OFFSET] ; edx holds the offset of the first section header from the file start
    add edx, [ELF_HEADER_USEFUL+EHU_FILESTART_OFFSET]   ; calculate it's address in memory
read_section_header:
    mov DWORD eax, [edx+ESHT_FLAGS_OFFSET]
    and ecx, 0x4    ; if the section is flagged as executable, skip it
    jnz step_to_next_section_header
    mov DWORD eax, [edx+ESHT_FLAGS_OFFSET]
    and ecx, 0x2    ; if the section is not flagged for allocation, skip it
    jz step_to_next_section_header

    ; determine what kind of section we're looking at
    mov DWORD eax, [edx+ESHT_TYPE_OFFSET]
    cmp eax, 1
    je read_progbits_header
    cmp eax, 8
    je read_nobits_header
    jmp step_to_next_program_header

read_progbits_header:
    ; setting ourselves up for more copying
    push edx

    mov ebx, [edx+ESHT_OFFSET_OFFSET]
    add ebx, [ELF_HEADER_USEFUL+EHU_FILESTART_OFFSET]
    mov ecx, [edx+ESHT_VADDR_OFFSET]
    mov eax, ecx
    add eax, [edx+ESHT_FILESZ_OFFSET]
    mov edx, eax

    ; check if this is the furthest point so far
    mov DWORD eax, [OS_HINT_TABLE]
    cmp edx, [eax+OHT_KERN_END_OFFSET]
    jle copy_progbits_byte
    mov [eax+OHT_KERN_END_OFFSET], edx

copy_progbits_byte:
    mov BYTE al, [ebx]
    mov BYTE [ecx], al
    inc ebx
    inc ecx
    cmp ecx, edx
    jne copy_progbits_byte

    pop edx
    jmp step_to_next_section_header

read_nobits_header:
    push edx

    ; we only need our destination start and stop point

    mov ecx, [edx+ESHT_VADDR_OFFSET]
    mov eax, ecx
    mov eax, [edx+ESHT_FILESZ_OFFSET]
    mov edx, eax
    mov al, 0

    ; check if this is the furthest point so far
    mov DWORD eax, [OS_HINT_TABLE]
    cmp edx, [eax+OHT_KERN_END_OFFSET]
    jle write_nobits_byte
    mov [eax+OHT_KERN_END_OFFSET], edx

write_nobits_byte:
    mov BYTE [ecx], al
    inc ecx
    cmp ecx, edx
    jne write_nobits_byte

    pop edx
    jmp step_to_next_section_header

step_to_next_section_header:
    inc ebp             ; add 1 to the entry counter
    cmp bp, [ELF_HEADER_USEFUL+EHU_SECTHEADNUM_OFFSET] ; exit if we've now seen enough entries
    jge section_headers_read
    mov eax, 0
    mov ax, [ELF_HEADER_USEFUL+EHU_SECTHEADSZ_OFFSET]
    add edx, eax        ; otherwise, move onto the next entry
    jmp read_section_header

section_headers_read:
    ; fuck it, geronimo part two!!!
    mov eax, [ELF_HEADER_USEFUL+EHU_PROGENT_OFFSET]
    push DWORD [OS_HINT_TABLE]
    jmp eax

elf_load_error:
    mov ebx, 0xffffffff
    mov edx, 0xffffffff
    hlt
    jmp $

OS_HINT_TABLE:
    dd 0x00000000

EHU_FILESTART_OFFSET equ 0          ; 4-byte memory address where the ELF file starts
EHU_PROGENT_OFFSET equ 4            ; 4-byte memory address where the program entry should end up
EHU_PROGHEADTBL_OFFSET equ 8        ; 4-byte offset into the file where the program header table starts
EHU_SECTHEADTBL_OFFSET equ 12       ; 4-byte offset into the file where the section header table starts
EHU_PROGHEADSZ_OFFSET equ 16        ; 2-byte number describing the size of each program header
EHU_PROGHEADNUM_OFFSET equ 18       ; 2-byte number describing how many program headers to expect in the program header table
EHU_SECTHEADSZ_OFFSET equ 20        ; 2-byte number describing the size of each section header
EHU_SECTHEADNUM_OFFSET equ 22       ; 2-byte number describing how many section headers to expect in the section header table
EHU_SECTHEADNAMESIND_OFFSET equ 24  ; 2-byte something something blah blah blah

align 4
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

EPHT_TYPE_OFFSET equ 0
EPHT_OFFSET_OFFSET equ 4
EPHT_VADDR_OFFSET equ 8
EPHT_PADDR_OFFSET equ 12
EPHT_FILESZ_OFFSET equ 16
EPHT_MEMSZ_OFFSET equ 20
EPHT_FLAGS_OFFSET equ 24
EPHT_ALIGN_OFFSET equ 28

ESHT_NAME_OFFSET equ 0
ESHT_TYPE_OFFSET equ 4
ESHT_FLAGS_OFFSET equ 8
ESHT_VADDR_OFFSET equ 12
ESHT_OFFSET_OFFSET equ 16
ESHT_FILESZ_OFFSET equ 20
ESHT_LINK_OFFSET equ 24
ESHT_INFO_OFFSET equ 28
ESHT_ADDRALIGN_OFFSET equ 32
ESHT_ENTSIZE_OFFSET equ 36

%include "src/os_hint_table.mac"

; this is where we start searching for the ELF header, so add some padding
kernelloader_end:
    dd 0x00000000