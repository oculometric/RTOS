[bits 32]
[extern interruptReintegrator]
section .text

interruptIndex:
    dd 0

[global interruptHandlerSize]
interruptHandlerSize:
    dd interruptHandlerASMEnd - interruptHandlerASM

; this macro will allow us to quickly define micro-ISRs, each of which does nothing more
; than find out which interrupt it represents, stash it, then pass it on to the real
; ISR, the interrupt aggregator below
%macro interruptHandler 1
align 4
interruptHandler%+%1:
    mov DWORD [interruptIndex], %1
    jmp interruptAggregator
%endmacro

; micro-ISR array starts here
; is this too janky? i feel like this is crossing a line
[global interruptHandlerASM]
align 4
interruptHandlerASM:
interruptHandler 0
align 4
interruptHandlerASMEnd:
interruptHandler 1
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
interruptHandler 32
interruptHandler 33
interruptHandler 34
interruptHandler 35
interruptHandler 36
interruptHandler 37
interruptHandler 38
interruptHandler 39
interruptHandler 40
interruptHandler 41
interruptHandler 42
interruptHandler 43
interruptHandler 44
interruptHandler 45
interruptHandler 46
interruptHandler 47
interruptHandler 48
interruptHandler 49
interruptHandler 50
interruptHandler 51
interruptHandler 52
interruptHandler 53
interruptHandler 54
interruptHandler 55
interruptHandler 56
interruptHandler 57
interruptHandler 58
interruptHandler 59
interruptHandler 60
interruptHandler 61
interruptHandler 62
interruptHandler 63
; FIXME: micro-ISRs only go up to 64. if we want to use all 256 interrupt vectors, this needs to be expanded (also see cpp file)

align 4
dd 0x4a6b7900

; this routine does the necessary stack and register protection, grabs the number
; of the interrupt currently being serviced, masks interrupts, then calls into the
; C function which handles dispatching the real ISRs back in C-land. it also
; cleans up after itself
interruptAggregator:
    cli     ; disable interrupts
    pushad  ; store registers
    cld     ; what?

    ; TODO: stash the rest, including floating point registers

    ; call the interrupt reintegrator in C
    push DWORD [interruptIndex]
    call interruptReintegrator
    pop eax ; clean up GCC's mess

    popad    ; restore registers
    sti     ; enable interrupts
    iret    ; interrupt return // FIXME: triple faulting (sometimes) after returning from an interrupt (i.e. after here)

    ; backtrace
    ; sometime after an interrupt occurs, a general protection fault (interrupt 0x0d) happens, with error code 0x0008
    ; this means that it relates to a selector, specifically in the GDT, index 1 (or possibly 8? unclear)
    ; then, for some reason, it can't handle this fault, and the relevant micro-ISR is never called (or while trying to call it, it fails)
    ; this leads to a double fault (interrupt 0x08), with error code zero, which also cannot be handled
    ; we end with a triple fault, obviously