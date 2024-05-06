[bits 32]
[extern interruptReintegrator]
section .text

; this macro will allow us to quickly define micro-ISRs, each of which does nothing more
; than find out which interrupt it represents, stash it, then pass it on to the real
; ISR, the interrupt aggregator below
%macro microISR 1
align 4
microISR%1:
    push DWORD 0
    push DWORD %1
    jmp microISRAggregator
%endmacro

%macro microEISR 1
align 4
microISR%1:
    push DWORD %1
    jmp microISRAggregator
%endmacro

; micro-ISR array starts here
microISR 0
microISR 1
microISR 2
microISR 3
microISR 4
microISR 5
microISR 6
microISR 7
microEISR 8
microISR 9
microEISR 10
microEISR 11
microEISR 12
microEISR 13
microEISR 14
microISR 15
microISR 16
microEISR 17
microISR 18
microISR 19
microISR 20
microEISR 21
microISR 22
microISR 23
microISR 24
microISR 25
microISR 26
microISR 27
microISR 28
microEISR 29
microEISR 30
microISR 31
microISR 32
microISR 33
microISR 34
microISR 35
microISR 36
microISR 37
microISR 38
microISR 39
microISR 40
microISR 41
microISR 42
microISR 43
microISR 44
microISR 45
microISR 46
microISR 47
microISR 48
microISR 49
microISR 50
microISR 51
microISR 52
microISR 53
microISR 54
microISR 55
microISR 56
microISR 57
microISR 58
microISR 59
microISR 60
microISR 61
microISR 62
microISR 63
; FIXME: micro-ISRs only go up to 64. if we want to use all 256 interrupt vectors, this needs to be expanded (also see cpp file)

[global microISRTable]
microISRTable:
%assign i 0 
%rep    64 
    dd microISR%+i
%assign i i+1
%endrep

; this routine does the necessary stack and register protection, grabs the number
; of the interrupt currently being serviced, masks interrupts, then calls into the
; C function which handles dispatching the real ISRs back in C-land. it also
; cleans up after itself
microISRAggregator:
    pushad  ; store registers ; ESP is 0x84cc before this line
    cld     ; puts us in the right calling convention, or something

    ;push DWORD ds
    ;push DWORD es
    ;push DWORD fs
    ;push DWORD gs
    ; TODO: stash the rest, including floating point registers

    ; we grab the interrupt index from halfway down the stack, and push it again
    push DWORD [esp + 32] 
    ; call the interrupt reintegrator in C
    call interruptReintegrator
    pop eax

freeze:
    ;pop gs
    ;pop fs
    ;pop es
    ;pop ds

    popad    ; restore registers

    add esp, 8 ; skip down the stack

    iret    ; interrupt return