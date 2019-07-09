
# esteve's ARM kgme1
### Crack by ispo (25/05/2018)

# Description
```
ARM kgme1
---------
esteve


This is a Linux/ARM keygenme, the rules are simple:

- Write a keygen for it.

So no patching is allowed. As the platform is not very usual it can be tested on an emulated system. 

It has been tested successfully on maemo scratchbox.
```
___


# Solution

For this crackme we'll use IDA pro and a cute raspberry pi :)

### Reversing the binary

We start from main():

```Assembly
.text:00008CF8 MAIN_ENTRY_POINT_8CF8
.text:00008CF8     MOV             R12, SP
.text:00008CFC     STMFD           SP!, {R11,R12,LR,PC}
.text:00008D00     SUB             R11, R12, #4        ; r11 = frame pointer
.text:00008D04     SUB             SP, SP, #0xA8
.text:00008D08     STR             R0, [R11,#-0x10]    ; loc_10 = argc
.text:00008D0C     STR             R1, [R11,#-0x14]    ; loc_14 = argv
.text:00008D10     MOV             R3, #0
.text:00008D14     STR             R3, [R11,#-0x98]    ; loc_98 = 0
.text:00008D18     MOV             R3, #0
.text:00008D1C     STR             R3, [R11,#-0x9C]    ; loc_9c = 0
.text:00008D20     LDR             R0, =aEnterYourKey  ; "Enter your key: \n"
.text:00008D24     BL              printf
.text:00008D28     SUB             R3, R11, #0x7C
.text:00008D2C     MOV             R0, #0
.text:00008D30     MOV             R1, R3
.text:00008D34     MOV             R2, #0x64
.text:00008D38     BL              read                ; read(stdin, &key_7c, 100)
.text:00008D3C     MOV             R3, #0xC4
.text:00008D3C     MOV             R3, #0xC4
.text:00008D40     STR             R3, [R11,#-0x90]    ; loc_94.offset = 0xc4
.text:00008D44     MOV             R0, #0x400
.text:00008D48     BL              malloc
.text:00008D4C     STR             R0, [R11,#-0x8C]    ; loc_94.buf_2 = malloc(400)
.text:00008D50     MOV             R0, #0x80
.text:00008D54     BL              malloc
.text:00008D58     STR             R0, [R11,#-0x94]    ; loc_94.buf_1 = malloc(0x80)
.text:00008D5C     MOV             R0, #0x80
.text:00008D60     BL              malloc
.text:00008D64     STR             R0, [R11,#-0x88]    ; loc_94.buf_3= malloc(0x80)
.text:00008D68     MOV             R0, #0x400
.text:00008D6C     BL              malloc
.text:00008D70     STR             R0, [R11,#-0x84]    ; loc_94.buf_4 = malloc(0x400)
.text:00008D74     LDR             R2, [R11,#-0x84]
.text:00008D78     LDR             R3, =f1_8494        ; store function pointers in loc_94.buf_4
.text:00008D7C     STR             R3, [R2]
.text:00008D80     MOV             R2, #4
.text:00008D84     LDR             R3, [R11,#-0x84]
.text:00008D88     ADD             R2, R2, R3
.text:00008D8C     LDR             R3, =f2_8544
.text:00008D90     STR             R3, [R2]
.....
.text:00008E70     SUB             R0, R11, #0x7C      ; arg1: key_7c
.text:00008E74     LDR             R3, [R11,#-0x94]
.text:00008E78     ADD             R12, R3, #4
.text:00008E7C     LDR             R3, [R11,#-0x94]
.text:00008E80     ADD             R3, R3, #8
.text:00008E84     STR             R3, [SP]            ; arg5: &loc_94.buf_1[2]
.text:00008E88     LDR             R3, [R11,#-0x94]
.text:00008E8C     ADD             R3, R3, #0xC
.text:00008E90     STR             R3, [SP,#4]         ; arg6: &loc_94.buf_1[3]
.text:00008E94     LDR             R3, [R11,#-0x94]
.text:00008E98     ADD             R3, R3, #0x10
.text:00008E9C     STR             R3, [SP,#8]         ; arg7: &loc_94.buf_1[4]
.text:00008EA0     LDR             R3, [R11,#-0x94]
.text:00008EA4     ADD             R3, R3, #0x14
.text:00008EA8     STR             R3, [SP,#0xC]       ; arg8: &loc_94.buf_1[5]
.text:00008EAC     LDR             R1, =aXXXXXX        ; "%x,%x,%x,%x,%x,%x"
.text:00008EB0     LDR             R2, [R11,#-0x94]    ; arg3: &loc_94.buf_1[0]
.text:00008EB4     MOV             R3, R12             ; arg4: &loc_94.buf_1[1]
.text:00008EB8     BL              sscanf
.text:00008EBC     STR             R0, [R11,#-0xA0]    ; return value
.text:00008EC0     LDR             R3, [R11,#-0xA0]
.text:00008EC4     CMP             R3, #6              ; if can't read 6 hex numbers, abort
.text:00008EC8     BEQ             ARGS_OK_8ED0
.text:00008ECC     B               BAD_KEY_9008
.text:00008ED0
.text:00008ED0 ARGS_OK_8ED0                            ; CODE XREF: .text:00008EC8j
.text:00008ED0     MOV             R2, #0x20
.text:00008ED4     LDR             R3, [R11,#-0x94]
.text:00008ED8     ADD             R2, R2, R3
.text:00008EDC     MOV             R3, #0
.text:00008EE0     STR             R3, [R2]            ; loc_94.buf_1[8] = 0
.text:00008EE4     MOV             R2, #0x34
.text:00008EE8     LDR             R3, [R11,#-0x94]
.text:00008EEC     ADD             R2, R2, R3
.text:00008EF0     MOV             R3, #0x20
.text:00008EF4     STR             R3, [R2]            ; loc_94.buf_1[13] = 0x20
.text:00008EF8     SUB             R0, R11, #0x94      ; arg1: loc_94
.text:00008EFC     BL              dispatcher_8410
```

`main()` reads 6 comma separated numbers from stdin, in the form: "xx,xx,xx,xx,xx,xx". Then
it initializes a data structure at loc_94:

```C
struct loc_94 {
     uint32_t *buf_1;              // 0x80
     uint32_t offset = 0xc4;       //
     uint32_t *buf_2;              // 0x400
     uint32_t *buf_3;              // 0x80
     uint32_t *buf_4;              // 0x400
};
```

After that it calls dispatcher_8410:
```Assembly
.text:00008410 dispatcher_8410                         ; CODE XREF: .text:00008EFCp
.text:00008410     STMFD           SP!, {R0-R12}
.text:00008414     MOV             R10, R0             ; r10 = arg1: loc_94
.text:00008418     LDR             R0, [R10,#4]        ; r0 = loc_94.offset = 0xc4
.text:0000841C     ADD             R6, LR, R0          ; r6 = return address + 0xc4 = 0x8fc4
.text:00008420     LDR             R0, [R10]           ; r0 = loc_94
.text:00008424     STR             LR, [R0,#0x7C]      ; loc_94[31] = return address = 0x8f00
.text:00008428     LDR             R1, [R10,#8]
.text:0000842C     STR             R1, [R0,#0x78]      ; loc_94[30] = loc_9?
.text:00008430
.text:00008430 LOOP_8430                               ; CODE XREF: dispatcher_8410+78j
.text:00008430     LDR             R0, [R10]
.text:00008434     LDR             R4, [R0,#0x7C]
.text:00008438     LDR             R4, [R4]            ; r4 = *((uint32_t*)loc_94.buf_1[31]) = A
.text:0000843C     AND             R3, R4, #0xFF       ; r3 = A & 0xff = i
.text:00008440     LDR             R0, [R10,#0x10]
.text:00008444     ADD             R3, R0, R3,LSL#2    ; r3 = &loc_94.buf_4[i]
.text:00008448     MOV             R0, R4,ASR#8
.text:0000844C     AND             R0, R0, #0xFF       ; arg1: (A >> 8) & 0xff
.text:00008450     MOV             R1, R4,ASR#16
.text:00008454     AND             R1, R1, #0xFF       ; arg2: (A >> 16) & 0xff
.text:00008458     MOV             R2, R4,ASR#24
.text:0000845C     AND             R2, R2, #0xFF       ; arg3:  (A >>24) & 0xff
.text:00008460     LDR             R12, [R3]           ; r12 = loc_94.buf_4[i] = pick a function from table
.text:00008464     MOV             R3, R10             ; arg4: loc_94
.text:00008468     STMFD           SP!, {LR}           ; restore lr
.text:0000846C     MOV             LR, PC
.text:00008470     ADD             LR, LR, #4          ; lr = 0x8478
.text:00008474     MOV             PC, R12             ; jump to loc_94.buf_4[i]
.text:00008478     LDMFD           SP!, {LR}           ; return here
.text:0000847C     LDR             R0, [R10]
.text:00008480     LDR             R4, [R0,#0x7C]
.text:00008484     CMP             R4, R6              ; if loc_94.buf_1[31] != 0x8fc4 continue
.text:00008488     BNE             LOOP_8430
.text:0000848C     LDMFD           SP!, {R0-R12}
.text:00008490     B               final_8FC8
```

This is a tricky one. Here, program reads a 32-bit number from `0x8f00`. It breaks it into 4 bytes
and uses the LSB as an index to invoke a function from `buf_4`. The rest 3 bytes are passed as arguments
to that function along with `buf_1`. When function returns, `buf_1[31]`, indicates the next 32-bit number
to be read and this loop continues until0x8fc4 we read from `0x8fc4`. This looks like a VM routine.
At the end `final_8FC8` is invoked to verify the result:

```Assembly
.text:00008FC8 final_8FC8                              ; CODE XREF: dispatcher_8410+80p
.text:00008FC8     ANDEQ           R0, R0, R0
.text:00008FCC     LDR             R3, [R11,#-0x94]
.text:00008FD0     LDR             R3, [R3]
.text:00008FD4     CMP             R3, #1               ; loc_94.buf_1[0] must be 1
.text:00008FD8     BNE             BAD_KEY_9008
.text:00008FDC     MOV             R2, #4
.text:00008FE0     LDR             R3, [R11,#-0x94]
.text:00008FE4     ADD             R3, R2, R3
.text:00008FE8     LDR             R3, [R3]
.text:00008FEC     CMP             R3, #1
.text:00008FF0     BNE             BAD_KEY_9008
.text:00008FF4     LDR             R0, =aCongratulation ; "Congratulations!!\n"
.text:00008FF8     BL              printf
.text:00008FFC     MOV             R3, #0
.text:00009000     STR             R3, [R11,#-0xA4]
.text:00009004     B               END_9018
.text:00009008
.text:00009008 BAD_KEY_9008                            ; CODE XREF: .text:00008ECCj
.text:00009008                                         ; final_8FC8+10j ...
.text:00009008     LDR             R0, =aBadKey        ; "Bad Key\n"
.text:0000900C     BL              printf
.text:00009010     MOV             R3, #1
.text:00009014     STR             R3, [R11,#-0xA4]
.text:00009018
.text:00009018 END_9018                                ; CODE XREF: final_8FC8+3Cj
.text:00009018     LDR             R0, [R11,#-0xA4]
.text:0000901C     SUB             SP, R11, #0xC
.text:00009020     LDMFD           SP, {R11,SP,PC}
```

To get the congratz message, buf_1[0] must be 1.


### Reversing the emulator

Clearly we're dealing with a VM crackme here. The virtual program starts at `0x8f00` up ot `0x8fc4`.
Here are the opcodes:

```
    0x37200600, 0x79B90700, 0xEFC60601, 0x379E0701, 0x01000005, 0x01010906, 0x01050908, 0x05090902,
    0x06000A02, 0x01030005, 0x01010B06, 0x00040B08, 0x040B0B02, 0x0A0B0C09, 0x090C0C09, 0x0C010107,
    0x09000105, 0x08080906, 0x01050908, 0x03090902, 0x06010A02, 0x04020105, 0x01010B06, 0x00040B08,
    0x020B0B02, 0x0A0B0C09, 0x090C0C09, 0x0C000007, 0x00010E00, 0x00000E01, 0x0E080802, 0x010D0804,
    0x07060607, 0xE3FF8103, 0xAAFE0200, 0x31A30300, 0x01BA0201, 0xFFBB0301, 0x00000204, 0x03008103,
    0x00010000, 0x00000001, 0x01010304, 0x03008103, 0x00010100, 0x00000101, 0x01010263, 0x01010263,
    0x01010263, 0x01010263
```

`buf_1` is the register array, `buf_3` is the program stack and `buf_4` is the dispatcher table. Upon 
startup, registers `r0-r5` are used to hold the 6 numbers from input. `r30` is used as the stack
pointer and `r31` as the program counter. Emulator contains 11 different instructions:
```Assembly
    .text:00009028 off_9028 DCD f1_8494                    ; DATA XREF: .text:00008D78r
    .text:0000902C off_902C DCD f2_8544                    ; DATA XREF: .text:00008D8Cr
    .text:00009030 off_9030 DCD f3_8BE4                    ; DATA XREF: .text:00008DA0r
    .text:00009034 off_9034 DCD f4_85F8                    ; DATA XREF: .text:00008DB4r
    .text:00009038 off_9038 DCD f5_8750                    ; DATA XREF: .text:00008DC8r
    .text:0000903C off_903C DCD f6_8820                    ; DATA XREF: .text:00008DDCr
    .text:00009040 off_9040 DCD f7_88E0                    ; DATA XREF: .text:00008DF0r
    .text:00009044 off_9044 DCD f8_8B34                    ; DATA XREF: .text:00008E04r
    .text:00009048 off_9048 DCD f9_89A0                    ; DATA XREF: .text:00008E18r
    .text:0000904C off_904C DCD f10_8A84                   ; DATA XREF: .text:00008E2Cr
    .text:00009050 off_9050 DCD f11_8C94                   ; DATA XREF: .text:00008E40r
```

Let's reverse `f2_8544`:
```Assembly
.text:00008544     MOV             R12, SP
.text:00008548     STMFD           SP!, {R11,R12,LR,PC}
.text:0000854C     SUB             R11, R12, #4
.text:00008550     SUB             SP, SP, #8
.text:00008554     STR             R3, [R11,#arg4_14]
.text:00008558     MOV             R3, R0
.text:0000855C     STRB            R3, [R11,#arg1_D]
.text:00008560     MOV             R3, R1
.text:00008564     STRB            R3, [R11,#arg2_E]
.text:00008568     MOV             R3, R2
.text:0000856C     STRB            R3, [R11,#arg3_F]
.text:00008570     LDR             R1, [R11,#arg4_14]
.text:00008574     LDRB            R3, [R11,#arg1_D]
.text:00008578     MOV             R2, R3,LSL#2        ; r2 = arg1 << 2
.text:0000857C     LDR             R3, [R1]
.text:00008580     ADD             R12, R2, R3         ; r12 = &loc_94.buf_1[arg1]
.text:00008584     LDRB            R2, [R11,#arg3_F]
.text:00008588     LDRB            R3, [R11,#arg2_E]
.text:0000858C     MOV             R3, R3,LSL#8
.text:00008590     ORR             R3, R2, R3          ; r3 = (arg2 << 8) | arg3
.text:00008594     MOV             R0, R3,LSL#16       ; r3 = ((arg2 << 8) | arg3) << 16
.text:00008598     LDR             R1, [R11,#arg4_14]
.text:0000859C     LDRB            R3, [R11,#arg1_D]
.text:000085A0     MOV             R2, R3,LSL#2
.text:000085A4     LDR             R3, [R1]
.text:000085A8     ADD             R3, R2, R3
.text:000085AC     LDR             R3, [R3]            ; r3 = loc_94.buf_1[arg1]
.text:000085B0     MOV             R3, R3,LSL#16
.text:000085B4     MOV             R3, R3,LSR#16       ; r3 = loc_94.buf_1[arg1] & 0xffff
.text:000085B8     ORR             R3, R0, R3          ; r3 = (((arg2 << 8) | arg3) << 16) | loc_94.buf_1[arg1] & 0xffff
.text:000085BC     STR             R3, [R12]           ; loc_94.buf_1[arg1] = r3
.text:000085C0     LDR             R3, [R11,#arg4_14]
.text:000085C4     MOV             R2, #0x7C
.text:000085C8     LDR             R3, [R3]
.text:000085CC     ADD             R1, R2, R3          ; r1 = &loc_94.buf_1[31]
.text:000085D0     LDR             R3, [R11,#arg4_14]
.text:000085D4     MOV             R2, #0x7C
.text:000085D8     LDR             R3, [R3]
.text:000085DC     ADD             R3, R2, R3
.text:000085E0     LDR             R3, [R3]            ; r3 = loc_94.buf_1[31] (iterator)
.text:000085E4     ADD             R3, R3, #4
.text:000085E8     STR             R3, [R1]            ; loc_94.buf_1[31] += 4
.text:000085EC     MOV             R0, #0xC            ; return 0xc
.text:000085F0     SUB             SP, R11, #0xC
.text:000085F4     LDMFD           SP, {R11,SP,PC}
```

This function can be decompiled to the following:
```C
    regs[a1] = (regs[a1] & 0xffff0000) | a2 | (a3 << 8); 
    regs[31] += 4 
    return 12
```

As you can see this function sets the low 16-bits from a register (a1, a2, a3 are the 2nd, 3rd and
4th bytes from the instruction respectively). Let's reverse `f4_85F8`:
```Assembly
.text:000085F8     MOV             R12, SP
.text:000085FC     STMFD           SP!, {R11,R12,LR,PC}
.text:00008600     SUB             R11, R12, #4
.text:00008604     SUB             SP, SP, #0x10
.text:00008608     STR             R3, [R11,#arg4_14]
.text:0000860C     MOV             R3, R0
.text:00008610     STRB            R3, [R11,#arg1_D]
.text:00008614     MOV             R3, R1
.text:00008618     STRB            R3, [R11,#arg2_E]
.text:0000861C     MOV             R3, R2
.text:00008620     STRB            R3, [R11,#arg3_F]
.text:00008624     LDRB            R3, [R11,#arg3_F]
.text:00008628     MOV             R2, R3              ; r2 = arg3
.text:0000862C     LDRB            R3, [R11,#arg2_E]
.text:00008630     MOV             R3, R3,LSL#8
.text:00008634     ORR             R3, R2, R3          ; r3 = (arg2 << 8) | arg3
.text:00008638     STRH            R3, [R11,#var_16]   ; v16  = ((arg2 << 8) | arg3)
.text:0000863C     LDR             R1, [R11,#arg4_14]
.text:00008640     LDRB            R3, [R11,#arg1_D]
.text:00008644     AND             R3, R3, #0x7F
.text:00008648     MOV             R2, R3,LSL#2        ; r2 = (arg1 & 0x7f) << 2
.text:0000864C     LDR             R3, [R1,#0xC]
.text:00008650     ADD             R3, R2, R3
.text:00008654     LDR             R3, [R3]            ; r3 = loc_94.buf_3[(arg1 & 0x7f) << 2]
.text:00008658     CMP             R3, #1
.text:0000865C     BNE             NOT_ONE_86D4        ; loc_94.buf_3[(arg1 & 0x7f) << 2] == 1 ?
.text:00008660     LDRSB           R3, [R11,#arg1_D]
.text:00008664     CMP             R3, #0
.text:00008668     BGE             POSITIVE_869C
.text:0000866C     LDR             R3, [R11,#arg4_14]
.text:00008670     MOV             R2, #0x7C
.text:00008674     LDR             R3, [R3]
.text:00008678     ADD             R1, R2, R3
.text:0000867C     LDR             R3, [R11,#arg4_14]
.text:00008680     MOV             R2, #0x7C
.text:00008684     LDR             R3, [R3]
.text:00008688     ADD             R3, R2, R3
.text:0000868C     LDR             R3, [R3]
.text:00008690     ADD             R3, R3, #4
.text:00008694     STR             R3, [R1]            ; loc_94.buf_1[31] += 4
.text:00008698     B               END_8744
.text:0000869C
.text:0000869C POSITIVE_869C                           ; CODE XREF: f4_85F8+70j
.text:0000869C     LDR             R3, [R11,#arg4_14]
.text:000086A0     MOV             R2, #0x7C
.text:000086A4     LDR             R3, [R3]
.text:000086A8     ADD             R0, R2, R3
.text:000086AC     LDR             R3, [R11,#arg4_14]
.text:000086B0     MOV             R2, #0x7C
.text:000086B4     LDR             R3, [R3]
.text:000086B8     ADD             R1, R2, R3
.text:000086BC     LDRSH           R3, [R11,#var_16]
.text:000086C0     MOV             R2, R3,LSL#2
.text:000086C4     LDR             R3, [R1]
.text:000086C8     ADD             R3, R3, R2
.text:000086CC     STR             R3, [R0]            ; loc_94.buf_1[31] -= (((arg2 << 8) | arg3) << 16) << 2
.text:000086D0     B               END_8744
.text:000086D4
.text:000086D4 NOT_ONE_86D4                            ; CODE XREF: f4_85F8+64j
.text:000086D4     LDRSB           R3, [R11,#arg1_D]
.text:000086D8     CMP             R3, #0              ; arg1 >= 0?
.text:000086DC     BGE             POSITIVE_8718
.text:000086E0     LDR             R3, [R11,#arg4_14]  ; arg1 is negative
.text:000086E4     MOV             R2, #0x7C
.text:000086E8     LDR             R3, [R3]
.text:000086EC     ADD             R0, R2, R3          ; r0 = &loc_94.buf_1[31]
.text:000086F0     LDR             R3, [R11,#arg4_14]
.text:000086F4     MOV             R2, #0x7C
.text:000086F8     LDR             R3, [R3]
.text:000086FC     ADD             R1, R2, R3
.text:00008700     LDRSH           R3, [R11,#var_16]   ; r3 = ((arg2 << 8) | arg3)
.text:00008704     MOV             R2, R3,LSL#2        ; r3 = ((arg2 << 8) | arg3) << 2
.text:00008708     LDR             R3, [R1]
.text:0000870C     ADD             R3, R3, R2
.text:00008710     STR             R3, [R0]            ; loc_94.buf_1[31] -= (((arg2 << 8) | arg3) << 16) << 2
.text:00008714     B               END_8744
.text:00008718
.text:00008718 POSITIVE_8718                           ; CODE XREF: f4_85F8+E4j
.text:00008718     LDR             R3, [R11,#arg4_14]
.text:0000871C     MOV             R2, #0x7C
.text:00008720     LDR             R3, [R3]
.text:00008724     ADD             R1, R2, R3
.text:00008728     LDR             R3, [R11,#arg4_14]
.text:0000872C     MOV             R2, #0x7C
.text:00008730     LDR             R3, [R3]
.text:00008734     ADD             R3, R2, R3
.text:00008738     LDR             R3, [R3]
.text:0000873C     ADD             R3, R3, #4
.text:00008740     STR             R3, [R1]            ; loc_94.buf_1[31] += 4
.text:00008744
.text:00008744 END_8744                                ; CODE XREF: f4_85F8+A0j
.text:00008744                                         ; f4_85F8+D8j ...
.text:00008744     MOV             R0, #0xC
.text:00008748     SUB             SP, R11, #0xC
.text:0000874C     LDMFD           SP, {R11,SP,PC}
```

The decompiled version of this function is shown below:
```C
    v16 = ((a2 << 8) | a3) & 0xffff
    if v16 & 0x8000: v16 = -(~v16+1 & 0xffff)   # 2's complement    

    if stack[(a1 & 0x7f)] == 1:
        if a1 & 0x80: regs[31] += 4
        else:         regs[31] += 4*v16
    else:
        if a1 & 0x80: regs[31] += 4*v16
        else:         regs[31] += 4

    return 12
```

Recall that r31 is the program counter. Therefore, this is compare-and-branch instruction.
That is, the `cbz/cbnz` equivalent from ARM. The MSBit of the a1 indicates the inversion (taken/not
taken upon equality). The 2nd value from stack is compared against 1 and the jump is taken 
accordingly. Similarly we decompile the other functions (for more details plz take a look at 
[arm_kgme1_emulator.py](arm_kgme1_emulator.py) and [arm_kgme1_disassemble.py](arm_kgme1_emulator.py)):
```
    f1_8494   ---> mov.w
    f2_8544   ---> mov.t
    f3_8BE4   ---> add
    f4_85F8   ---> cbz/cbnz
    f5_8750   ---> sete
    f6_8820   ---> push
    f7_88E0   ---> pop
    f8_8B34   ---> sub
    f9_89A0   ---> lsl/lsr
    f10_8A84  ---> xor
    f11_8C94  ---> nop
```

### Reversing the emulated program
At this point, we have everything that we need to reverse the emulated program. By using the 
arm_kgme1_disassemble.py script, we can get a nice disassembly of the emulated program:

```Assembly
8f00: mov.w r6, #3720h                      ;
8f04: mov.w r7, #79B9h                      ;
8f08: mov.t r6, #C6EFh                      ; A = 0xc6ef3720
8f0c: mov.t r7, #9E37h                      ; B = 0x9e3779b9

8f10: LOOP:
8f10: push  r0                              ;
8f14: pop   r9                              ;
8f18: lsr   r9, #5                          ;        
8f1c: add   r9, r9, r5                      ; r9 = (key[0] >> 5) + key[5]
8f20: add   r10, r0, r6                     ; r10 = key[0] + A
8f24: push  r0                              ;
8f28: pop   r11                             ;
8f2c: lsl   r11, #4                         ;
8f30: add   r11, r11, r4                    ; r11 = (key[0] << 4) + key[4]
8f34: xor   r12, r11, r10                   ; 
8f38: xor   r12, r12, r9                    ; r12 = ((key[0] >> 5) + key[5]) ^ 
                                            ;       ((key[0] << 4) + key[4]) ^
                                            ;       (key[0] + A) = C
8f3c: sub   r1, r1, r12                     ; key[1] -= C
8f40: push  r1                              ;
8f44: pop   r9                              ;
8f48: lsr   r9, #5                          ;
8f4c: add   r9, r9, r3                      ; r9 = (key[1] >> 5) + key[3]
8f50: add   r10, r1, r6                     ; r10 = key[1] + A
8f54: push  r1                              ;
8f58: pop   r11                             ;
8f5c: lsl   r11, #4                         ;
8f60: add   r11, r11, r2                    ; r11 = (key[1] << 4) + key[2]
8f64: xor   r12, r11, r10                   ; 
8f68: xor   r12, r12, r9                    ; r12 = ((key[1] >> 5) + key[3]) ^
                                            ;       ((key[1] << 4) + key[2])
                                            ;       (key[1] + A) = D
8f6c: sub   r0, r0, r12                     ; key[0] -= D

8f70: mov.w r14, #0001h                     ;
8f74: mov.t r14, #0000h                     ; r14 = 1
8f78: add   r8, r8, r14                     ; r8++
8f7c: sete  S[1], r8, r13                   ; r8 == r13 (0x20) ?
8f80: sub   r6, r6, r7                      ; A -= B
8f84: cbz   S[1], #8F10h                    ; if r8 != 0x20 then goto LOOP

8f88: mov.w r2, #AAFEh                      ;
8f8c: mov.w r3, #31A3h                      ;
8f90: mov.t r2, #BA01h                      ; r2 = 0xba01aafe
8f94: mov.t r3, #BBFFh                      ; r3 = 0xbbff31a3
8f98: sete  S[1], r2, r0                    ; key[0] == 0xba01aafe ?
8f9c: cbz   S[1], #8FA8h                    ; 
8fa0: mov.w r0, #0001h                      ;
8fa4: mov.t r0, #0000h                      ; r0 = 1

8fa8: sete  S[1], r3, r1                    ;
8fac: cbz   S[1], #8FB8h                    ; if 0xbbff31a3 == key[1] then goto SKIP
8fb0: mov.w r1, #0001h                      ;
8fb4: mov.t r1, #0000h                      ; r1 = 1

8fb8: SKIP:
8fb8: mov.t r1, #0000h                      ;
8fbc: mov.t r1, #0000h                      ;
8fc0: mov.t r1, #0000h                      ; r1 = 0
8fc4: mov.t r1, #0000h
```

The decompiled version of the emulated program is shown below:
```C
    key = [0x2287ff52, 0x7c9d762c, 0x1337, 0xbeef, 0xcafe, 0x1590]
    A   = 0xc6ef3720
    B   = 0x9e3779b9

    for i in range(32):
        print '%x, %x,  %x' % (key[0], key[1], A)

        key[1] -= ((key[0] >> 5) + key[5]) ^ ((key[0] << 4) + key[4]) ^ (key[0] + A)
        key[1] &= 0xffffffff                    # truncate to 32 bits

        key[0] -= ((key[1] >> 5) + key[3]) ^ ((key[1] << 4) + key[2]) ^ (key[1] + A)
        key[0] &= 0xffffffff                    # truncate to 32 bits
        
        A -= B                                  # update A
        A  &= 0xffffffff


    if key[0] == 0xba01aafe and key[1] == 0xbbff31a3:
        print 'Congratulations!!'
    else:
        print 'Bad Key'

    print 'Key:', ','.join(['%x' % k for k in key])
```

As you can see, `r2`, `r3`, `r4` and `r5` are initialized by the user and stay invariant during the
whole execution. Thus, we can pick any random values from them and find the correct pair
r0-r1 that leads to the congratulations message.

Reversing the algorithm is straight-forward as you can see in 
[arm_kgme1_crack.py](arm_kgme1_crack.py). Below are some valid keys:

```
ispo@nogirl:~/reversing$ ./arm_kgme1_crack.py 
    Enter 4 comma separated hex numbers: 1,2,3,4
    Valid Key: 8989a381,3af9341e,1,2,3,4
    Congratulations!!

ispo@nogirl:~/reversing$ ./arm_kgme1_crack.py 
    Enter 4 comma separated hex numbers: beef,13371337,dead9999,ffffffff
    Valid Key: 4fa9cfd,5099c1e1,beef,13371337,dead9999,ffffffff
    Congratulations!!
```

We can verify that the following keys are valid:
```
pi@raspberrypi:~/reversing $ ./arm_kgme1 
    Enter your key: 
    8989a381,3af9341e,1,2,3,4
    Congratulations!!

pi@raspberrypi:~/reversing $ ./arm_kgme1 
    Enter your key: 
    4fa9cfd,5099c1e1,beef,13371337,dead9999,ffffffff
    Congratulations!!
```

That was all folks :)
Have a nice day!

___
