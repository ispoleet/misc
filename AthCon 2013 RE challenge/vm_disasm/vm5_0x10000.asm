[001]	41cade	6	lea     edi, dword_4065A8+4Bh					; overwrite the real values of VECTOR
[002]	41cae4	6	mov     dword ptr [edi], 8DF489ACh
[003]	41caea	3	add     edi, 4
[004]	41caed	6	mov     dword ptr [edi], 0C9AEF51h
[005]	41caf3	3	add     edi, 4
[006]	41caf6	6	mov     dword ptr [edi], 9DCA468Bh
[007]	41cafc	3	add     edi, 4
[008]	41caff	6	mov     dword ptr [edi], 9AE2B76h
[009]	41cb05	3	add     edi, 4
[010]	41cb08	6	mov     dword ptr [edi], 7501167Dh
[011]	41cb0e	3	add     edi, 4
[012]	41cb11	6	mov     dword ptr [edi], 0A7D831Ah
[013]	41cb17	3	add     edi, 4
[014]	41cb1a	6	mov     dword ptr [edi], 8C850F03h
[015]	41cb20	3	add     edi, 4
[016]	41cb23	6	mov     dword ptr [edi], 8334889Dh
[017]	41cb29	5	mov esi, 0040649Ch								; start decryption from here
[018]	41cb2e	5	mov ecx, 000000E0h	
[019]	41cb33	2		mov     eax, [esi]
[020]	41cb35	5		xor     eax, 8456FD3Ah						; 4-byte XOR decoding
[021]	41cb3a	2		mov     [esi], eax
[022]	41cb3c	3		add     esi, 4
[023]	41cb3f	3		sub     ecx, 4
[024]	41cb42	2	jnz -0Fh (target: 41cb33)
[025]	41cb44	2	push 00h										; arg4: MB_OK
[026]	41cb46	5	push 004064BDh									; arg3: AthCon 2013 - RE Challenge
[027]	41cb4b	5	push 004064D8h									; arg2: Failure is simply the opportunity to begin again, this time more intelligently (Henry Ford). Try harder...‹
[028]	41cb50	2	push 00h										; arg1: HWND = NULL
[029]	41cb52	5	call VM (40303Eh)	 (jmp     ds:MessageBoxA)
[030]	41cb57	2	push 00h
[031]	41cb59	5	call VM (40304Ah)	 (jmp     ds:ExitProcess)
[032]	41cb5e	5	and     eax, 0DE51CF4Dh
[033]	41cb63	2	cmp     al, 0E2h
[034]	41cb65	1	retf