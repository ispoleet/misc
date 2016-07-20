## Athcon 2013 reverse engineering challenge
#### Authors: Kyriakos Economou & Nikolaos Tsapakis
___
That's one of the toughest programs I've ever cracked. I have attempted to solve this challenge during the 
contest but I failed to. Since then, it turned to be a personal challenge for me until today :)

Ok, let's start: We will be using IDA pro throughout the tutorial, because we need IDA scripting capabilities. 
This is a VM crackme. The emulated ISA is a very simplified version of classic x86. There are multiple VMs and 
program switches between them during execution. Function calls are made through sysenter instruction and all 
of them are included in the ntdll.dll. Finally there are many garbage instructions in the real program, 
as well as in the emulated program. (about 1 useful instruction every ~50).

___

### PART 1: Analysing the emulator
The real program starts from 00407730. I won't analyse how one can understand that this binary constitutes a 
VM crackme. There are very nice articles online about reversing VM crackmes. The emulated program starts at 
address 0x40cade. The 1st part of the emulator is the following:

```assembly
.data:0040779A 81 EC 00 08+sub     esp, 800h
.data:004077A0 E8 00 00 00+call    $+5
.data:004077A5 5F          pop     edi
.data:004077A6 83 EF 79    sub     edi, 79h                        ; edi = 0x40772C = &VIRTUAL_REGISTERS
.data:004077A9 EB 01       jmp     short loc_4077AC                ; edi = PROG_40CADE
.data:004077A9             ; ---------------------------------------------------------------------------
.data:004077AB 7E          db  7Eh ; ~                             ; confuse linear sweep disassemblers
.data:004077AC             ; ---------------------------------------------------------------------------
.data:004077AC             memcpy(0x40a074, eip, 15) --> Fetch the next 15 virtual opcodes
.data:004077AC
.data:004077AC             loc_4077AC:                             ; CODE XREF: .data:004077A9j
.data:004077AC 8B 3F       mov     edi, [edi]                      ; edi = PROG_40CADE
[..... more instructions .....]
.data:004077DD EB DD       jmp     short loc_4077BC
.data:004077DF             ; ---------------------------------------------------------------------------
.data:004077DF             start decoding. Let INST[0:15] be the array with the opcodes
```

Code copies 15 bytes from the next location to the buffer 0x40a074 (INST). Then program decodes that opcodes
from INST buffer and not from the original location (we'll later why). The next step is:
```assembly
.data:004077DF             DECODE_INSTR_4077DF:                    ; CODE XREF: .data:004077DBj
.data:004077DF 8B EC       mov     ebp, esp
.data:004077E1 8B FE       mov     edi, esi
.data:004077E3 8B E7       mov     esp, edi
.data:004077E5 5B          pop     ebx							   ; ebx = INST[0:3]
.data:004077E6 8B E5       mov     esp, ebp
.data:004077E8 8A CB       mov     cl, bl
[..... more instructions .....]
.data:00407804 80 F3 8A    xor     bl, 8Ah						   ; bl ^ 0x8A = INST[0]
.data:00407807             -----------------------------------------------------------------------
.data:00407807             The OPCODE SWITCH starts here. Search until you find the right opcode
.data:00407807             We start with 2-byte opcode identification
```
At this point bl contains the first opcode from the emulated instruction (XORed with 0x8A). Then there's a 
big "switch" statement that tries to find the instruction corresponds to that opcode and jump to that 
location. The decoder uses the 1st byte (and sometimes the 2nd) for identifying the instruction. Decoder 
starts by checking 2 2-byte instructions. Then continues by checking only the 1st byte. Let's see a common
"case" statement:
```assembly
.data:00407894 53          push    ebx
.data:00407895 59          pop     ecx							   ; ecx = INST[0:3]
.data:00407897 80 F1 A5    xor     cl, 0A5h                        ; INST[0] ^ 0x8a == 0xa5 ?
.data:0040789A 9C          pushf
.data:0040789F 5A          pop     edx
.data:004078A1 83 E2 40    and     edx, 40h
.data:004078A4 0F 85 8A 2F+jnz     mov_al_imm_40A834               ; movb al, imm
```
(NOTE: Garbage instructions have been removed from the disassembly listings.)

Here, if the first byte of the opcode is 0x2f then we have a "mov byte al, imm_byte" instruction. Note that 
the above code is repeated for many different instructions. The only difference is the value that's XORed 
with cl. By following the jump we can see the emulated instruction:
```assembly
.data:0040A834             mov_al_imm_40A834:                      ; CODE XREF: .data:004078A4j
.data:0040A834 8B 07       mov     eax, [edi]                      ; mov byte al, imm_byte
.data:0040A836 40          inc     eax
.data:0040A837 8A 00       mov     al, [eax]
.data:0040A839 8B 57 DC    mov     edx, [edi-24h]
.data:0040A83C 8B 5F FC    mov     ebx, [edi-4]
.data:0040A83F 53          push    ebx
.data:0040A840 9D          popf
.data:0040A841 8A D0       mov     dl, al
.data:0040A843 9C          pushf
.data:0040A844 58          pop     eax
.data:0040A845 89 47 FC    mov     [edi-4], eax					   ; update eflags
.data:0040A848 89 57 DC    mov     [edi-24h], edx
.data:0040A84B 8B 07       mov     eax, [edi]
.data:0040A84D 83 C0 02    add     eax, 2						   ; eip += 2
.data:0040A850 89 07       mov     [edi], eax
.data:0040A852 E9 76 F8 FF+jmp     CMD_END_40A0CD
```
Some instructions many require 2 bytes for decoding. This means that after the first jump takes place, 
decoder uses the 2nd byte to find out the actual instruction. Lat's take a look at an example:
```assembly
.data:0040A709 53          push    ebx
.data:0040A70A 80 F7 01    xor     bh, 1
.data:0040A70D 5B          pop     ebx
.data:0040A70E 0F 84 15 15+jz      mov_eax_0_40BC29                ; mov eax, 0
.data:0040A714 53          push    ebx
.data:0040A715 80 F7 02    xor     bh, 2
.data:0040A718 5B          pop     ebx
.data:0040A719 0F 84 28 15+jz      mov_ebx_0_40BC47                ; mov ebx, 0
[..... more instructions .....]
```
Here we have to check the 2nd byte (remember ebx = INST[0:3]) to find out the correct instruction. By 
following the jumps we can reach the code for the emulated instruction.

The emulator has ~250 different instructions. It would be really boring if wrote them all, as many of them 
are very similar. However let's analyse few instructions:
```assembly
.data:0040BF78             push_eax_40BF78:                        ; CODE XREF: .data:00407C7Ej
.data:0040BF78 8B 47 F8    mov     eax, [edi-8]                    ; push eax
.data:0040BF7B 83 E8 04    sub     eax, 4
.data:0040BF7E 89 47 F8    mov     [edi-8], eax
.data:0040BF81 8B D0       mov     edx, eax
.data:0040BF83 8B 47 DC    mov     eax, [edi-24h]
.data:0040BF86 36 89 02    mov     ss:[edx], eax
.data:0040BF89 8B 07       mov     eax, [edi]
.data:0040BF8B 40          inc     eax
.data:0040BF8C 89 07       mov     [edi], eax
.data:0040BF8E E9 3A E1 FF+jmp     CMD_END_40A0CD
```
Here, address at [edi-8] is reduced by 4. Then value of [edi-24h] is written to the address [edi-4], and 
finally address of [edi] is increased by 1. What could this instruction? Ok let's assume that [edi-8] is esp, 
[edi-24h] is eax and [edi] is eip. Then our emulated instruction is: push eax!

Let's analyse one harder instruction:
```assembly
.data:0040CA4E             loope_offset_40CA4E:                    ; CODE XREF: .data:00407BEDj
.data:0040CA4E 8B 4F E4    mov     ecx, [edi-1Ch]                  ; loope
.data:0040CA51 66 49       dec     cx
.data:0040CA53 89 4F E4    mov     [edi-1Ch], ecx
.data:0040CA56 66 83 F9 00 cmp     cx, 0
.data:0040CA5A 74 32       jz      short loc_40CA8E
.data:0040CA5C 8B 47 FC    mov     eax, [edi-4]
.data:0040CA5F 83 E0 40    and     eax, 40h
.data:0040CA62 74 2A       jz      short loc_40CA8E
.data:0040CA64 8B 37       mov     esi, [edi]
.data:0040CA66 33 DB       xor     ebx, ebx
.data:0040CA68 8A 5E 01    mov     bl, [esi+1]
.data:0040CA6B 80 FB 80    cmp     bl, 80h
.data:0040CA6E 72 10       jb      short loc_40CA80
.data:0040CA70 8B 07       mov     eax, [edi]
.data:0040CA72 03 C3       add     eax, ebx
.data:0040CA74 2D FE 00 00+sub     eax, 0FEh
.data:0040CA79 89 07       mov     [edi], eax
.data:0040CA7B E9 4D D6 FF+jmp     CMD_END_40A0CD
.data:0040CA80             ; ---------------------------------------------------------------------------
.data:0040CA80             loc_40CA80:                             ; CODE XREF: .data:0040CA6Ej
.data:0040CA80 8B 07       mov     eax, [edi]
.data:0040CA82 03 C3       add     eax, ebx
.data:0040CA84 83 C0 02    add     eax, 2
.data:0040CA87 89 07       mov     [edi], eax
.data:0040CA89 E9 3F D6 FF+jmp     CMD_END_40A0CD
.data:0040CA8E             ; ---------------------------------------------------------------------------
.data:0040CA8E             loc_40CA8E:                             ; CODE XREF: .data:0040CA5Aj
.data:0040CA8E                                                     ; .data:0040CA62j
.data:0040CA8E 8B 07       mov     eax, [edi]
.data:0040CA90 83 C0 02    add     eax, 2
.data:0040CA93 89 07       mov     [edi], eax
.data:0040CA95 E9 33 D6 FF+jmp     CMD_END_40A0CD
.data:0040CA9A             ; ---------------------------------------------------------------------------
```
Here, [edi-1Ch] is reduced by 1. If becomes 0, [edi] (eip) is increased by 2. Otherwise we check the 7th bit
of [edi-4]. If it's 0, eip += 2. If not, we then have to read the 2nd byte from the instruction, and we add 
it to eip (if its >0x80 we subtract it). Can you guess  the command? Oh yeah, it's a "loope"! Clearly, 
[edi-1Ch] is ecx and [edi-4] is eflags. 

At the end of every instruction there's a jump to 0x40a0cd which is the code used to finalize the instruction:
```assembly
.data:0040A0CD             CMD_END_40A0CD:                         ; CODE XREF: .data:0040A80Aj
.data:0040A0CD                                                     ; .data:0040A82Fj ...
.data:0040A0CD 8B D8       mov     ebx, eax						   ; ebx = the next instruction address
.data:0040A0CF 83 E9 10    sub     ecx, 10h
.data:0040A0D2 83 EA 14    sub     edx, 14h
.data:0040A0D5 81 C4 00 04+add     esp, 400h
.data:0040A0DB 83 EE 18    sub     esi, 18h
.data:0040A0DE 81 C4 00 04+add     esp, 400h
.data:0040A0E4 83 EF 1C    sub     edi, 1Ch
.data:0040A0E7 E9 AE D6 FF+jmp     CMD_START_40779A
.data:0040A0EC             ; ---------------------------------------------------------------------------
```
The emulated commands can reveal the registers:
```assembly
.data:00407708             ***********************************************
.data:00407708                   Virtual Registers are stored here
.data:00407708             ***********************************************
.data:00407708 00 00 00 00 VIRTUAL_REGISTERS dd 0                  ; [edi - 0x24] = EAX
.data:0040770C 00 90 FD 7F dd 7FFD9000h                            ; [edi - 0x20] = EBX
.data:00407710 A6 0E 81 7C dd 7C810EA6h                            ; [edi - 0x1C] = ECX
.data:00407714 F4 E4 91 7C dd 7C91E4F4h                            ; [edi - 0x18] = EDX
.data:00407718 00 00 00 00 dd 0                                    ; [edi - 0x14] = ESI
.data:0040771C 00 00 00 00 dd 0                                    ; [edi - 0x10] = EDI
.data:00407720 F0 FF 12 00 dd 12FFF0h                              ; [edi - 0x0C] = EBP
.data:00407724 C0 FF 12 00 dd 12FFC0h                              ; [edi - 0x08] = ESP
.data:00407728 46 03 00 00 dd 346h                                 ; [edi - 0x04] = EFLAGS
.data:0040772C DE CA 40 00 dd 40CADEh                              ; [edi - 0x00] = EIP
```
Cool! After analysing each and every of the emulated instructions, we can put them all in a table:
```
+----+------------+-----------+------------------------+-------------------------------------------+
| ## | OPCODE LEN | INST. LEN | MNEMONIC               | COMMENTS                                  |
+----+------------+-----------+------------------------+-------------------------------------------+
| 01 |          2 |         2 | jmp eax                |                                           |
| 02 |          2 |         3 | shr eax, imm_byte      | 1 byte shift value                        |
| 03 |          2 |         2 | sysenter               |                                           |
| 04 |          2 |         6 | jxx address            | all conditional jumps                     |
| 05 |          1 |         2 | mov byte reg, imm_byte | reg = [al|bl|cl|dl]                       |
| 06 |          1 |         5 | mov reg, imm           |                                           |
| 07 |          1 |         2 | jxx offset             | 1b offset. Jump backward if offset>0x80   |
| 08 |          1 |         2 | loop/loope/loopne      |                                           |
| 09 |          1 |         5 | call                   | continue to a new VM                      |
| 10 |          1 |         5 | jmp address            |                                           |
| 11 |          1 |         2 | jmp offset             |                                           |
| 12 |          1 |         1 | push reg               |                                           |
| 13 |          1 |         1 | pop reg                |                                           |
| 14 |          1 |         1 | inc reg                |                                           |
| 15 |          1 |         1 | dec reg                |                                           |
| 16 |          1 |         5 | push dword             |                                           |
| 17 |          1 |         2 | push byte              | arithmetic byte to dword extension        |
| 18 |          2 |         2 | mov byte reg, 0        | reg = [al|bl|cl|dl]                       |
| 19 |          2 |         2 | mov reg, 0             | reg = [^esp]                              |
| 20 |          2 |         2 | xor eax, reg           | reg = [ebx|ecx|edx]                       |
| 21 |          2 |         2 | test byte reg, reg     | Compare the same reg. reg=[al|bl|cl|dl]   |
| 22 |          2 |         2 | test reg, reg          | Compare the same reg. reg = [^esp]        |
| 23 |          2 |         2 | mov reg, reg           | Registers are different                   |
| 24 |          2 |         2 | mov byte reg, reg      | Registers are different reg=[al|bl|cl|dl] |
| 25 |          2 |         2 | cmp reg, reg           | Registers are different                   |
| 26 |          2 |         2 | cmp byte reg, reg      | Registers are different reg=[al|bl|cl|dl] |
+----+------------+-----------+------------------------+-------------------------------------------+
```

The opcode length column shows how many bytes the decoder needs to examine in order to decode the instruction.
However, I have lied regarding instruction #18. Let's look at it:
```assembly
.data:0040BBBD             mov_al_0_40BBBD:                        ; CODE XREF: .data:0040A6DDj
.data:0040BBBD 8B 47 FC    mov     eax, [edi-4]                    ; mov al, 0
.data:0040BBC0 50          push    eax
.data:0040BBC1 9D          popf
.data:0040BBC2 B0 00       mov     al, 0
.data:0040BBC4 9C          pushf
.data:0040BBC5 5A          pop     edx
.data:0040BBC6 89 57 FC    mov     [edi-4], edx
.data:0040BBC9 89 47 DC    mov     [edi-24h], eax
.data:0040BBCC 8B 07       mov     eax, [edi]
.data:0040BBCE 83 C0 02    add     eax, 2
.data:0040BBD1 89 07       mov     [edi], eax
.data:0040BBD3 E9 F5 E4 FF+jmp     CMD_END_40A0CD
```
As you can see it's not actually "mov byte al, 0". Instruction loads eflags to eax, but before it clears the
LSByte. It's not really important, so let's leave it for now.

It will be very helpful if we have comments in every emulated instruction. But take a look at instruction 
\#23: "mov reg, reg". The only limitation is that registers must be different. We have 8 registers, so we'll 
have 7*8 = 56 different mov instructions. It's really tedious to set the right comments and breakpoints to 
every of these instructions. Now it's time for our first script. "mov" instructions start from 0x40B35C up 
to 0x40BA8F. The offset between 2 emulated "mov" instructions is 0x21 bytes. Fortunately for us, there's a
pattern on the instructions: The first 7 instructions are: "mov eax, reg". The next 7 are: "mov ebx, reg", 
and so on. The order of the registers is: eax, ebx, ecx, edx, ebp, esp, esi, edi. Thus we can find which
mov is emulated at every address. We can also do the same for "mov byte", "cmp" and "cmp byte"; The order
of the registers still remains the same (byte register order is: al, bl, cl, dl), but the addresses and 
the offsets are different.

Now it's time for our first script: make_instr.idc. Note that this step is required only if we want to 
implement the debugging approach (see below).

Unfortunately things can become even worse, because the emulator has the capability to execute an 
instruction outside of the virtual environment. Let's see how:
```assembly
.data:00408048 53          push    ebx
.data:00408049 8B 1F       mov     ebx, [edi]
.data:0040804B 81 C3 00 12+add     ebx, 1200h                      ; move 1200 bytes above eip
.data:00408051 8A 1B       mov     bl, [ebx]
.data:00408053 80 FB 01    cmp     bl, 1						   ; if byte is not 1, escape VM
.data:00408056 0F 85 E9 00+jnz     escape_VM_408145                ; escape VM!
```
That code exists in the middle of the decoder. Emulator jumps also to the address 0x408145 if decoder fails
to decode the 2nd byte of any instructions 18-26, or fails to decode the 1st byte. Let's decompile code from 
0x408145 to 0x408171: Code searches on array 0x407608 until it finds the INST[0] (the 1st byte of the 
instruction -without the 0x8a XOR). When it finds it, replaces INST[0] its index:
			INST[0] = foo, where ARRAY_407608[foo] = INST[0]

(that's why we backup the original opcodes to INST at the beginning of every instruction).
After this, there's a complex code, starting from 0x409210. Sometimes in reversing, you don't need to analyse
a piece of code; you can understand what's going on, only by looking the output. I didn't reverse that code. 
However I found that at some point, execution reaches 0x40a074 (INST)! Before that there's a small code at 
address 0x40A03B that loads the virtual registers to the physical ones. Later, at 0x40A083 program restores 
physical registers, back to virtual.

It's obvious now. Program executes the next instruction outside of the VM. The first byte of the instruction 
is hidden (code at 0x408048 reveals it) to prevent disassembly. Program executes only 1 instruction. However 
the buffer 0x40a074 is 15 bytes long. That's the purpose of the code from 0x409210 to 0x40A039: It identifies 
the instruction length, and adds a jump instruction to skip the unused bytes of the 0x40a074 buffer. 
Let's see an example:
```assembly
.data:0040A071 8B 7F F0    mov     edi, [edi-10h]                  ; execute the following instruction!
.data:0040A074 93          xchg    eax, ebx
.data:0040A075 EB 0C       jmp     short RESTORE_CTX_40A083
.data:0040A077             ; ---------------------------------------------------------------------------
.data:0040A077 51          push    ecx
.data:0040A078 5A          pop     edx
.data:0040A079 42          inc     edx
.data:0040A07A 56          push    esi
.data:0040A07B 5F          pop     edi
.data:0040A07C 47          inc     edi
.data:0040A07D 5E          pop     esi
.data:0040A07E 5B          pop     ebx
.data:0040A07F 5A          pop     edx
.data:0040A080 58          pop     eax
.data:0040A081 C9          leave
.data:0040A082 C3          retn
.data:0040A083             ; ---------------------------------------------------------------------------
.data:0040A083
.data:0040A083             RESTORE_CTX_40A083:                     ; CODE XREF: .data:0040A075j
.data:0040A083 57          push    edi
```
So far all good. But there's one piece missing from our puzzle: call instructions. Let's see code from 0x40C969:
```assembly
.data:0040C969
.data:0040C969             change_VM_loc_40C969:                   ; CODE XREF: .data:00407C27j
.data:0040C969 8B 37       mov     esi, [edi]                      ; change virtual emulator
.data:0040C96B 8B 46 01    mov     eax, [esi+1]                    ; eax = INST[1:4]
.data:0040C96E 03 F0       add     esi, eax
.data:0040C970 83 C6 05    add     esi, 5                          ; eip += 5 + INST[1:4]
.data:0040C973 E8 00 00 00+call    $+5
.data:0040C978 58          pop     eax
.data:0040C979 2D 74 53 00+sub     eax, 5374h                      ; 0x407604
.data:0040C97E 8B 00       mov     eax, [eax]
.data:0040C980 2B F0       sub     esi, eax                        ; eip += 5 + INST[1:4] - (DWORD*)0x407604
.data:0040C982 E8 00 00 00+call    $+5
.data:0040C987 58          pop     eax
.data:0040C988 83 C0 34    add     eax, 34h                        ; eax = 0x40C9BB
.data:0040C98B 8B 67 F8    mov     esp, [edi-8]
.data:0040C98E 50          push    eax                             ; return address of PHYSICAL_TO_VIRTUAL_REGS in stack
.data:0040C98F 56          push    esi                             ; address of the new emulator
.data:0040C990 83 07 05    add     dword ptr [edi], 5              ; veip += 5
.data:0040C993 8B 5F E4    mov     ebx, [edi-1Ch]                  ; load virtual to physical registers
[..... more instructions .....]
.data:0040C9BA C3          retn                                    ; start virtual machine 2
.data:0040C9BB             ; ---------------------------------------------------------------------------
.data:0040C9BB
.data:0040C9BB             PHYSICAL_TO_VIRTUAL_REGS:               ; load physical to virtual registers
.data:0040C9BB 57          push    edi
[..... more instructions .....]
.data:0040C9FD E9 98 AD FF+jmp     CMD_START_40779A
.data:0040CA02             ; ---------------------------------------------------------------------------
```
As you can see, virtual eip is increased by 5, while the starting address of the new emulator is calculated
by the following formula: eip += 5 + INST[1:4] - (DWORD*)0x407604. The good news are that the new emulator
is exactly the same with the main emulator. The only difference is a constant offset between the main 
emulator and the new emulator. We'll call this offset "vm_offst" in our scripts. 

Maybe we want to copy all of the comments and the breakpoints to the new emulator. copy_comm.idc will do this
job for you.

At this point we have finished with the analysis of the emulator(s). Now, we have everything we need to write 
a program that takes the emulated instructions as input and outputs their disassembly. But it's easier to understand 
a program if you can also debug it (we can watch the registers, the memory and the control flow). Definitely,
this program can be solved only with the disassembly listing.

Unfortunately, I got fooled by the difficulty (it said 4) and I decided to write a debugging script (because
it's easier). After a bit I realized that I also had to write the disassembly script. I will also present the 
debugging script, because it's very helpful and it's a method that rarely used when dealing with VM crackmes.

___
### PART 2: Disassembly emulated instructions
The main idea is very simple: Just do exactly what the emulator does, but instead of executing a command, print
it. It's important to use an IDA script because we have to disassembly real x86 instructions (remember, the 
emulator can execute 1 real instruction). By using function GetDisasm() we can get the mnemonic of any x86 
instruction.

There's one more problem. I mentioned before that I didn't analyse the code that finds the length of the real
instruction. We need the length in order to find where starts the next instruction starts in the emulated program. 
So, how can we get the instruction length? We'll do a very simple trick:

	1. MakeUnknown() 	--> Undefine all instructions
	2. MakeCode()		--> Return the length of the first instruction
	3. GetDisasm()		--> Get instruction mnemonic

File disasm.idc contains the code that performs the disassembly listing. We can get the disassembly listing
of any VM by setting the right offset to vm_offst (0 for main emulator). 

___
### PART 3: Debugging the emulated instructions

During analysis of the emulated instructions, I also set a breakpoint at the beginning of every emulated
instruction (either manually, or by script :P). I also set a breakpoint at the beginning of the decoder (this 
could be not needed). By running the program we can see every instruction of the emulated program. We can also 
inspect the stack, the registers and the memory. This is very useful, but it doesn't show the "big picture".
Our debugging script is debug.idc. As you can see it's much simpler than disassembly script.

The for loop on the main function contains some code to bypass the anti-debugging tricks. We'll discuss later
about the protections, and how one can bypass them.

I also set a breakpoint at the beginning of the decoder (this could be not needed). By running the program we can see every instruction of the emulated program. We can also 	404	instruction (either manually, or by script :P). I also set a breakpoint at the beginning of the decoder (may
399	inspect the stack, the registers and the memory. This is very useful, but it doesn't show the "big picture".

___
### PART 4: Reversing the emulated program

That's a good point to relax. We've finished with the hard part. Having the real disassembly listing, we can
see what really that program does. From the main file we can see calls to 4 other VMs:
	VM1 ==> main VM
	VM2 ==> VM at offset 0x18000
	VM3 ==> VM at offset 0x20000
	VM4 ==> VM at offset 0x8000
	VM5 ==> VM at offset 0x10000
	
I have put the code from all VMs (I added some comments) in the ~/vm_disasm directory. 

Ok let's start our analysis. The first important note is 2 calls to VM2. So, what VM2 does? Before we answer
that question, let's try to answer a more important question: Do we need VM2? Let's say that we don't need 
it (it's a lie! but we don't know it yet :P) and skip it. We skip it because when it gets executed, debugger
freezes. Go on.

Next we have some code:
```assembly
	[082]	40cb92	4	sub     dword ptr [ebp-14h], 4
	[083]	40cb96	4	sub     dword ptr [ebp-18h], 4
	.....
	[093]	40cbb1	2	mov esi, 0
	[094]	40cbb3	2	mov ebx, 0
	[095]	40cbb5	3		movzx   ebx, byte ptr [eax]
	[096]	40cbb8	3		rol     ebx, 7
	[097]	40cbbb	2		add     esi, ebx
	[098]	40cbbd	3		rol     esi, 7
	[099]	40cbc0	2		xor     esi, ebx
	[100]	40cbc2	1		inc eax
	[101]	40cbc3	3		cmp     byte ptr [eax], 0
	[102]	40cbc6	2		jz 02h (target: 40cbca)
	[103]	40cbc8	2	jmp -13h (target: 40cbb5)
	[104]	40cbca	2	mov eax, esi
	[105]	40cbcc	2	mul     esi
	[106]	40cbce	2	add     eax, edx
```

If I tell you that eax points to a string (break when *eax = 0), you'll tell me that loop gets a checksum
from that string. After this code tries to match the checksum:
```assembly
	[107]	40cbd0	5	cmp     eax, 946CE828h	
	[108]	40cbd5	2	jnz 15h (target: 40cbec)
	....
	[115]	40cbec	5	cmp     eax, 5F43B254h	
	[116]	40cbf1	2	jnz 15h (target: 40cc08)
	....
	[139]	40cc40	5	cmp     eax, 67F17733h
	[140]	40cc45	6	jnz FFFFFF47h (target: 40cb92)
```

If checksum doesn't match, then code executes the code again and again. Now, if I tell you that the string
that eax contains is a string from the exported function names of ntdll.dll?? Oh yeah it's pretty clear now what
is going on. Program iterates through all functions of ntdll.dll and for each one calculates a checksum. If code
matches the checksuum then the following code get executed:
```assembly
	[109]	40cbd7	5	call VM (401ADEh)	 (push    offset loc_41F730               ; initialize virtual registers (VM2))
	[110]	40cbdc	3	mov     eax, [ebp-1Ch]
	[111]	40cbdf	1	inc eax
	[112]	40cbe0	2	mov     eax, [eax]
	[113]	40cbe2	3	mov     [ebp-30h], eax
	[114]	40cbe5	7	mov     dword ptr [ebp-1Ch], 0
```
loc1C ([ebp-1Ch]) contains the entry point of the current function. Then it stores the 2nd byte, in local
variable (different for each checksum). This is a good point to write a new script (functions.py) to get
the real function names:
```
	('ZwUnmapViewOfSection', '0x946ce828L')
	('ZwMapViewOfSection', 	'0x5f43b254L')
	('ZwCreateSection', 	'0xa7afd948L')
	('ZwCreateFile', 		'0x848955acL')
	('ZwClose', 			'0x67f17733L')
```
Now we know the functions. Let's look inside to see what happens:
```
	ntdll.dll:7C91D090             ntdll_NtCreateFile proc near
	ntdll.dll:7C91D090 B8 25 00 00+mov     eax, 25h
	ntdll.dll:7C91D095 BA 00 03 FE+mov     edx, offset off_7FFE0300
	ntdll.dll:7C91D09A FF 12       call    dword ptr [edx]
	ntdll.dll:7C91D09C C2 2C 00    retn    2Ch
	ntdll.dll:7C91D09C             ntdll_NtCreateFile endp
```
Then 2nd byte is the service number in the system service table. Function stores service number in eax,
and calls ntdll_KiFastSystemCall:
```assembly
	ntdll.dll:7C91E4F0             ntdll_KiFastSystemCall proc near        ; DATA XREF: debug008:off_7FFE0300o
	ntdll.dll:7C91E4F0 8B D4       mov     edx, esp
	ntdll.dll:7C91E4F2 0F 34       sysenter
	ntdll.dll:7C91E4F4
	ntdll.dll:7C91E4F4             ntdll_KiFastSystemCallRet:
	ntdll.dll:7C91E4F4 C3          retn
	ntdll.dll:7C91E4F4             ntdll_KiFastSystemCall endp
```
That's how program makes the calls: through sysenter. Program searches for a function name based on its hash.
Once it finds the hash it stores the service number in a local variable. Every time that the program needs to call 
this function, it "simulates" the native call behaviour:
```assembly
	[185]	40cccf	3	lea     eax, [ebp-28h]							; eax service number
	[186]	40ccd2	2	mov     eax, [eax]
	[187]	40ccd4	2	mov edx, esp									; edx = user space stack
	[188]	40ccd6	2	sysenter										; ZwCreateFile() 
	[189]	40ccd8	2	mov     eax, [ebx]
```
It's cool, isn't it? :)
Searching in the function table is a really slow process. To save time we can bypass this loop, and set the
right service number to our local variables, in order to allow the execution of sysenter commands:
```
	PatchDword(0x12ffbc-0x30, 0x10b);
	PatchDword(0x12ffbc-0x20, 0x6c);
	PatchDword(0x12ffbc-0x24, 0x32);
	PatchDword(0x12ffbc-0x28, 0x25);
	PatchDword(0x12ffbc-0x34, 0x19);
```
Code is trying to open a file with name "g". After all it maps that file in the memory. Then we have:
```
	[250]	40cd8c	2	mov esi, eax									; esi points to file contents
```
	
This is an important point because it's the first time that the user's data start participating in the game.
Skipping all the dummy instruction and the ZWClose() calls, we see that program reads 4 dwords from the file.

Lines 0x40cf8f-0x40d0f9 check whether any of these bytes is NULL. If this is it the case, program jumps to the 
bad boy message. Lines 0x40d0ff-0x40d31d calculate a hash:
	Let KEY[0:15] be the input array
	Let VECTOR[0:15] be an array starting from address 0x4065f3
	Then:
        HASH = KEY[0:3] ^ KEY[4:7] ^ KEY[8:11] ^ KEY[12:15] ^
               VECTOR[0:3] ^ VECTOR[4:7] ^ VECTOR[8:11] ^ VECTOR[12:15] - 
			   0x4e1a9001
			   
Then code makes a call to VM4, and does the following check:
```
	[1105]	40d3b0	1	pop ebx											; ebx = 0xA640740E
	.....
	[1109]	40d3b7	2	cmp eax, ebx									; ret value from 0x401000 == 0xA640740E ?
	[1110]	40d3b9	2	jno 00h (target: 40d3bb)
	[1111]	40d3bb	2	jz 07h (target: 40d3c4)							; if not go to the bad boy message
	.....
	[1159]	40d410	1	pop eax
	[1160]	40d411	2	jmp eax											; jump to the good boy (0x40d44e)
```
If VM4 returns 0xA640740E, then we make a jmp to eax (where eax = HASH). What's the target address that we
want? It's 0x40d44e, because from this point starts the preparation for MessageBox call. Target address 
can't be after 0x40d44e, because MessageBox won't be called correctly. Furthermore, target address can't be
before 0x40d44e, because if we fall in range 0x40d413-0x40d449 we'll jump to bad boy message after a bit.

If we want to decrypt the goodboy/badboy messages we can do it through simple code:
```c
	for( i=0; i<0xE0; i = i+4)						// decrypt good boy message
	{
		j = Dword( 0x0040649C + i ) ^ 0x8456FD3A;
		
		Message("%c", j & 0xff );
		Message("%c", (j & 0xff00) >> 8 );
		Message("%c", (j & 0xff0000) >> 16 );
		Message("%c", (j & 0xff000000) >> 24 );
 	}
```
VM4 gets 3 inputs: HASH, 0xa0 and 0x406663, and returns a value that must be equal with 0xA640740E. The
first thing I did was to set eax to 0x40d44e and execute that function. The return value was the expected.
So, I didn't have to further analyse that function. 

The challenge of the crackme was to find a valid, not to fully understand every little detail of the program.
So, as long as I'm getting the expected value, I won't further analyse it.

___
### PART 5: Key-genning

Key-genning it's trivial now. We can produce zillions of valid keys. So let's set arbitrary values in 
KEY[0:11], and find the right value of KEY[12:15] such that HASH = 0x40d44e. Script keygen.idc can do 
this work for us.

I tried the program from the debugger, and I got the good boy message. I thought that I had finished, but
I did 1 last check, to test the "g" file outside of the debugger. Unfortunately I was getting the bod boy
message. What went wrong? It took me 2 days to understand what happened. Program worked fine while I 
debugged it, but not otherwise. The only difference was that I had disabled calls to VM2. A quick look
at VM2, revealed that it modifies VECTOR values. Oh that's a problem!

Remember that I told you that we don't need VM2? Well, we need it :( Let's look it more carefully. VM2 
makes periodically calls to KiGetTickCount() (through int 2Ah) to measure the clock ticks. If it doesn't detect
an increase in clock ticks or detects a large number of ticks between every interval, programs causes a 
segmentation fault by jumping to a null pointer:
```assembly
	[099]	424bed	2	int     2Ah										; KiGetTickCount()
	[100]	424bef	2	test eax, eax
	[101]	424bf1	2	jz 0Fh (target: 424c02)
	[102]	424bf3	3	cmp     eax, [ebp-64h]
	[103]	424bf6	2	jl 0Ah (target: 424c02)
	[104]	424bf8	3	sub     eax, [ebp-64h]
	[105]	424bfb	5	cmp     eax, 4E20h
	[106]	424c00	2	jle 38h (target: 424c3a)
	.....
	[114]	424c14	2		mov eax, 0
	.....
	[126]	424c38	2		jmp     edx									; cause a segmentation fault
	[127]	424c3a	1	pop edx
```
Hmmm, how we can deal with this trick? Simply we can make time "moving slower" in our debugging script:
```c
		/* Make time move slower :P */
		if( strstr(mnem, "int     2Ah") != -1 )			// call to KiGetTickCount() ?
		{
			StepOver();									// go the interrupt
			GetDebuggerEvent(WFNE_SUSP, -1);

			StepOver();									// execute it
			GetDebuggerEvent(WFNE_SUSP, -1);
			
			/* now, eax contains the clock ticks */
			
			EAX   = 0x300 + clock;						// set clock ticks to our own value
			clock = clock + 1;							// clock increases very slowly
			
			Message( "\t\teax = 0x%08x\n", EAX );		// print value
		}	
```
We actually patch the return value of KiGetTickCount() to make the program believe it that doesn't 
gets executed under a debugger.		
Then there's some code which starts from 0x424c3b that searches for the right functions based on their
hash, as main VM does. The functions are:
```
	('ZwAllocateVirtualMemory', 	'0x217a4264L')
	('ZwDelayExecution', 			'0x95aaf2e1L')
	('ZwFreeVirtualMemory', 		'0xe27847f7L')
	('ZwGetContextThread', 			'0xd2950638L')
	('ZwQueryInformationProcess', 	'0x8afa4d6dL')
	('ZwQueryVirtualMemory', 		'0x25f2995dL')
	('ZwSetInformationThread', 		'0x318a50b7L')
```
				 
However once we execute ZwSetInformationThread(), debugger freezes. At this we can stop investigating, or
we can take a deeper loop. A quick google search at "ZwSetInformationThread antidebug" brings us some useful
information:
	"When called with ThreadInformationClass set to 0x11 (ThreadHideFromDebugger constant), the thread 
	will be detached from the debugger."
See this line (0x424ce4)?
```assembly
	[190]	424ce4	2	push 11h										; arg3: ThreadHideFromDebugger
	[191]	424ce6	2	push FEh										; arg2
	[192]	424ce8	5	call VM (402CACh)	 (push    427730h                         ; GO TO VM3)
	[193]	424ced	3	add     eax, 0Dh
	[194]	424cf0	2	push 00h										; arg1
	[195]	424cf2	1	push eax
	[196]	424cf3	3	mov     eax, [ebp-60h]
	[197]	424cf6	2	mov edx, esp
	[198]	424cf8	2	sysenter										; call ZwSetInformationThread()
```
Let's stop the analysis of VM2 here. There may be more anti-debug tricks, but who cares?. The only information
that I want from VM2 is the difference in VECTOR values before and after the call. VECTOR is changing many
times inside VM2.

My first approach was to find the exact values that added on VECTOR dwords, every time VM2 is called, and 
find how many times VM2 called  before VECTOR is used. This didn't work because, I missed something (there
are many many calls). We're so close to the solution... Let's find another way to do it: hooking.

The idea is the following: we'll add a huge loop (with virtual instructions) before the values get XORed.
Then we'll run the program outside of the debugger. We'll have enough time then to attach a debugger to
the running process and read the right values. Let's start. Look at the following code:
```assembly
	[833]	40d1e3	6	add     ecx, 100h
	[834]	40d1e9	2	mov edi, ebx
	[835]	40d1eb	1	dec ecx
	[836]	40d1ec	2	loop -03h (target: 40d1e9)
```
Change the value that's going to be added on ecx from 0x100 to 0x7fffffff. That's it! Attach the debugger
and read the first 2 values:
```
	VECTOR[0:3] = 0xc3ec8a62
	VECTOR[4:7] = 0x4292f007
```
But before we do the XOR with VECTOR[8:15] there's another call to VM2:
```
	[897]	40d24e	5	call VM (401ADEh)
```
Let's go somewhere a bit after 0x40d24e and overwrite this code (remember to pad the code with 1 byte 
instructions like "inc eax"):
```
	mov ecx, 7fffffffh
		ja 00
	loop -02h
```
The virtual opcodes are: 7f ff ff ff 7f 51 00 96 fc. After patching the program, we can finally get the 
other 2 values:
```
	VECTOR[8:11]  = 0xe9e6474e 
	VECTOR[12:15] = 0x55ca2c39
```
Ok now we've finished! Let's produce some valid "g" files:
```
	EF BE AD DE 37 13 37 13 EF BE AD DE 6A 66 3E 60
	78 56 34 12 F0 DE BC 9A EF BE AD DE 3A 43 2C 25
```
___	
### PART 6: Conclusion

That was a pretty tough crackme :|. Maybe I didn't analyse and/or understand 100% the code. I can't say that
I enjoyed it, because it was very painful for me. However I learned many things during this process. A big
thanks to the author for this really well-built and designed challenge.

(I also like to thanks my friend Nick for correcting many syntax errors in this report :P)

Have a nice day!
Bye bye :)
___