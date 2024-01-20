[org 0x7c00]
section .text.boot

; setup stack pointer
mov bp, $+0x400
mov sp, bp

jmp screen_mode_success

; set graphics to pixel based
mov ah, 0x4f    ; we want to talk to the VBE controller
mov al, 0x02    ; we want to set the video mode
mov bx, 0b0100000100010010  ; represents the video mode config that we want
int 0x10        ; go!

cmp ax, 0x004f  ; check for success
je screen_mode_success
jmp $           ; hang

screen_mode_success:
; TODO: better demo screen
;jmp $           ; lock up

; TODO: load disk sectors
; TODO: then jump to main
; TODO: read video data
; TODO: read memory data
; TODO: enable floating point extensions

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

times 510-($-$$) db 0   ; fill with zeros
dw 0xaa55               ; define bootable signature byte