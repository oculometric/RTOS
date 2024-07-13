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
microISR 64
microISR 65
microISR 66
microISR 67
microISR 68
microISR 69
microISR 70
microISR 71
microISR 72
microISR 73
microISR 74
microISR 75
microISR 76
microISR 77
microISR 78
microISR 79
microISR 80
microISR 81
microISR 82
microISR 83
microISR 84
microISR 85
microISR 86
microISR 87
microISR 88
microISR 89
microISR 90
microISR 91
microISR 92
microISR 93
microISR 94
microISR 95
microISR 96
microISR 97
microISR 98
microISR 99
microISR 100
microISR 101
microISR 102
microISR 103
microISR 104
microISR 105
microISR 106
microISR 107
microISR 108
microISR 109
microISR 110
microISR 111
microISR 112
microISR 113
microISR 114
microISR 115
microISR 116
microISR 117
microISR 118
microISR 119
microISR 120
microISR 121
microISR 122
microISR 123
microISR 124
microISR 125
microISR 126
microISR 127
microISR 128
microISR 129
microISR 130
microISR 131
microISR 132
microISR 133
microISR 134
microISR 135
microISR 136
microISR 137
microISR 138
microISR 139
microISR 140
microISR 141
microISR 142
microISR 143
microISR 144
microISR 145
microISR 146
microISR 147
microISR 148
microISR 149
microISR 150
microISR 151
microISR 152
microISR 153
microISR 154
microISR 155
microISR 156
microISR 157
microISR 158
microISR 159
microISR 160
microISR 161
microISR 162
microISR 163
microISR 164
microISR 165
microISR 166
microISR 167
microISR 168
microISR 169
microISR 170
microISR 171
microISR 172
microISR 173
microISR 174
microISR 175
microISR 176
microISR 177
microISR 178
microISR 179
microISR 180
microISR 181
microISR 182
microISR 183
microISR 184
microISR 185
microISR 186
microISR 187
microISR 188
microISR 189
microISR 190
microISR 191
microISR 192
microISR 193
microISR 194
microISR 195
microISR 196
microISR 197
microISR 198
microISR 199
microISR 200
microISR 201
microISR 202
microISR 203
microISR 204
microISR 205
microISR 206
microISR 207
microISR 208
microISR 209
microISR 210
microISR 211
microISR 212
microISR 213
microISR 214
microISR 215
microISR 216
microISR 217
microISR 218
microISR 219
microISR 220
microISR 221
microISR 222
microISR 223
microISR 224
microISR 225
microISR 226
microISR 227
microISR 228
microISR 229
microISR 230
microISR 231
microISR 232
microISR 233
microISR 234
microISR 235
microISR 236
microISR 237
microISR 238
microISR 239
microISR 240
microISR 241
microISR 242
microISR 243
microISR 244
microISR 245
microISR 246
microISR 247
microISR 248
microISR 249
microISR 250
microISR 251
microISR 252
microISR 253
microISR 254
microISR 255

[global microISRTable]
microISRTable:
%assign i 0 
%rep    256 
    dd microISR%+i
%assign i i+1
%endrep

; this routine does the necessary stack and register protection, grabs the number
; of the interrupt currently being serviced, masks interrupts, then calls into the
; C function which handles dispatching the real ISRs back in C-land. it also
; cleans up after itself
microISRAggregator:
    pushad  ; store registers
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