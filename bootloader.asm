[org 0x7c00]
section .text.boot

; set up os hint table, to be passed to OS when jumping to protected mode
mov DWORD [OS_HINT_TABLE+OHT_GDT_OFFSET], gdt_descriptor
mov DWORD [OS_HINT_TABLE+OHT_MEMTBL_OFFSET], MMAP_TABLE_ADDR
mov DWORD [OS_HINT_TABLE+OHT_CHKSM_OFFSET], 0x4a6b7900

; setup stack pointer
align 4
mov bp, $+0x400
mov sp, bp

; get CPU info
cpu_check:
    mov eax, 1
    cpuid
    mov DWORD [OS_HINT_TABLE+OHT_CPUID_ECX], ecx        ; store in hint structure
    mov DWORD [OS_HINT_TABLE+OHT_CPUID_EDX], edx       ; store in hint structure
    and edx, 0b10000000000000000000000000   ; check that we have have SSE instructions available
    cmp edx, 0
    jne sse_config                          ; noo SSE, no game
    mov cl, 'F'
    jmp show_error

; enable SSE
sse_config:
    mov eax, cr0
    and ax, 0b1111111111111011  ; disable coprocessor emulation
    or ax, 0b0000000000000010   ; enable coprocessor monitoring
    mov cr0, eax
    mov eax, cr4
    or ax, 0b0000011000000000   ; set OSFXSR and OSXMMEXCPT
    mov cr4, eax

memory_query:
    ; query low memory
    int 12
    mov [OS_HINT_TABLE+OHT_LOWMEM_OFFSET], ax

    ; query high memory, in 24 byte entries
    mov ax, 0x50
    mov es, ax          ; memory map table starts here
    mov di, 0x0         ; start at zero
    mov bp, 0           ; count number of entries here
    mov ebx, 0x0        ; used by the BIOS to keep track of where it is in the memory quest
next_mmap_entry:
    mov eax, 0xe820     ; ultimate memory finding function
    mov ecx, 24         ; expected number of bytes
    mov edx, 0x534d4150 ; magic number
    int 0x15
    jnc no_carry
    cmp bp, 0
    je mmap_failed      ; if we have no entries, and the carry flag was set (i.e first run) something went wrong
    jmp mmap_complete   ; if we have entries and the carry flag was set, then we reached the end of the list
no_carry:
    cmp eax, 0x534d4150
    jne mmap_failed     ; if not set to magic value, something went wrong
    inc bp              ; increment number of entries
    add di, 24          ; increment pointer to next entry
    cmp ebx, 0
    jne next_mmap_entry ; if EBX is zero, we've reached the end, otherwise go back for another entry
mmap_complete:
    mov [OS_HINT_TABLE+OHT_HIGHMEM_OFFSET], bp
    jmp screen_mode_setup
mmap_failed:
    mov cl, 'M'
    jmp show_error

; set graphics to pixel based
screen_mode_setup:
    mov ah, 0x4f                ; we want to talk to the VBE controller
    mov al, 0x02                ; we want to set the video mode
    mov bx, 0b0100000100010010  ; represents the video mode config that we want
    int 0x10                    ; go!
    cmp ax, 0x004f              ; check for success
    je protected_mode_setup
    mov cl, 'G'
    jmp show_error

; TODO: load disk sectors
; TODO: then jump to main
; TODO: project build structure
; TODO: relocate stack?
; TODO: keyboard/mouse IO?

; prepare for the jump to protected mode
protected_mode_setup:
    cli                     ; clear interrupts
    lgdt [gdt_descriptor]   ; load flat GDT
    mov eax, cr0            ; copy out cr0
    or eax, 0x1             ; turn on bit 0 (protected mode)
    mov cr0, eax            ; copy back into cr0
    jmp GDT_CODE_SEGMENT_OFFSET:protected_mode_arrival  ; perform the far jump to protected mode entry point

[bits 32]
protected_mode_arrival:
    mov BYTE [0xb8000], 'N'
    jmp $

[bits 16]
align 4
; GDT
gdt_descriptor:
    dw (gdt_end - gdt_start) - 1    ; length - 1
    dd gdt_start                    ; start pointer

gdt_start:
gdt_null:
    dd 0
    dd 0
gdt_code:
    dw 0xffff       ; first 16 bits of limit
    dw 0x0          ; first 16 bits of base
    db 0x0          ; next 8 bit of base
    db 0b10011010   ; flags and type
    db 0b11001111   ; other flags and last 4 bits of limit
    db 0x0          ; last 8 bits of base
gdt_data:
    dw 0xffff       ; first 16 bits of limit
    dw 0x0          ; first 16 bits of base
    db 0x0          ; next 8 bit of base
    db 0b10010010   ; flags and type
    db 0b11001111   ; other flags and last 4 bits of limit
    db 0x0          ; last 8 bits of base
gdt_end:

GDT_CODE_SEGMENT_OFFSET equ gdt_code - gdt_start
GDT_DATA_SEGMENT_OFFSET equ gdt_data - gdt_start
OS_HINT_TABLE equ 0x500
MMAP_TABLE_ADDR equ 0x900

OHT_GDT_OFFSET equ 0
OHT_LOWMEM_OFFSET equ 4
OHT_CPUID_ECX equ 8
OHT_CPUID_EDX equ 12
OHT_HIGHMEM_OFFSET equ 16
OHT_MEMTBL_OFFSET equ 20
OHT_CHKSM_OFFSET equ 24

;os_hint_table:          ; define a table of useful info to pass to the OS when we eventually jump to kernel main. this table is now defined at 0x500 for safety
;    dd gdt_descriptor   ; pointer to the GDT
;    dd 0x0              ; number of available 1k blocks
;    dd 0x0              ; contents of ECX after CPUID
;    dd 0x0              ; contents of EDX after CPUID
;    dd 0x0              ; number of memory map table entries
;    dd MMAP_TABLE_ADDR  ; pointer to first entry in memory map table
;    dd 0x4a6b7900       ; checksum

show_error:             ; places the character in cl as an error code on the screen
    mov ax, 0xb800
    mov es, ax
    mov di, 0x0
    mov BYTE [es:di+1], 0x47
    mov BYTE [es:di], cl
    jmp $

times 510-($-$$) db 0   ; fill with zeros
dw 0xaa55               ; define bootable signature byte