# ---------------------------------------------------------------------------------------
# Tool for cracking many time pad. Tool used for Canyouhack.it Crack Challenge 8.
# In this problem we have 4 16-byte ciphertexts that have been encrypted with the same 
# key. We 'll use the crib drag attack as shown in [1]. After that the plaintexts are:
#       [1] There is no pass
#       [2] word. :D Here is
#       [3]  your flag: b4d_
#       [4] rc4_is_b4d!.....
#   
# So the password (flag) is: b4d_rc4_is_b4d!
#
# ispo - 22/11/2014
#
#
# [1]. http://travisdazell.blogspot.com/2012/11/many-time-pad-attack-crib-drag.html
# ---------------------------------------------------------------------------------------
import string

#cipher = [

cipher = [
    [0xC5,0x5D,0x32,0xCE,0xBA,0xC7,0x1C,0xA7,0xF3,0x59,0xAA,0x4B,0x8C,0xA4,0x3E,0x75],
    [0xE6,0x5A,0x25,0xD8,0xF1,0xC7,0x4F,0x90,0xD9,0x7F,0xA0,0x19,0x99,0xE5,0x24,0x75],
    [0xB1,0x4C,0x38,0xC9,0xAD,0xC7,0x13,0xB8,0xB2,0x50,0xFF,0x4B,0x9E,0xF1,0x29,0x59],
    [0xE3,0x56,0x63,0xE3,0xB6,0x94,0x2A,0xB6,0xE7,0x53,0xE4,0x61,0xF6,0xC5,0x4D,0x06]
]

plain = [ ['.']*16, ['.']*16, ['.']*16, ['.']*16 ]
# ---------------------------------------------------------------------------------------
def guess( word ):
    h = [None]*len(word)

    for l in range(0, len(word)):
         h[l] = ord(word[l])        
    
    return h
        
# ---------------------------------------------------------------------------------------
if __name__ == '__main__':

    while( 1 ):                                 # forever...
        word = raw_input('Give me a word: ')    # read word
        r    = [None]*len(word)
        h    = guess(word)
        x    = [None] * 16  
        nxt  = 0

        for i in range(0, 4):
            for j in range(i+1, 4):             # for each pair
                print 'Trying pair: ['+str(i+1)+'],['+str(j+1)+']'

                # XOR ciphers
                for k in range(0,16): x[k] = cipher[i][k] ^ cipher[j][k]

                for p in range(0, 16-len(word)+1):  # for each position
                    invalid = 0             

                    for s in range(0, len(word)):   # try the word
                        r[s] = x[p+s] ^ h[s]
                
                        if(r[s] >= ord('a') and r[s] <= ord('z')) or\
                          (r[s] >= ord('A') and r[s] <= ord('Z')) or\
                          (r[s] >= ord('0') and r[s] <= ord('9')) or\
                          (r[s] == ord(' ')) or\
                          (r[s] == ord(':')) or\
                          (r[s] == ord(';')) or\
                          (r[s] == ord('_')) or\
                          (r[s] == ord('!')) or\
                          (r[s] == ord(',')) or\
                          (r[s] == ord('.')) or\
                          (r[s] == ord('\n'))or\
                          (r[s] == ord('\0')):

                            invalid = 0
                        else:
                            invalid = 1
                            break
                        
                    if invalid is 0:
                        print 'Match found at ['+str(p)+ ']: ' + ''.join(chr(y) for y in r)
                        ans = raw_input('Use it on 1st or 2nd cipher [1/2/Esc]? ')

                        if ans == '1':
                            for g in range(0, len(word)):
                                plain[i][p+g] = word[g]         
                                plain[j][p+g] = chr(r[g])           

                            nxt = 1                         # go on the next character
                            break
                        elif ans =='2':
                            for g in range(0, len(word)):
                                plain[j][p+g] = word[g]         
                                plain[i][p+g] = chr(r[g])           

                            nxt = 1                         # go on the next character
                            break

                if nxt is 1: break
            if nxt is 1: break

        print '[1]', ''.join(str(y) for y in plain[0])  
        print '[2]', ''.join(str(y) for y in plain[1])  
        print '[3]', ''.join(str(y) for y in plain[2])  
        print '[4]', ''.join(str(y) for y in plain[3])  
                
# ---------------------------------------------------------------------------------------
''' 
Sample output:

    Give me a word: There 
    Trying pair: [1],[2]
    Match found at [0]: word. 
    Use it on 1st or 2nd cipher [1/2/Esc]? 1
    [1] There ..........
    [2] word. ..........
    [3] ................
    [4] ................
    Give me a word: There 
    Trying pair: [1],[2]
    Match found at [0]: word. 
    Use it on 1st or 2nd cipher [1/2/Esc]? 
    Match found at [7]: cBCx75
    Use it on 1st or 2nd cipher [1/2/Esc]? 
    Trying pair: [1],[3]
    Match found at [0]:  your 
    Use it on 1st or 2nd cipher [1/2/Esc]? 1
    [1] There ..........
    [2] word. ..........
    [3]  your ..........
    [4] ................
    Give me a word: There 
    Trying pair: [1],[2]
    Match found at [0]: word. 
    Use it on 1st or 2nd cipher [1/2/Esc]? 
    Match found at [7]: cBCx75
    Use it on 1st or 2nd cipher [1/2/Esc]? 
    Trying pair: [1],[3]
    Match found at [0]:  your 
    Use it on 1st or 2nd cipher [1/2/Esc]? 
    Match found at [5]: Tgz3lu
    Use it on 1st or 2nd cipher [1/2/Esc]? 
    Trying pair: [1],[4]
    Match found at [0]: rc4_is
    Use it on 1st or 2nd cipher [1/2/Esc]? 1
    [1] There ..........
    [2] word. ..........
    [3]  your ..........
    [4] rc4_is..........
    Give me a word:  pass
    Trying pair: [1],[2]
    Match found at [3]: 6;a D
    Use it on 1st or 2nd cipher [1/2/Esc]? 
    Match found at [4]: kp2DY
    Use it on 1st or 2nd cipher [1/2/Esc]? 
    Match found at [6]: sGKUy
    Use it on 1st or 2nd cipher [1/2/Esc]? 
    Match found at [11]: re is
    Use it on 1st or 2nd cipher [1/2/Esc]? 1
    [1] There ..... pass
    [2] word. .....re is
    [3]  your ..........
    [4] rc4_is..........
    Give me a word:  pass
    Trying pair: [1],[2]
    Match found at [3]: 6;a D
    Use it on 1st or 2nd cipher [1/2/Esc]? 
    Match found at [4]: kp2DY
    Use it on 1st or 2nd cipher [1/2/Esc]? 
    Match found at [6]: sGKUy
    Use it on 1st or 2nd cipher [1/2/Esc]? 
    Match found at [11]: re is
    Use it on 1st or 2nd cipher [1/2/Esc]?  
    Trying pair: [1],[3]
    Match found at [0]: Taktd
    Use it on 1st or 2nd cipher [1/2/Esc]? 
    Match found at [1]: 1zfds
    Use it on 1st or 2nd cipher [1/2/Esc]? 
    Match found at [4]: 7pnl2
    Use it on 1st or 2nd cipher [1/2/Esc]? 
    Match found at [8]: ay4sa
    Use it on 1st or 2nd cipher [1/2/Esc]? 
    Match found at [11]:  b4d_
    Use it on 1st or 2nd cipher [1/2/Esc]? 1
    [1] There ..... pass
    [2] word. .....re is
    [3]  your ..... b4d_
    [4] rc4_is..........

    Give me a word:  flag
    Trying pair: [1],[2]
    Match found at [6]: sQFGm
    Use it on 1st or 2nd cipher [1/2/Esc]? 
    Trying pair: [1],[3]
    Match found at [5]:  is n
    Use it on 1st or 2nd cipher [1/2/Esc]? 2
    [1] There is n. pass
    [2] word. .....re is
    [3]  your flag. b4d_
    [4] rc4_is..........
    Give me a word: flag
    Trying pair: [1],[2]
    Match found at [0]: Ekvq
    Use it on 1st or 2nd cipher [1/2/Esc]? 
    Trying pair: [1],[3]
    Match found at [6]: is n
    Use it on 1st or 2nd cipher [1/2/Esc]? 
    Trying pair: [1],[4]
    Trying pair: [2],[3]
    Match found at [6]: :D
    H
    Use it on 1st or 2nd cipher [1/2/Esc]? 1
    [1] There is n. pass
    [2] word. flag.re is
    [3]  your :D
    H. b4d_
    [4] rc4_is..........
    Give me a word: flag
    Give me a word: There is no pass
    Trying pair: [1],[2]
    Match found at [0]: word. :D
    Here is
    Use it on 1st or 2nd cipher [1/2/Esc]? 1
    [1] There is no pass
    [2] word. :D
    Here is
    [3]  your flag. b4d_
    [4] rc4_is..........
    Give me a word: There is no pass
    Trying pair: [1],[2]
    Match found at [0]: word. :D
    Here is
    Use it on 1st or 2nd cipher [1/2/Esc]? 
    Trying pair: [1],[3]
    Match found at [0]:  your flag: b4d_
    Use it on 1st or 2nd cipher [1/2/Esc]? 
    Trying pair: [1],[4]
    Match found at [0]: rc4_is_b4d!
    Use it on 1st or 2nd cipher [1/2/Esc]? 1
    [1] There is no pass
    [2] word. :D
    Here is
    [3]  your flag. b4d_
    [4] rc4_is_b4d!
''' 
# ---------------------------------------------------------------------------------------

