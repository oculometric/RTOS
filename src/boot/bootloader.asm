[org 0x7c00]
section .boot

; set up os hint table, to be passed to OS when jumping to protected mode
mov DWORD [OS_HINT_TABLE+OHT_GDT_OFFSET], gdt_descriptor
mov DWORD [OS_HINT_TABLE+OHT_MEMTBL_OFFSET], MMAP_TABLE_ADDR
mov DWORD [OS_HINT_TABLE+OHT_CHKSM_OFFSET], 0x4a6b7900
mov DWORD [OS_HINT_TABLE+OHT_BDA_OFFSET], 0x400
mov WORD [OS_HINT_TABLE+OHT_DISKNUM_OFFSET], 0x0
mov [OS_HINT_TABLE+OHT_DISKNUM_OFFSET], dl
mov DWORD [OS_HINT_TABLE+OHT_NOVO_OFFSET], 0x4f564f4e

; setup stack pointer, allowing 2kb for stack immediately (almost immediately) after this bootloader
align 4
mov bp, STACK_BASE_ADDR
mov sp, bp

; get CPU info
cpu_check:
    mov eax, 1
    cpuid
    mov DWORD [OS_HINT_TABLE+OHT_CPUID_ECX_OFFSET], ecx        ; store in hint structure
    mov DWORD [OS_HINT_TABLE+OHT_CPUID_EDX_OFFSET], edx       ; store in hint structure
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
    int 0x12
    mov [OS_HINT_TABLE+OHT_LOWMEM_OFFSET], ax

    ; query high memory, in 24 byte entries
    mov ax, MMAP_TABLE_ADDR / 16
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
    jmp load_kernel_bootstrapper
mmap_failed:
    mov cl, 'M'
    jmp show_error

; load the next 127 sectors (each 512b)
load_kernel_bootstrapper:
    mov bx, KERNEL_LOAD_ADDR / 16
    mov es, bx      ; kernel will be loaded at es:bx, so we set that to be 0:KERNEL_LOAD_ADDR
    mov bx, 0x0
    mov ah, 2
    mov ch, 0       ; cylinder number
    mov cl, 2       ; start at sector 1 (CHS sector index starts at 1 because of course it does)
    mov dl, [OS_HINT_TABLE+OHT_DISKNUM_OFFSET] ; disk number
    mov bp, 0       ; used to keep track of retries
continue_kernel_loading:
    mov ah, 2
    mov al, 1       ; load 1 sector
    mov dh, 0       ; head number (this gets clobbered)
    int 0x13        ; actually do the interrupt this time
    jc kernel_end_check
    cmp ah, 0       ; if we failed, just try again
    jne retry_load_kernel
    add bx, 0x200   ; increment bx by 512
    inc cl          ; move to next sector
    cmp cl, 126
    jge kernel_load_finished
    mov bp, 0       ; reset retry counter
    jmp continue_kernel_loading

retry_load_kernel:
    cmp bp, 3
    jge kernel_load_failed
    inc bp

kernel_end_check:
    cmp bx, 0x0     ; if zero, then there were no sectors to load, and thus no kernel to jump to (so dont do that)
    je kernel_load_failed
    jmp kernel_load_finished

kernel_load_failed:
    mov cl, 'D'
    jmp show_error

kernel_load_finished:

; set graphics to pixel based
screen_mode_setup:
    mov ah, 0x4f                ; we want to talk to the VBE controller
    mov al, 0x02                ; we want to set the video mode
    mov bx, 0b1100000100010010  ; represents the video mode config that we want
    int 0x10                    ; go!
    cmp ax, 0x004f              ; check for success
    je screen_mode_infograb
    mov cl, 'G'
    jmp show_error
screen_mode_infograb:
    mov ah, 0x4f
    mov al, 0x01
    mov cx, bx
    mov bx, VBE_MODEINFO_ADDR / 16
    mov es, bx
    mov di, 0
    int 0x10
    mov DWORD [OS_HINT_TABLE+OHT_VBE_MODE_INFO_OFFSET], 0x0
    cmp ax, 0x004f
    je screen_infograb_success
    mov cl, 'G'
    jmp show_error
screen_infograb_success:
    mov DWORD [OS_HINT_TABLE+OHT_VBE_MODE_INFO_OFFSET], VBE_MODEINFO_ADDR

; prepare for the jump to protected mode
protected_mode_setup:
    cli                     ; clear interrupts
    lgdt [gdt_descriptor]   ; load flat GDT
    mov eax, cr0            ; copy out cr0
    or eax, 0x1             ; turn on bit 0 (protected mode)
    mov cr0, eax            ; copy back into cr0
    mov ax, GDT_DATA_SEGMENT_OFFSET
    mov es, ax
    mov ds, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp GDT_CODE_SEGMENT_OFFSET:protected_mode_arrival  ; perform the far jump to protected mode entry point

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

BOOTLOADER_ADDR equ 0x7c00      ; address where the bootloader (this fucker) starts
STACK_BASE_ADDR equ BOOTLOADER_ADDR + 0x200 + 0x800 ; address where the stack base starts (the stack grows down toward the bootloader, oh boy i sure do hope that doesnt cause any problems)
KERNEL_LOAD_ADDR equ 0x10000    ; address where the kernel will be loaded (i.e. 128 sectors of raw executable)
KERNEL_FINAL_ADDR equ 0x100000  ; address where the kernel code should actually be (something something ELF relocation ill do it later)
OS_HINT_TABLE equ STACK_BASE_ADDR + 0x200   ; address where the hint table for the OS will be placed, just on top of the stack (with some padding)
VBE_MODEINFO_ADDR equ OS_HINT_TABLE + 0x400 ; address where the VBE mode info table will be placed
MMAP_TABLE_ADDR equ VBE_MODEINFO_ADDR + 0x200 ; address where the memory map will be placed, on top of the VBE mode info block

%include "src/os_hint_table.mac"

show_error:             ; places the character in cl as an error code on the screen
    mov ax, 0xb800
    mov es, ax
    mov di, 0x0
    mov BYTE [es:di+1], 0x47
    mov BYTE [es:di], cl
    jmp $

[bits 32]
protected_mode_arrival:
    mov BYTE [0xb8000], 'N'
    mov eax, 0x4a6b7900
    push OS_HINT_TABLE
    jmp KERNEL_LOAD_ADDR                 ; geronimo!

times 510-($-$$) db 0   ; fill with zeros
dw 0xaa55               ; define bootable signature byte