## Advanced Math Crackme
##### October 2013
___
### Description:
This crackme needs the good unterstanding of cryptography. ;)

I think you have to patch only the public keys! Nothing other. :)

Much fun :)
___

About "functions.md": This file contains some functions of crackme. I write these functions in a 
separate file, in order to keep this file as simple as possible.

This crackme uses MIRACL library. Here: http://certivox.org/display/EXT/MIRACL+Reference+Manual
you can find the official MIRACL reference manual. It's very easy to determine which functions
a program uses, if you read this article: http://beatrix2004.free.fr/pamplemousse/miracl.pdf 
(thanks to redoC for his hint).

Let's start from module entry point: 00408B92. When function (keygenme.00401110) at line 
	00408C5B executed, then the main window is shown:
```assembly
	00401110  MOV EAX,DWORD PTR SS:[ESP+4]
	00401114  PUSH 0                                                ; /lParam = NULL
	00401116  PUSH keygenme.00401000                                ; |DlgProc = keygenme.00401000
	0040111B  PUSH 0                                                ; |hOwner = NULL
	0040111D  PUSH 65                                               ; |pTemplate = 65
	0040111F  PUSH EAX                                              ; |hInst
	00401120  MOV DWORD PTR DS:[410714],EAX                         ; |
	00401125  CALL DWORD PTR DS:[<&USER32.DialogBoxParamA>]      	; \DialogBoxParamA
	0040112B  XOR EAX,EAX
	0040112D  RETN 10
```
The interesting part here is Dialog Procedure which is located at 00401000. Inside this function
	we are interested in WM_COMMAND messages, with Register button (ControlID = 0x3F1) as lParam:

```assembly
	........
	0040108D  CMP DWORD PTR SS:[ESP+C],3F1		=> lparam == 0x3F1 (register button clicked?)
	00401095  JNZ SHORT keygenme.004010A5		=> if not return 1
	00401097  MOV EAX,DWORD PTR DS:[410710]		=>
	0040109C  PUSH EAX							=> 1st arg: *v_410710
	0040109D  CALL keygenme.00401130			=> call verify()
	004010A2  ADD ESP,4							=>
	004010A5  MOV EAX,1							=>
	004010AA  RETN 10							=>
    ........
```

Function keygenme.00401130 is the main function of serial verification...

```assembly	
	00401130  SUB ESP,2C0					=> allocate memory for local variables
	00401136  MOV DL,BYTE PTR DS:[410718]	=> dl = 0
	0040113C  PUSH EBX						=> backup registers
	0040113D  PUSH EBP						=>					
	0040113E  PUSH ESI						=>
	0040113F  PUSH EDI						=>
	00401140  MOV ECX,3F					=> 
	........
	004011B0  LEA EDI,DWORD PTR SS:[ESP+D9]	=>
	004011B7  PUSH 10						=> !!!
	004011B9  REP STOS DWORD PTR ES:[EDI]	=>
	004011BB  STOS WORD PTR ES:[EDI]		=>
	004011BD  STOS BYTE PTR ES:[EDI]		=>
```

=>	Here, we initialize (using REP STOS DWORD PTR ES:[EDI] instruction) the local 
		arrays as follows:
```c
		char serial_1[52] = { '\0' }; // array from 0x70  to 0xA4
		char serial_2[52] = { '\0' }; // array from 0xA4  to 0xD8
		char name [120]   = { '\0' }; // array from 0xD8  to 0x150
		char b_150[128]   = { '\0' }; // array from 0x150 to 0x1D0
		char b_1d0[256]   = { '\0' }; // array from 0x1D0 to 0x2D0
```
```assembly
	004011BE  XOR EAX,EAX					=> eax = 0
	004011C0  PUSH 200						=> !!!
	004011C5  MOV DWORD PTR SS:[ESP+65],EAX	=>
	004011C9  MOV BYTE PTR SS:[ESP+64],DL	=>
	004011CD  MOV DWORD PTR SS:[ESP+69],EAX	=>
	004011D1  MOV DWORD PTR SS:[ESP+6D],EAX	=>
	004011D5  MOV DWORD PTR SS:[ESP+71],EAX	=>
	004011D9  MOV WORD PTR SS:[ESP+75],AX	=>
	004011DE  MOV BYTE PTR SS:[ESP+77],AL	=>
```
=>	Here, the 2 PUSH instructions change the ESP. This means that the offsets
		change too. Thus [ESP+0x64], is actually [ESP+0x64-8] = [ESP+0x58]

		char b_58[20] = { '\0' };	

=> 	It would be better to keep track of PUSH/POP instructions in order to find
		easy variables in stack. So far: OFFSET = 0x8 (we've 2 PUSH)
___
```assembly
	004011E2  CALL keygenme.00401ED0 				=> mirsys(512, 16)
	004011E7  PUSH 0								=> 1st arg: 0; (OFFSET = 0xc)
	004011E9  MOV DWORD PTR DS:[EAX+238],10			=> p[0x238] = 0x10
	004011F3  CALL keygenme.00401DA0				=> call mirvar
	004011F8  MOV EDI,EAX							=> edi = (flash) mirvar(0);
	004011FA  PUSH 0								=> 1st arg: 0; (OFFSET = 0x10)
	004011FC  MOV DWORD PTR SS:[ESP+50],EDI			=> v_40 = mirvar(0) ([ESP+50-OFFSET] = [ESP+40])
	........
	004012A2  MOV DWORD PTR SS:[ESP+40],EAX			=>
	004012A6  CALL keygenme.00401DA0				=> call mirvar
	004012AB  MOV ECX,DWORD PTR SS:[ESP+28]			=> (ignore)
	004012AF  PUSH keygenme.0040F140                => (ignore) (OFFSET = 0x10)
	004012B4  PUSH ECX								=> (ignore) (OFFSET = 0x14)
	004012B5  MOV DWORD PTR SS:[ESP+64],EAX			=> v_58 = mirvar(0) (OFFSET = 0x14)
```
=> We have:
```c
			v_40 = mirvar(0); // OFFSET = 0x10
			v_48 = mirvar(0); // OFFSET = 0x14
			v_14 = mirvar(0); // OFFSET = 0x18
			v_18 = mirvar(0); // OFFSET = 0x1c
			v_58 = mirvar(0); // OFFSET = 0x20
			v_54 = mirvar(0); // OFFSET = 0x24
			v_3c = mirvar(0); // OFFSET = 0x28
			ebp  = mirvar(0); // OFFSET = 0x2c
			ebx  = mirvar(0); // OFFSET = 0x2c
			v_24 = mirvar(0); // OFFSET = 0x34
			v_38 = mirvar(0); // OFFSET = 0x38
			v_28 = mirvar(0); // OFFSET = 0x3c
			v_2c = mirvar(0); // OFFSET = 0x40
			v_30 = mirvar(0); // OFFSET = 0x0  (00401287  ADD ESP,40)
			v_1c = mirvar(0); // OFFSET = 0x8
			v_34 = mirvar(1); // OFFSET = 0xc
			v_50 = mirvar(0); // OFFSET = 0x14
```            
___
```assembly
	004012AB  MOV ECX,DWORD PTR SS:[ESP+28]			=> ecx = v_1c (OFFSET = 0xc)
	004012AF  PUSH keygenme.0040F140                => 2nd arg: "2B0E91B6EFF461C9E1AC4C659"
	004012B4  PUSH ECX								=> 1st arg: v_1c
	004012B5  MOV DWORD PTR SS:[ESP+64],EAX			=>
	004012B9  CALL keygenme.00404610				=> call cinstr (store big num to v_1c)
	........
	004012E3  MOV EAX,DWORD PTR SS:[ESP+44]			=> eax = v_18 (OFFSET = 0x2c)
	004012E7  PUSH keygenme.0040F0D0                => OFFSET = 0x30
	004012EC  PUSH EAX								=> OFFSET = 0x34
	004012ED  CALL keygenme.00404610				=> call cinstr (store big num to v_1c)
```
```c
		cinstr(v_1c, "2B0E91B6EFF461C9E1AC4C659");	
		cinstr(v_40, "3776741C950EC29BC0A0CB7A4");
		cinstr(v_28, "24961CC92EA9ABE6E1175C88F");	// OFFSET = 0x1c => [ESP+44-1c] = [ESP+28]
		cinstr(v_48, "231AD302175D8E4277AD80B03");
		cinstr(v_18, "3776741C950EC6DF909AAF8A7");	// OFFSET = 0x2c => [ESP+44-2c] = [ESP+18] 
```

```assembly
	004012F2  MOV EDX,DWORD PTR DS:[410710]			=> edx = *v_410710 (hWnd)
	004012F8  MOV ESI,DWORD PTR DS:[<&USER32.GetDlgItemTextA>]
	004012FE  ADD ESP,34							=> OFFSET    = 0x0
	00401301  LEA ECX,DWORD PTR SS:[ESP+D8]			=> ecx       = &name
	00401308  PUSH 78                               => Count     = 120
	0040130A  PUSH ECX                              => Buffer    = name
	0040130B  PUSH 3E8                              => ControlID = 0x3E8
	00401310  PUSH EDX                              => hWnd 	 = 001003E0 (OFFSET = 0x10)
	00401311  CALL ESI                              => call GetDlgItemText	(OFFSET = 0x0)
	00401313  CALL keygenme.0040180C
			-------------------------------------------------------------
			0040180C  PUSH EDI						=> 
			0040180D  XOR EAX,EAX					=> eax = 0
			0040180F  MOV DWORD PTR DS:[410764],EAX	=> *v_410764 = 0
			00401814  MOV EDI,keygenme.00410720		=> edi = &v_410720
			00401819  MOV ECX,10					=> ecx = 16
			0040181E  REP STOS DWORD PTR ES:[EDI]	=> memset(v_410720,0,64)
			00401820  MOV EAX,keygenme.00410720		=> return &v_410720
			00401825  POP EDI						=> 
			00401826  RETN							=> 
			
				v_410764 = 0;
				memset(v_410720,0,64); 
			-------------------------------------------------------------
					
	00401318  LEA EDI,DWORD PTR SS:[ESP+D8]			=> edi = &name
	0040131F  OR ECX,FFFFFFFF						=> ecx = -1
	00401322  XOR EAX,EAX							=> eax = 0
	00401324  REPNE SCAS BYTE PTR ES:[EDI]			=> for( i=0; name[i]!=0; i++, ecx-- );
	00401326  NOT ECX								=> 
	00401328  DEC ECX								=> ecx = ~ecx - 1 = strlen( name )
	00401329  LEA EAX,DWORD PTR SS:[ESP+D8]			=>
	00401330  PUSH ECX								=> 2nd arg: strlen(name)
	00401331  PUSH EAX								=> 1st arg: name (OFFSET = 0x18)
	00401332  CALL keygenme.00401828				=> call copy_name()
	00401337  CALL keygenme.00401874				=> call pre_shuffle()
	0040133C  LEA ECX,DWORD PTR SS:[ESP+5C]			=> (OFFSET = 0x0)
	00401340  PUSH ECX								=> 3rd arg: &b_5c
	00401341  PUSH 10								=> 2nd arg: 16
	00401343  PUSH EAX								=> 1st arg: &v_410720
	00401344  CALL keygenme.00401690				=> hex_to_ascii(char* in, int length, char* out)

	00401349  MOV EDX,DWORD PTR SS:[ESP+14]		=> 
	........
	00401380  CALL keygenme.00404610			=> call cinstr
```
```c
	cinstr(v_14, "3776741C950EC29BC0A0CB7A8");	// OFFSET = 0x0
	cinstr(v_3c, v_5c);							// OFFSET = 0x8
	cinstr(v_2c, "E822EC76C6B42C3FFC77EE26");	// OFFSET = 0x10
	cinstr(v_30, "190D00984A452060C42DA9A0F");	// OFFSET = 0x18
```

```assembly
	00401385  MOV EDX,DWORD PTR SS:[ESP+2F4]	=> edx = v_2c4 ??
	0040138C  ADD ESP,20						=> (OFFSET - 0x0)
	0040138F  LEA ECX,DWORD PTR SS:[ESP+70]		=> ecx = v_70
	00401393  PUSH 34							=> Count     = 52
	00401395  PUSH ECX							=> Buffer    = &serial_1
	00401396  PUSH 3F4							=> ControlID = 0x3F4
	0040139B  PUSH EDX							=> hWnd 	 = 001003E0 (OFFSET = 0x10)
	0040139C  CALL ESI							=> call GetDlgItemText	(OFFSET = 0x0)
	0040139E  LEA EDI,DWORD PTR SS:[ESP+70]		=>
	........
	004013B4  PUSH keygenme.0040F030			=> 2nd arg: 0x0040F030: "-\0"
	........
	004013CB  LEA ECX,DWORD PTR SS:[ESP+74]		=> strncpy(b_150, serial_1, strlen(serial_1))
	004013CF  PUSH ECX							=> 1st arg: serial_1 (OFFSET = 0x8)
	004013D0  CALL keygenme.00408AF6 			=> call split()
	---------------------------------------------------------------------
	004013D5  LEA EDI,DWORD PTR SS:[ESP+78]		=> edi = serial_1 (OFFSET = 0x8)
	........
	0040140A  REP MOVS BYTE PTR ES:[EDI],BYTE PTR DS:[ESI]
```
This code extracts, the second part of the serial and copy it, into serial_2 array.	
So, serial must contains 2 fields ('-' is the delimiter). The 1st field is copying 
at serial_1 and the second part at serial_2.


```assembly
	0040140C  LEA ECX,DWORD PTR SS:[ESP+78]		=> ecx = serial_1 (OFFSET = 0x8)
	00401410  PUSH ECX							=> 2nd arg: serial_1
	00401411  PUSH EBP							=> 1st arg: ebp (=mirvar(0), 0040124B)
	00401412  CALL keygenme.00404610			=> cinstr(ebp, serial_1)

	00401417  MOV EDI,DWORD PTR SS:[ESP+24]		=> edi = v_14 (OFFSET = 0x10)
	0040141B  MOV EDX,DWORD PTR SS:[ESP+44]		=> edx = v_34 (= (big num) 1)
	0040141F  PUSH EDI							=>
	00401420  PUSH EDX							=>
	00401421  PUSH EDI							=> (OFFSET = 0x1c)
	00401422  CALL keygenme.00406F90			=> subtract(v_14, v_34, v_14) => v_14 -= 1

	00401427  LEA EAX,DWORD PTR SS:[ESP+C0]		=> eax = &serial_2
	0040142E  PUSH EAX							=> 2nd arg: serial_2
	0040142F  PUSH EBX							=> 1st arg: ebx (=mirvar(0), 00401252)
	00401430  CALL keygenme.00404610			=> cinstr(ebx, serial_2)

	00401435  MOV ECX,DWORD PTR SS:[ESP+6C]		=> ecx = v_48 (OFFSET = 0x24)
	00401439  MOV EDX,DWORD PTR SS:[ESP+64]		=> edx = v_40
	0040143D  PUSH 0							=> 4th arg: 0
	0040143F  PUSH EDI							=> 3rd arg: v_14
	00401440  PUSH ECX							=> 2nd arg: v_48
	00401441  PUSH EDX							=> 1st arg: v_40 (OFFSET = 0x34)
	00401442  CALL keygenme.004030B0			=> call ecurve_init(v_40,v_48,v_14,0)
	
	ecurve_init(A,B,p,0)	=> y^2 = x^3 + Ax + B (mod p)
	-----------------------------------------------------------------
	00401447  CALL keygenme.00403220		=> epoint_init()
	0040144C  MOV ECX,DWORD PTR SS:[ESP+5C]	=> ecx = v_28 (OFFSET = 0x34)
	00401450  MOV DWORD PTR SS:[ESP+78],EAX	=> e_44 = epoint_init();
	00401454  PUSH EAX						=> 4th arg: e_44
	00401455  MOV EAX,DWORD PTR SS:[ESP+64]	=> eax = v_2c (OFFSET = 0x38)
	00401459  PUSH 0						=> 3rd arg:	0
	0040145B  PUSH EAX						=> 2nd arg: v_2c
	0040145C  PUSH ECX						=> 1st arg: v_28
	0040145D  CALL keygenme.004032E0		=> epoint_set(v_28, v_2c, 0, e_44)
	00401462  ADD ESP,44					=> (OFFSET = 0x0)
	00401465  CALL keygenme.00403220		=> 
	0040146A  MOV EDX,DWORD PTR SS:[ESP+1C]	=> edx = v_1c
	0040146E  MOV DWORD PTR SS:[ESP+4C],EAX	=> e_4c = epoint_init();
	00401472  PUSH EAX						=> 4th arg: e_4c
	00401473  MOV EAX,DWORD PTR SS:[ESP+34] => eax = v_30 (OFFSET = 0x4)
	00401477  PUSH 0						=> 3rd arg: 0
	00401479  PUSH EDX						=> 2nd arg: v_1c
	0040147A  PUSH EAX						=> 1st arg: v_30  (OFFSET = 0x10)
	0040147B  CALL keygenme.004032E0		=> epoint_set(v_30, v_1c, 0, e_4c)
	00401480  CALL keygenme.00403220		=> 
	00401485  MOV DWORD PTR SS:[ESP+24],EAX	=> e_14 = epoint_init();
	00401489  CALL keygenme.00403220		=>  
	0040148E  MOV ECX,DWORD PTR SS:[ESP+28]	=> ecx = v_18
	00401492  PUSH EBX						=> 5th arg: ebx
	00401493  PUSH EBX						=> 4th arg: ebx
	00401494  PUSH EBX						=> 3rd arg: ebx
	00401495  PUSH ECX						=> 2nd arg: v_18
	00401496  PUSH EBX						=> 1st arg: ebx (OFFSET = 0x24)
	00401497  MOV ESI,EAX					=> esi = epoint_init();
```
```assembly
	00401499  CALL keygenme.00406670		=> call xgcd => ebx = 1/ebx mod v_18
	0040149E  MOV EDX,DWORD PTR SS:[ESP+48]	=> edx = v_24 (OFFSET = 0x24)
	004014A2  MOV EAX,DWORD PTR SS:[ESP+60]	=> eax = v_3c
	004014A6  PUSH EDX						=> 
	004014A7  PUSH EBX						=> 
	004014A8  PUSH EAX						=> (OFFSET = 0x30)
	004014A9  CALL keygenme.004057C0		=> multiply(v_3c, ebx, v_24)
	004014AE  MOV ECX,DWORD PTR SS:[ESP+68]	=> ecx = v_38
	004014B2  PUSH ECX						=>
	004014B3  PUSH EBX						=>
	004014B4  PUSH EBP						=> (OFFSET = 0x3c)
	004014B5  CALL keygenme.004057C0		=> multiply(ebp, ebx, v_38);
	004014BA  MOV EDX,DWORD PTR SS:[ESP+50]	=> edx = e_14
	004014BE  MOV EAX,DWORD PTR SS:[ESP+80]	=> eax = e_44
	004014C5  MOV ECX,DWORD PTR SS:[ESP+60]	=> ecx = v_24
	004014C9  PUSH EDX						=>
	004014CA  PUSH EAX						=>
	004014CB  PUSH ECX						=> (OFFSET = 0x48)
	004014CC  CALL keygenme.00404270		=> ecurve_mult( v_24, e_44, e_14 )
	004014D1  MOV EDX,DWORD PTR SS:[ESP+94]	=> edx = e_4c
	004014D8  MOV EAX,DWORD PTR SS:[ESP+80]	=> eax = v_38
	004014DF  ADD ESP,48					=> (OFFSET = 0x0)
	004014E2  PUSH ESI						=>
	004014E3  PUSH EDX						=>
	004014E4  PUSH EAX						=> (OFFSET = 0xc)
	004014E5  CALL keygenme.00404270		=> ecurve_mult( v_38, e_4c, esi )
	004014EA  MOV ECX,DWORD PTR SS:[ESP+20]	=> ecx = e_14
	004014EE  PUSH ESI						=>
	004014EF  PUSH ECX						=> (OFFSET = 0x14)
	004014F0  CALL keygenme.00403800		=> ecurve_add(e_14, esi)
	004014F5  MOV EDX,DWORD PTR SS:[ESP+30]	=> edx = v_1c
	004014F9  PUSH EDX						=>
	004014FA  PUSH EBP						=>
	004014FB  PUSH ESI						=> (OFFSET = 0x20)
	004014FC  CALL keygenme.00403550		=> epoint_get(esi, ebp, v_1c)
	00401501  MOV EAX,DWORD PTR SS:[ESP+38]	=> eax = v_18
	00401505  MOV ECX,DWORD PTR SS:[ESP+54]	=> ecx = v_34
	00401509  PUSH EBP						=>
	0040150A  PUSH EAX						=>
	0040150B  PUSH ECX						=>
	0040150C  PUSH EBP						=> (OFFSET = 0x30)
	0040150D  CALL keygenme.00404F20		=> powmod(ebp, v_34, v_18, ebp)
	00401512  LEA EDX,DWORD PTR SS:[ESP+200]=> edx = v_1d0
	00401519  PUSH EDX						=>
	0040151A  PUSH EBP						=> (OFFSET = 0x38)
	0040151B  CALL keygenme.004048A0		=> cotstr(ebp, b_1d0)
```	
This chunk of code, is the classic Elliptic Curve Digital Signature Algorithm (ECDSA) 
Message Verification. The input is the message hash, and a pair	(r,s) which is the 
signature. The algorithm is simple:
```c	
	w  = 1/s mod n
	u1 = e * w mod n			(e = Hash(m))
    u2 = r * w mod n	
    X  = u1*P + u2*Q = (x1,y1)	(Q is public key)
    v  = x1 mod n				(if v == r then signature is correct)			
```

```assembly
	00401520  MOV ECX,DWORD PTR DS:[410710]	=>
	00401526  ADD ESP,38					=> (OFFSET = 0x0)
	00401529  LEA EAX,DWORD PTR SS:[ESP+1D0]=>
	00401530  PUSH EAX                      => Text      = b_1d0
	00401531  PUSH 3F8                      => ControlID = 0x3F8
	00401536  PUSH ECX                      => hWnd      = 001003E0 
	00401537  CALL DWORD PTR DS:[<&USER32.SetDlgItemTextA>] 
	0040153D  LEA EDX,DWORD PTR SS:[ESP+1D0]=> edx  = &b_1d0 (OFFSET = 0x0)
	00401544  LEA EAX,DWORD PTR SS:[ESP+70]	=> eax  = &serial_1
	00401548  MOV DWORD PTR SS:[ESP+20],EDX	=> v_20 = &b_1d0
	0040154C  MOV CL,BYTE PTR DS:[EAX]		=>
	........
	00401576  JNZ SHORT keygenme.0040154C	=> strcmp(serial_1, b_1d0)
	00401578  XOR EAX,EAX					=> eax = 0 (iff serial_1 == b_1d0)
	0040157A  JMP SHORT keygenme.00401581	=>
	0040157C  SBB EAX,EAX					=> 
	0040157E  SBB EAX,-1					=> eax = -1 (iff serial_1 != b_1d0)

	00401581  TEST EAX,EAX					=> eax == 0 ?
	00401583  JNZ SHORT keygenme.00401597 	=>
	00401585  MOV EAX,DWORD PTR DS:[410710]	=>
	0040158A  PUSH keygenme.0040F058        => Text      = "Thank you for your registration!"
	0040158F  PUSH 3F8						=> ControlID = 0x3F8
	00401594  PUSH EAX						=> hWnd      = 001003E0 
	00401595  JMP SHORT keygenme.004015A8

	00401597  MOV ECX,DWORD PTR DS:[410710] => (eax != 0)
	0040159D  PUSH keygenme.0040F03C		=> Text      = "Sorry,your key is invalid!"
	004015A2  PUSH 3F8						=> ControlID = 0x3F8
	004015A7  PUSH ECX						=> hWnd		 = 001003E0 

	004015A8  CALL DWORD PTR DS:[<&USER32.SetDlgItemTextA>]
```
___
```assembly
	004015AE  MOV EDX,DWORD PTR SS:[ESP+40]	=>
	........
	00401679  CALL keygenme.004023F0		=>
```
This is the "cleanup".
___

```assembly
	0040167E  POP EDI						=> restore registers
	0040167F  POP ESI						=>
	00401680  POP EBP						=>
	00401681  XOR EAX,EAX					=> return 0
	00401683  POP EBX						=> function epilog
	00401684  ADD ESP,2C0					=> release memory for local vars
	0040168A  RETN							=> 
```
___
Thus, all we have to do is to write the ECDSA code generation. If we are able to produce the
	valid (r,s) signature, then we'll get the good message. But there's a problem: In order to
	generate (r,s) we need private key (d). We have only, a point (P) on curve, and the public key 
	Q = d*P. It's impossible to get d from Q (this is based on Discrete Logarithm Problem, DLP). 
	So we need to create a new private key d', a new public key Q', and patch the original
	executable with the new keys. After that it's easy to generate valid serials.
	
In patched version I used as private key: d = 1337b4dc0d3. Thus public key Q will be:
	(1DF5585508F8FC871844D0855, D88B4F4A3A42950488CD0110).
		
Code generation algorithm is:
```
	1.Select a random k in range [1, n-1]. I use the same k=123456789CAFEBABE to keep code simple.	
	2.k*P = (x1,y1)
	3.r   = x1 mod n			  (r must be != 0)
	4.s   = 1/k * (e + d*r) mod n (s must be != 0)
```	

Serial is the pair (r,s), or r-s ('-' is delimiter, not subtract).
Examples:
```
	Name  : thisisareally_longname_1234
	Serial: 213A610DC6BE8C2E7AF0F4BFE-BA6DD7A492DFFF4207D1EFC3

	Name  : mopy
	Serial: 213A610DC6BE8C2E7AF0F4BFE-35BEA8822024359DDCA9DDD29
```
ΝΟΤΕ: Verification Algorithm does not check wheather r or s are 0. Thus for each name,
		serial "0" is accepted. :)
___