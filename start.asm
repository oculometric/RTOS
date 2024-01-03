[org 0x7c00]
top:

; setup stack pointer
mov bp, 0x7c00+0x400
mov sp, bp

; print some numbers to demo the hex stuff
push 0x55aa
call print_hex_16
push 0x45ef
call print_hex_16
push 0x69
call print_hex_8

; newline
call print_ln

; say hi!
push hello_world
call print_string

push intro_string
call print_string

; print out a character literal version of the image
mov bx, top
mov cx, 0
mov ah, 0x0e
self_display_loop:
    mov al, [bx]
    cmp bx, end
    je self_display_end
    cmp al, 32
    jge skip_subtract
    mov al, 32
    skip_subtract:
    int 0x10
    inc bx
    inc cx
    cmp cx, 32
    jne self_display_loop
    mov cx, 0
    mov al, 0xa
    int 0x10
    mov al, 0xd
    int 0x10
    jmp self_display_loop
self_display_end:

; wait for user input
mov ah, 0x0
int 0x16

; print ourself in hex now
call print_ln

push second_string
call print_string

mov bx, top
mov cx, 0
mov dh, 0
self_display_hex_loop:
    mov dl, [bx]
    cmp bx, end
    je self_display_hex_end
    pusha
    push dx
    call print_hex_8
    popa
    mov ah, 0x0e
    mov al, ' '
    int 0x10
    inc bx
    inc cx
    cmp cx, 32
    jne self_display_hex_loop
    mov cx, 0
    mov al, 0xa
    int 0x10
    mov al, 0xd
    int 0x10
    mov ah, 0x0
    int 0x16
    jmp self_display_hex_loop
self_display_hex_end:


sleep:
    mov ax, 0xFFFF
    mov bx, 0x8FFF
_sleep:
    dec ax
    cmp ax, 0x00
    jne _sleep
    dec bx
    mov ax, 0xFFFF
    cmp bx, 0x00
    jne _sleep
    ret

print_string:
    pop cx
    pop bx
    push cx
    mov ah, 0x0e
ps_inner:
    mov al, [bx]
    cmp al, 0
    je ps_end
    int 0x10
    inc bx
    jmp ps_inner
ps_end:
    ret

print_ln:
    mov al, 0xa             ; new line
    int 0x10
    mov al, 0xd             ; carriage return
    int 0x10
    ret

print_hex_8:
    pop cx
    pop dx
    push cx
    mov ah, 0x0e        ; TTY mode

    mov al, '0'
    int 0x10
    mov al, 'x'
    int 0x10

    mov al, dl          ; grab bottom byte
    shr al, 4           ; get the highest nibble

    mov bl, al          ; get char
    mov bh, 0x0
    add bx, hex_chars
    mov al, [bx]

    int 0x10

    mov al, dl          ; grab the bottom byte of the number
    and al, 0x0f        ; and get the lowest nibble
    
    mov bl, al          ; get char
    mov bh, 0x0
    add bx, hex_chars
    mov al, [bx]

    int 0x10            ; print
    ret

print_hex_16:
    pop cx
    pop dx
    push cx
    mov ah, 0x0e        ; TTY mode

    mov al, '0'
    int 0x10
    mov al, 'x'
    int 0x10

    mov al, dh          ; grab top byte
    shr al, 4           ; get the highest nibble

    mov bl, al          ; get char
    mov bh, 0x0
    add bx, hex_chars
    mov al, [bx]

    int 0x10

    mov al, dh          ; grab top byte
    and al, 0x0f        ; get the lowest nibble

    mov bl, al          ; get char
    mov bh, 0x0
    add bx, hex_chars
    mov al, [bx]

    int 0x10

    mov al, dl          ; grab bottom byte
    shr al, 4           ; get the highest nibble

    mov bl, al          ; get char
    mov bh, 0x0
    add bx, hex_chars
    mov al, [bx]

    int 0x10

    mov al, dl          ; grab the bottom byte of the number
    and al, 0x0f        ; and get the lowest nibble
    
    mov bl, al          ; get char
    mov bh, 0x0
    add bx, hex_chars
    mov al, [bx]

    int 0x10            ; print
    ret

hello_world:
    db "Hello, World!", 0x0a, 0x0d, 0
intro_string:
    db "here's what i look like:", 0xa, 0xd, 0
second_string:
    db "and here it is in hexadecimal:", 0xa, 0xd, 0
hex_chars:
    db "0123456789abcdef", 0

times 510-($-$$) db 0   ; fill with zeros
dw 0xaa55               ; define bootable signature byte
end: