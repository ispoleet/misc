#!/usr/bin/env python2
# --------------------------------------------------------------------------------------------------
# esteve's ARM kgme1 (Difficulty: 3)
#
# VM emulator
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

regs = [                                        # r0-r5 hold the input
    0x2287ff52, 0x7c9d762c, 0x1337, 0xbeef, 0xcafe, 0x1590, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
]

stack = [0x00]*0x400

# --------------------------------------------------------------------------------------------------
def f1(a1, a2, a3, regs):                       # mov.w a1, #a3a2
    regs[a1] = (regs[a1] & 0xffff0000) | a2 | (a3 << 8); 
    regs[31] += 4 
    return 12


# --------------------------------------------------------------------------------------------------
def f2(a1, a2, a3, regs):                       # mov.t a1, #a2a3
    regs[a1] = (((a2 << 8) | a3) << 16) | (regs[a1] & 0xffff)
    regs[31] += 4 
    return 12


# --------------------------------------------------------------------------------------------------
def f3(a1, a2, a3, regs):                       # add a1, a2, a3
    regs[a1] = (regs[a2] + regs[a3]) & 0xffffffff
    regs[31] += 4 
    return 12


# --------------------------------------------------------------------------------------------------
def f4(a1, a2, a3, regs):                       # cbz/cbnz, a1, #a2a3
    v16 = ((a2 << 8) | a3) & 0xffff
    if v16 & 0x8000: v16 = -(~v16+1 & 0xffff)   # 2's complement    

    if stack[(a1 & 0x7f)] == 1:
        if a1 & 0x80: regs[31] += 4
        else:         regs[31] += 4*v16
    else:
        if a1 & 0x80: regs[31] += 4*v16
        else:         regs[31] += 4

    return 12


# --------------------------------------------------------------------------------------------------
def f5(a1, a2, a3, regs):                       # sete stack[1], a1, a2
    stack[1] = 1 if regs[a1] == regs[a2] else 0    
    regs[31] += 4 
    return 12


# --------------------------------------------------------------------------------------------------
def f6(a1, a2, a3, regs):                       # push a1
    stack[ regs[30] ] = regs[a1]
    regs[30] += 1
    regs[31] += 4 
    return 12


# --------------------------------------------------------------------------------------------------
def f7(a1, a2, a3, regs):                       # pop a1
    regs[30] -= 1
    regs[a1] = stack[ regs[30] ]
    regs[31] += 4 
    return 12


# --------------------------------------------------------------------------------------------------
def f8(a1, a2, a3, regs):                       # sub a1, a2, a3
    regs[a1] = (regs[a2] - regs[a3]) & 0xffffffff
    regs[31] += 4
    return 12


# -------------------------------------------------------- ------------------------------------------
def f9(a1, a2, a3, regs):                       # shf a1, a2, a3 (a3 = direction) 
    if a3 != 1: regs[a1] <<= a2
    else:       regs[a1] >>= a2

    regs[a1] &= 0xffffffff
    regs[31] += 4
    return 12


# --------------------------------------------------------------------------------------------------
def f10(a1, a2, a3, regs):                      # xor a1, a2, a3
    regs[a1] = regs[a2] ^ regs[a3]
    regs[31] += 4
    return 12


# --------------------------------------------------------------------------------------------------
def f11(a1, a2, a3, regs):                      # nop
    regs[31] += 4
    return 12


# --------------------------------------------------------------------------------------------------
if __name__ == "__main__":
    regs[8]  = 0x0                              # r8 = 0
    regs[13] = 0x20                             # r13 = 32
    regs[30] = 0                                # r30 = stack pointer (sp)
    regs[31] = 0x8f00                           # r31 = program counter (pc)
    
    A = True

    while regs[31] != 0x8fc4:
        # decode instruction        
        insn = vm_program[(regs[31] - 0x8f00) >> 2]
        i, a1, a2, a3 = insn & 0xff, (insn >> 8) & 0xff, (insn >> 16) & 0xff, (insn >> 24)


        print '0x%X: ---> %02x %02x %02x %02x' % (regs[31], i, a1, a2, a3)

        # dispatch instruction
        { 0  : f1,
          1  : f2,
          2  : f3,
          3  : f4,
          4  : f5,
          5  : f6,
          6  : f7,
          7  : f8,
          8  : f9,
          9  : f10,
          10 : f11,
          11 : f11,
          99 : f11 }[i](a1, a2, a3, regs)


        # print registers
        for i in range(len(regs)):
            print '%08X' % regs[i],
            if (i+1) % 8 == 0:
                print

        print '-'*80

        # sys.stdin.read(1)
        

    # final check
    if regs[0] == 1: print 'Congratulations!!'
    else:            print 'Bad Key'

# --------------------------------------------------------------------------------------------------
'''
ispo@nogirl:~/reversing$ ./arm_kgme1_emulator.py 
0x8F00: ---> 00 06 20 37
2287FF52 7C9D762C 00001337 0000BEEF 0000CAFE 00001590 00003720 00000000
00000000 00000000 00000000 00000000 00000000 00000020 00000000 00000000
00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
00000000 00000000 00000000 00000000 00000000 00000000 00000000 00008F04
--------------------------------------------------------------------------------

0x8F04: ---> 00 07 b9 79
2287FF52 7C9D762C 00001337 0000BEEF 0000CAFE 00001590 00003720 000079B9
00000000 00000000 00000000 00000000 00000000 00000020 00000000 00000000
00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
00000000 00000000 00000000 00000000 00000000 00000000 00000000 00008F08
--------------------------------------------------------------------------------

[..... TRUNCATED FOR BREVITY .....]

--------------------------------------------------------------------------------
0x8FBC: ---> 63 02 01 01
00000001 00000001 BA01AAFE BBFF31A3 0000CAFE 00001590 00000000 9E3779B9
00000020 05E0B87C 5A36AB5C BFF32D67 E0253E47 00000020 00000001 00000000
00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
00000000 00000000 00000000 00000000 00000000 00000000 00000000 00008FC0
--------------------------------------------------------------------------------
0x8FC0: ---> 63 02 01 01
00000001 00000001 BA01AAFE BBFF31A3 0000CAFE 00001590 00000000 9E3779B9
00000020 05E0B87C 5A36AB5C BFF32D67 E0253E47 00000020 00000001 00000000
00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
00000000 00000000 00000000 00000000 00000000 00000000 00000000 00008FC4
--------------------------------------------------------------------------------
Congratulations!!
'''
# --------------------------------------------------------------------------------------------------


