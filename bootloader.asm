[org 0x7c00]
section .text.boot

; setup stack pointer
align 4
mov bp, $+0x400
mov sp, bp

; get CPU info
mov eax, 1
cpuid
mov DWORD [os_hint_table+8], ecx        ; store in hint structure
mov DWORD [os_hint_table+12], edx       ; store in hint structure
and edx, 0b10000000000000000000000000   ; check that we have have SSE instructions available
cmp edx, 0
je $                                    ; noo SSE, no game

; enable SSE
mov eax, cr0
and ax, 0b1111111111111011  ; disable coprocessor emulation
or ax, 0b0000000000000010   ; enable coprocessor monitoring
mov cr0, eax
mov eax, cr4
or ax, 0b0000011000000000   ; set OSFXSR and OSXMMEXCPT
mov cr4, eax

;jmp screen_mode_success

; set graphics to pixel based
mov ah, 0x4f    ; we want to talk to the VBE controller
mov al, 0x02    ; we want to set the video mode
mov bx, 0b0100000100010010  ; represents the video mode config that we want
int 0x10        ; go!

cmp ax, 0x004f  ; check for success
je screen_mode_success
jmp $           ; hang

; query number of memory blocks
int 12
jmp $

screen_mode_success:

; TODO: load disk sectors
; TODO: then jump to main
; TODO: read memory data
; TODO: project build structure
; TODO: relocate stack?
; TODO: keyboard/mouse IO?

; prepare for the jump to protected mode
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

; TODO: data table to pass to OS

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

align 4
os_hint_table:      ; define a table of useful info to pass to the OS when we eventually jump to kernel main
    dd gdt_descriptor   ; pointer to the GDT
    dd 0x0              ; number of available 1k blocks
    dd 0x0              ; contents of ECX after CPUID
    dd 0x0              ; contents of EDX after CPUID
    dd 0x4a6b7900       ; checksum

times 510-($-$$) db 0   ; fill with zeros
dw 0xaa55               ; define bootable signature byte