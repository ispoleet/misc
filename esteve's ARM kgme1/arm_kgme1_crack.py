#!/usr/bin/env python2
# --------------------------------------------------------------------------------------------------
# esteve's ARM kgme1 (Difficulty: 3)
#
# Key crack
# --------------------------------------------------------------------------------------------------
import struct
import sys


# --------------------------------------------------------------------------------------------------
# The original algorithm: key[2:6] are invariant in each iteration. Thus we can initialize them
# with arbitrary values and find the correct values for key[0] and key[1] that lead to a valid key.
#
def original_algo(key):
#   key = [0x2287ff52, 0x7c9d762c, 0x1337, 0xbeef, 0xcafe, 0x1590]
    A   = 0xc6ef3720
    B   = 0x9e3779b9

    for i in range(32):
        key[1] -= ((key[0] >> 5) + key[5]) ^ ((key[0] << 4) + key[4]) ^ (key[0] + A)
        key[1] &= 0xffffffff                    # truncate to 32 bits

        key[0] -= ((key[1] >> 5) + key[3]) ^ ((key[1] << 4) + key[2]) ^ (key[1] + A)
        key[0] &= 0xffffffff                    # truncate to 32 bits
        
        A -= B                                  # update A
        A  &= 0xffffffff


    # print 'Key:', ','.join(['%x' % k for k in key])
    if key[0] == 0xba01aafe and key[1] == 0xbbff31a3:
        print 'Congratulations!!'
        return True
    else:
        print 'Bad Key'
        return False


# --------------------------------------------------------------------------------------------------
if __name__ == "__main__":
    print 'Enter 4 comma separated hex numbers:',
    k = [int(n, 16) for n in raw_input().split(',')]

    # initialize key (final state)
    key = [0xba01aafe, 0xbbff31a3, k[0], k[1], k[2], k[3]]


    # pre-calculate A's and use them in reverse order (it's simpler)
    A, B = 0xc6ef3720, 0x9e3779b9
    A_list = [0xc6ef3720]                       # start with initial A
    for i in range(32):
        A = (A - B) & 0xffffffff
        A_list.append(A)
    
    # print 'A parameters:', ['%08x' % a for a in A_list]

    A_list = A_list[:-1]                        # last element is 0; drop it

    for i in range(32,0,-1):                    # move backwards
        A = A_list.pop(-1)                      # get next A

        # recover key[0] first
        key[0] += ((key[1] >> 5) + key[3]) ^ ((key[1] << 4) + key[2]) ^ (key[1] + A)        
        key[0] &= 0xffffffff
      
        # then recover key[1]
        key[1] += ((key[0] >> 5) + key[5]) ^ ((key[0] << 4) + key[4]) ^ (key[0] + A)
        key[1] &= 0xffffffff
        

    print 'Valid Key:', ','.join(['%x' % k for k in key])
    
    if not original_algo(key):
        raise Exception('Key was wrong! :(')

# --------------------------------------------------------------------------------------------------
'''
ispo@nogirl:~/reversing$ ./arm_kgme1_crack.py 
    Enter 4 comma separated hex numbers: 1,2,3,4
    Valid Key: 8989a381,3af9341e,1,2,3,4
    Congratulations!!
ispo@nogirl:~/reversing$ ./arm_kgme1_crack.py 
    Enter 4 comma separated hex numbers: beef,13371337,dead9999,ffffffff
    Valid Key: 4fa9cfd,5099c1e1,beef,13371337,dead9999,ffffffff
    Congratulations!!
'''
# --------------------------------------------------------------------------------------------------


