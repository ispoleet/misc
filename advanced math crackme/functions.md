## Advanced Math Crackme
##### October 2013

This file contains all functions used by main code of this crackme. 
___
```assembly
// ============================================================================================= //
//                                       keygenme.00401828                                       //
//                                          copy_name()                                          //
// ============================================================================================= //
	00401828  to 0040182D:									=> prolog
	0040182E  MOV EBX,DWORD PTR SS:[EBP+C]					=> ebx = strlen(name) = len
	00401831  MOV ESI,DWORD PTR SS:[EBP+8]					=> esi = &name		
	00401834  JMP SHORT keygenme.00401867					=>
  /-00401836  MOV EAX,DWORD PTR DS:[410764]					=> eax = 0 (from keygenme.0040180C)
  | 0040183B  MOV EDX,10									=>
  | 00401840  SUB EDX,EAX									=>  
  | 00401842  CMP EBX,EDX									=> 
  | 00401844  JNB SHORT keygenme.00401848					=>
  | 00401846  MOV EDX,EBX									=> edx = (ebx > 16) ? 16 : ebx
  | 00401848  LEA EDI,DWORD PTR DS:[EAX+410730]				=>
  | 0040184E  MOV ECX,EDX									=> 
  | 00401850  REP MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI] 	=> memcpy(&h[16],name,edx)
  | 00401852  SUB EBX,EDX									=> ebx -= edx 
  | 00401854  ADD EAX,EDX									=> eax += edx 
  | 00401856  CMP EAX,10									=> eax == 16 ?
  | 00401859  JNZ SHORT keygenme.00401862					=> if yes call shuffle()
  | 0040185B  CALL keygenme.004016F0						=>
  | 00401860  XOR EAX,EAX									=> eax = 0
  | 00401862  MOV DWORD PTR DS:[410764],EAX 				=> 
  | 00401867  OR EBX,EBX									=> ebx == 0 ?
  \-00401869  JNZ SHORT keygenme.00401836					=> if yes break
	0040186B to 0040186E									=> epilog
	0040186F  RETN 8										=> POP args from stack (OFFSET = 0x0)
```
```c
	void copy_name( const char *name, const int len )
	{
		int ebx, edx;

		for( ebx=len; ebx!=0;  ebx -= edx)
		{
			edx = (ebx > 16) ? 16 : ebx;
			memcpy(&h[16],name,edx);

			if( edx == 16 ) { shuffle(); I(&h[0x44]) = 0; }
			else I(&h[0x44]) = edx;

			name += 16;
		}
	}
```
___
```assembly	
// ============================================================================================= //
//                                       keygenme.00401874                                       //
//                                         pre_shuffle()                                         //
// ============================================================================================= //
	00401874  PUSH ESI										=>
	00401875  PUSH EDI										=> backup registers
	00401876  MOV EAX,DWORD PTR DS:[410764]					=> eax = v_410764
	0040187B  MOV EDX,10									=> 
	00401880  SUB EDX,EAX									=> 
	00401882  LEA EDI,DWORD PTR DS:[EAX+410730]				=> edi = &h[16 + v_410764]
	00401888  AND EDX,0FF									=> edx = (16 - v_410764) & 0xff
	0040188E  MOV EAX,EDX									=> eax = (16 - v_410764) & 0xff
	00401890  MOV ECX,EDX									=> ecx = (16 - v_410764) & 0xff
	00401892  REP STOS BYTE PTR ES:[EDI]					=> expand name with name len to 16 bytes
	00401894  CALL keygenme.004016F0						=> call shuffle()
	00401899  MOV ESI,keygenme.00410750						=> esi = &h[48]
	0040189E  MOV EDI,keygenme.00410730						=> edi = &h[16]
	004018A3  MOV ECX,10									=> ecx = 16
	004018A8  REP MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]	=> memcpy(&h[16],&h[48],16)
	004018AA  CALL keygenme.004016F0						=> call shuffle()
	004018AF  MOV EAX,keygenme.00410720						=> return &h
	004018B4  POP EDI										=> restore registers
	004018B5  POP ESI										=>
	004018B6  RETN											=>
```
```c
	UCHAR *pre_shuffle( void )
	{
		memset( &h[16 + I(&h[0x44])], (16 - I(&h[0x44])) & 0xff, (16 - I(&h[0x44])) & 0xff );
		shuffle();
		memcpy(&h[16],&h[48],16);
		shuffle();

		return h;
	}
```
___
```assembly	
// ============================================================================================= //
//                                       keygenme.004016F0                                       //
//                                           shuffle()                                           //
// ============================================================================================= //
	004016F0  PUSHAD							=> all registers in stack
	004016F1  MOV EBP,keygenme.00410720			=> ebp = v_410720 = h
	004016F6  SUB ESP,4							=>
	004016F9  MOV EAX,DWORD PTR SS:[EBP+10] 	=> 
	004016FC  MOV EBX,DWORD PTR SS:[EBP+14] 	=>
	004016FF  MOV ECX,DWORD PTR SS:[EBP+18] 	=>
	00401702  MOV EDX,DWORD PTR SS:[EBP+1C] 	=>
	00401705  XOR EAX,DWORD PTR SS:[EBP] 		=>
	00401708  XOR EBX,DWORD PTR SS:[EBP+4] 	 	=>
	0040170B  XOR ECX,DWORD PTR SS:[EBP+8]	  	=>
	0040170E  XOR EDX,DWORD PTR SS:[EBP+C]  	=>
	00401711  MOV DWORD PTR SS:[EBP+20],EAX 	=> I(h[32]) = I(h[16]) ^ I(h[0])
	00401714  MOV DWORD PTR SS:[EBP+24],EBX 	=> I(h[36]) = I(h[20]) ^ I(h[4])
	00401717  MOV DWORD PTR SS:[EBP+28],ECX 	=> I(h[40]) = I(h[24]) ^ I(h[8])
	0040171A  MOV DWORD PTR SS:[EBP+2C],EDX 	=> I(h[44]) = I(h[28]) ^ I(h[12])
	0040171D  MOVZX EDX,BYTE PTR SS:[EBP+3F]	=> edx = h[0x3f]
	00401721  MOV EDI,-18						=> edi = -0x18
	---------------------------------------------------------------------
  /-00401726  MOVZX ECX,BYTE PTR DS:[EDI+EBP+28]=> ecx = h[0x28 + i + edi]
  |	0040172B  MOVZX EAX,BYTE PTR DS:[EDI+EBP+48]=> eax = h[0x48 + i + edi]
  |	00401730  XOR EDX,ECX						=> edx ^= h[0x28 + i + edi]
  |	00401732  MOVZX EBX,BYTE PTR DS:[EDX+40F160]=> ebx = v_40F160[ edx ]
  |	00401739  XOR EAX,EBX						=> eax = h[0x48+edi] ^ v_40F160[ edx ]
  |	0040173B  MOV BYTE PTR DS:[EDI+EBP+48],AL	=> h[0x48 + i + edi] = al
  |	0040173F  MOV EDX,EAX						=> edx = h[0x48 + edi]
  |
  |	00401741 to 0040175A  						=> the same code with i = 1
  |	0040175C to 00401775    					=> the same code with i = 2
  |	00401777 to 00401790    					=> the same code with i = 3
  |
  |	00401792  ADD EDI,4							=> edi += 4
  \-00401795  JNZ SHORT keygenme.00401726       => if edi == 0 then break
	---------------------------------------------------------------------
	00401797  XOR EAX,EAX						=> eax = 0
	00401799  MOV ESI,keygenme.00410720			=> esi = &h
	0040179E  XOR EDX,EDX						=> edx = 0
	004017A0  MOV EBP,-30						=> ebp = -0x30
	---------------------------------------------------------------------
/-/-004017A5  MOVZX EBX,BYTE PTR SS:[EBP+ESI+30]	=> ebx = h[0x30 + i + ebp]
| |	004017AA  MOVZX EAX,BYTE PTR DS:[EAX+40F160]	=> 
| |	004017B1  XOR EAX,EBX							=> eax = h[0x30 + i + ebp] ^ v_40F160[ eax ]
| |	004017B3  MOV BYTE PTR SS:[EBP+ESI+30],AL		=> h[0x30 + i + ebp] ^= v_40F160[ eax ]
| |	
| |	004017B7 to 004017C5						=> the same code with i = 1
| |	004017C9 to 004017D7 						=> the same code with i = 2
| |	004017DB to 004017E9  						=> the same code with i = 3
| |	
| |	004017ED  ADD EBP,4							=> ebp += 4
| \-004017F0  JNZ SHORT keygenme.004017A5		=> if ebp == 0 then break
|	---------------------------------------------------------------------
|	004017F2  ADD EAX,EDX						=> eax += edx
|	004017F4  INC EDX							=> 
|	004017F5  AND EAX,0FF						=> eax &= 0xff
|	004017FA  CMP EDX,12						=> ++edx == 18 ?
|	004017FD  MOV EBP,-30						=> ebp = -30
/---00401802  JNZ SHORT keygenme.004017A5		=> if yes break
	---------------------------------------------------------------------
	00401804  ADD ESP,4							=> 
	00401807  POPAD								=> pop registers from stack
	00401808  RETN								=> return	
```

```c
	void shuffle( void )
	{
	#define I(a) (*(int*) a)

		int i, j, eax, edx;

		I(&h[32]) = I(&h[16]) ^ I(&h[0]);
		I(&h[36]) = I(&h[20]) ^ I(&h[4]);
		I(&h[40]) = I(&h[24]) ^ I(&h[8]);
		I(&h[44]) = I(&h[28]) ^ I(&h[12]);

		edx = h[0x3f] & 0xff;

		for( i=-0x18; i!=0; i++ )
		{
			edx ^= h[0x28 + i];
			h[0x48 + i] ^= v_40F160[ edx & 0xff ];
			edx = h[0x48 + i] & 0xff;
		}

		for( eax=0, j=0; j<18; j++, eax+=j-1 )
		for( i=-0x30; i!=0; i++ )
		{
			h[0x30 + i] ^= v_40F160[ eax & 0xff ];
			eax = h[0x30 + i];
		}
	}	
```	
___
```assembly
// ============================================================================================= //
//                                       keygenme.00408AF6                                       //
//                                            split()                                            //
// ============================================================================================= //
	00408AF6  PUSH EBP							=>
	00408AF7  MOV EBP,ESP						=> prolog
	00408AF9  SUB ESP,20						=> allocate memory in stack
	00408AFC  PUSH EBX							=>
	00408AFD  PUSH ESI							=> 
	00408AFE  MOV ESI,DWORD PTR SS:[EBP+C]		=> esi = 0040F030 (arg 2)
	00408B01  PUSH EDI							=> backup registers
	00408B02  PUSH 8							=> 
	00408B04  XOR EAX,EAX						=> eax = 0
	00408B06  POP ECX							=> ecx = 8
	00408B07  LEA EDI,DWORD PTR SS:[EBP-20]		=> edi = &B
	00408B0A  REP STOS DWORD PTR ES:[EDI]		=> memset( B, 0, 32 )
	00408B0C  PUSH 7							=>
	00408B0E  POP EDI							=> edi = 7
	---------------------------------------------------------------------
	00408B0F  MOV DL,BYTE PTR DS:[ESI]			=> dl = v_40F030[i]
	00408B11  MOV BL,1							=> bl = 1
	00408B13  MOVZX ECX,DL						=> ecx = v_40F030[i]
	00408B16  MOV EAX,ECX						=> eax = v_40F030[i]
	00408B18  AND ECX,EDI						=> ecx = v_40F030[i] & 7
	00408B1A  SHR EAX,3							=> eax = v_40F030[i] >> 3
	00408B1D  SHL BL,CL							=> bl  = 1 << (v_40F030[i] & 7)
	00408B1F  LEA EAX,DWORD PTR SS:[EBP+EAX-20]	=> eax = B[ v_40F030[i] >> 3 ]
	00408B23  OR BYTE PTR DS:[EAX],BL			=> B[ v_40F030[i] >> 3 ] |= 1 << (v_40F030[i] & 7)
	00408B25  INC ESI							=> esi = i++;
	00408B26  TEST DL,DL						=> 
	00408B28  JNZ SHORT keygenme.00408B0F		=> if  v_40F030[i] == 0 then break
	
	=> Here v_40F030 contains only one character: '-'. Thus we have:
	B[ '-' >> 3 ] |= 1 << ('-' & 7) = 0x20
	B[ 0 ] |= 1 << 0 = 0x01	
	---------------------------------------------------------------------
	00408B2A  MOV EDX,DWORD PTR SS:[EBP+8]		=> edx = serial
	00408B2D  TEST EDX,EDX						=>
	00408B2F  JNZ SHORT keygenme.00408B37		=> if( edx == 0 ) then
	00408B31  MOV EDX,DWORD PTR DS:[410788]		=>     edx = v_410788 (always FALSE)
	---------------------------------------------------------------------
	00408B37  MOV AL,BYTE PTR DS:[EDX]			=>
	........
	00408B57  MOV EBX,EDX						=> (garbage)
	---------------------------------------------------------------------
	00408B59  MOV AL,BYTE PTR DS:[EDX]			=> al = serial[i]
	00408B5B  TEST AL,AL						=> al == 0 ? 
	00408B5D  JE SHORT keygenme.00408B7D		=> if yes break;
	00408B5F  MOVZX ESI,AL						=> esi = serial[i]
	00408B62  MOV ECX,ESI						=> ecx = serial[i]
	00408B64  PUSH 1							=>
	00408B66  AND ECX,EDI						=> ecx = serial[i] & 7
	00408B68  POP EAX							=> 
	00408B69  SHL EAX,CL						=> eax = 1 << (serial[i] & 7)
	00408B6B  SHR ESI,3							=> esi = serial[i] >> 3
	00408B6E  MOV CL,BYTE PTR SS:[EBP+ESI-20]	=> cl = B[ serial[i] >> 3 ]
	00408B72  TEST CL,AL						=> B[ serial[i] >> 3 ] & (1 << (serial[i] & 7)) != 0 ?
	00408B74  JNZ SHORT keygenme.00408B79		=> if yes break
	00408B76  INC EDX							=> i++
	00408B77  JMP SHORT keygenme.00408B59		=> continue loop
	
	for( i=1; serial[i]!=0; i++ )
		if( B[ serial[i] >> 3 ] & (1 << (serial[i] & 7)) != 0 )
		break;
		
	=> Here, we search for the first occurence of '-' character in serial
	---------------------------------------------------------------------
	00408B79  AND BYTE PTR DS:[EDX],0			=> serial[i] = 0
	00408B7C  INC EDX							=> 
	00408B7D  MOV EAX,EBX						=> eax = previous i (from 00408B57)
	00408B7F  POP EDI							=> restore registers
	00408B80  SUB EAX,EDX						=> 
	00408B82  POP ESI							=>
	00408B83  NEG EAX							=> eax = ~(prev_i - i)
	00408B85  SBB EAX,EAX						=> eax = 0
	00408B87  MOV DWORD PTR DS:[410788],EDX		=> *v_410788 = ++i
	00408B8D  AND EAX,EBX						=> eax = 0
	00408B8F  POP EBX							=> 
	00408B90  LEAVE								=> epilog
	00408B91  RETN								=>
```

```c
	void split( char serial, const char *v_40F030 )
	{
		for( i=0; serial[i]!=0; i++ )
			if( serial[i] == '-' )
			{
				serial[i] = 0;
				break;
			}
	}	
```
___