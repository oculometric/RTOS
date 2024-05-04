[bits 32]
[extern interruptReintegrator]
section .text

interruptIndex:
    dd 0

[global interruptHandlerSize]
interruptHandlerSize:
    dd interruptHandlerASMEnd - interruptHandlerASM

%macro interruptHandler 1
align 4
    mov DWORD [interruptIndex], %1
    jmp interruptAggregator
%endmacro

; is this too janky? i feel like this is crossing a line
align 4
[global interruptHandlerASM]
interruptHandlerASM:
    mov DWORD [interruptIndex], 0
    jmp interruptAggregator
align 4
interruptHandlerASMEnd:
interruptHandler 2
interruptHandler 3
interruptHandler 4
interruptHandler 5
interruptHandler 6
interruptHandler 7
interruptHandler 8
interruptHandler 9
interruptHandler 10
interruptHandler 11
interruptHandler 12
interruptHandler 13
interruptHandler 14
interruptHandler 15
interruptHandler 16
interruptHandler 17
interruptHandler 18
interruptHandler 19
interruptHandler 20
interruptHandler 21
interruptHandler 22
interruptHandler 23
interruptHandler 24
interruptHandler 25
interruptHandler 26
interruptHandler 27
interruptHandler 28
interruptHandler 29
interruptHandler 30
interruptHandler 31

interruptAggregator:
    cli
    pusha
    cld

    ; TODO: stash the rest, including floating point registers

    ; call the interrupt reintegrator in C
    push DWORD [interruptIndex]
    call interruptReintegrator
    pop eax
    pop eax

    popa
    sti
    iret