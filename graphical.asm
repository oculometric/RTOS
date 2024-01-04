[org 0x7c00]
top:

; setup stack pointer
mov bp, $+0x400
mov sp, bp

; set graphics to pixel based
mov ah, 0x00
mov al, 0x12
int 0x10

mov ah, 0x0c    ; pixel writing mode
mov al, 0x06    ; orange colour
mov bh, 0x00    ; page 0
mov cx, 0       ; x = 0
mov dx, 0       ; y = 0

int 0x10        ; write pixel

loop:
    mov ah, 0x00
    int 0x16        ; wait for key
    pusha
    ; write black block
    push 0x00
    push cx
    push 10
    push dx
    push 10
    call fill_block
    popa
check_right_arrow:
    cmp ax, 0x4d00  ; check for right arrow
    jne check_left_arrow
    add cx, 10      ; move right
check_left_arrow:
    cmp ax, 0x4b00  ; check for left arrow
    jne check_down_arrow
    sub cx, 10      ; move left
check_down_arrow:
    cmp ax, 0x5000  ; check for down arrow
    jne check_up_arrow
    add dx, 10      ; move down
check_up_arrow:
    cmp ax, 0x4800  ; check for up arrow
    jne done_check_keys
    sub dx, 10      ; move up
done_check_keys:
    ; write orange pixel
    pusha
    push 0x06
    push cx
    push 10
    push dx
    push 10
    call fill_block
    popa
    jmp loop

_fill_min:
    dw 0, 0
_fill_max:
    dw 0, 0
fill_block:                 ; stack should contain (top to bottom) y size, y origin, x size, x origin, colour
    pop ax

    pop bx
    pop dx
    mov [_fill_min+2], dx
    add bx, dx
    mov [_fill_max+2], bx

    pop bx
    pop cx
    mov [_fill_min], cx
    add bx, cx
    mov [_fill_max], bx

    pop bx

    push ax
    mov ax, bx
    mov ah, 0x0c
    mov bh, 0x00
_fill_loop:
    int 0x10                ; draw the pixel
    inc cx                  ; increment the x coord
    cmp cx, [_fill_max]     ; check for hitting zero
    jl _fill_loop
    mov cx, [_fill_min]
    inc dx                  ; increment y coord
    cmp dx, [_fill_max+2]   ; check for hitting zero
    jl _fill_loop
_fill_done:
    ret

times 510-($-$$) db 0   ; fill with zeros
dw 0xaa55               ; define bootable signature byte
end: