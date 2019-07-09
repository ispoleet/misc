#!/usr/bin/env python2
# --------------------------------------------------------------------------------------------------
# esteve's ARM kgme1 (Difficulty: 3)
#
# VM disassembler
# --------------------------------------------------------------------------------------------------
import struct
import sys


vm_program = [
    0x37200600, 0x79B90700, 0xEFC60601, 0x379E0701, 0x01000005, 0x01010906, 0x01050908, 0x05090902,
    0x06000A02, 0x01030005, 0x01010B06, 0x00040B08, 0x040B0B02, 0x0A0B0C09, 0x090C0C09, 0x0C010107,
    0x09000105, 0x08080906, 0x01050908, 0x03090902, 0x06010A02, 0x04020105, 0x01010B06, 0x00040B08,
    0x020B0B02, 0x0A0B0C09, 0x090C0C09, 0x0C000007, 0x00010E00, 0x00000E01, 0x0E080802, 0x010D0804,
    0x07060607, 0xE3FF8103, 0xAAFE0200, 0x31A30300, 0x01BA0201, 0xFFBB0301, 0x00000204, 0x03008103,
    0x00010000, 0x00000001, 0x01010304, 0x03008103, 0x00010100, 0x00000101, 0x01010263, 0x01010263,
    0x01010263, 0x01010263
]

# --------------------------------------------------------------------------------------------------
if __name__ == "__main__":
    pc = 0x8f00                                 # initialize PC

    for insn in vm_program:                     # for each instruction
        opcode, op1, op2, op3 = insn & 0xff, (insn >> 8) & 0xff, (insn >> 16) & 0xff, (insn >> 24)


        # disassemble instruction
        if   opcode ==  0: dis = 'mov.w r%d, #%04Xh'    % (op1, (op3 << 8) | op2)
        elif opcode ==  1: dis = 'mov.t r%d, #%04Xh'    % (op1, (op2 << 8) | op3)
        elif opcode ==  2: dis = 'add   r%d, r%d, r%d'  % (op1, op2, op3)
        elif opcode ==  3: 
            v16 = ((op2 << 8) | op3) & 0xffff
            if v16 & 0x8000: v16 = -(~v16+1 & 0xffff)

            if op1 & 0x80: dis = 'cbz   S[%d], #%04Xh'  % (op1 & 0x7f, pc + 4*v16)
            else:          dis = 'cbnz  r%d, #%04Xh'    % (op1 & 0x7f, pc + 4*v16)

        elif opcode ==  4: dis = 'sete  S[1], r%d, r%d' % (op1, op2)
        elif opcode ==  5: dis = 'push  r%d'            % op1
        elif opcode ==  6: dis = 'pop   r%d'            % op1
        elif opcode ==  7: dis = 'sub   r%d, r%d, r%d'  % (op1, op2, op3)
        elif opcode ==  8: dis = 'ls%c   r%d, #%X'      % ('l' if op3 != 1 else 'r', op1, op2)
        elif opcode ==  9: dis = 'xor   r%d, r%d, r%d'  % (op1, op2, op3)
        elif opcode == 10: dis = 'nop'

        print '%4x: %s' % (pc, dis)             # print instruction
        pc += 4

# --------------------------------------------------------------------------------------------------
'''
ispo@nogirl:~/reversing$ ./arm_kgme1_disassemble.py 
8f00: mov.w r6, #3720h
8f04: mov.w r7, #79B9h
8f08: mov.t r6, #C6EFh
8f0c: mov.t r7, #9E37h
8f10: push  r0
8f14: pop   r9
8f18: lsr   r9, #5
8f1c: add   r9, r9, r5
8f20: add   r10, r0, r6
8f24: push  r0
8f28: pop   r11
8f2c: lsl   r11, #4
8f30: add   r11, r11, r4
8f34: xor   r12, r11, r10
8f38: xor   r12, r12, r9
8f3c: sub   r1, r1, r12
8f40: push  r1
8f44: pop   r9
8f48: lsr   r9, #5
8f4c: add   r9, r9, r3
8f50: add   r10, r1, r6
8f54: push  r1
8f58: pop   r11
8f5c: lsl   r11, #4
8f60: add   r11, r11, r2
8f64: xor   r12, r11, r10
8f68: xor   r12, r12, r9
8f6c: sub   r0, r0, r12
8f70: mov.w r14, #0001h
8f74: mov.t r14, #0000h
8f78: add   r8, r8, r14
8f7c: sete  S[1], r8, r13
8f80: sub   r6, r6, r7
8f84: cbz   S[1], #8F10h
8f88: mov.w r2, #AAFEh
8f8c: mov.w r3, #31A3h
8f90: mov.t r2, #BA01h
8f94: mov.t r3, #BBFFh
8f98: sete  S[1], r2, r0
8f9c: cbz   S[1], #8FA8h
8fa0: mov.w r0, #0001h
8fa4: mov.t r0, #0000h
8fa8: sete  S[1], r3, r1
8fac: cbz   S[1], #8FB8h
8fb0: mov.w r1, #0001h
8fb4: mov.t r1, #0000h
8fb8: mov.t r1, #0000h
8fbc: mov.t r1, #0000h
8fc0: mov.t r1, #0000h
8fc4: mov.t r1, #0000h
'''
# --------------------------------------------------------------------------------------------------


