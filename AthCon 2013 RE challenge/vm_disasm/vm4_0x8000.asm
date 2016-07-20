[001]	414ade	1	push ebp
[002]	414adf	2	mov ebp, esp
[003]	414ae1	1	push esi
[004]	414ae2	1	push edi
[005]	414ae3	1	push ebx
[006]	414ae4	1	push ecx
[007]	414ae5	1	push edx
[008]	414ae6	2	mov ecx, 0
[009]	414ae8	5	mov esi, 00406767h								; start from that address
[010]	414aed	2		mov eax, ecx
[011]	414aef	5		mov edx, 00000008h							; calculate a "hash" value
[012]	414af4	5			test    eax, 1
[013]	414af9	2			jbe 09h (target: 414b04)
[014]	414afb	2			shr     eax, 1
[015]	414afd	5			xor     eax, 0EDB88320h
[016]	414b02	2			jmp 02h (target: 414b06)
[017]	414b04	2			shr     eax, 1
[018]	414b06	1			dec edx
[019]	414b07	2		jnz -13h (target: 414af4)
[020]	414b09	2		mov     [esi], eax							; fill it with the "hash"
[021]	414b0b	3		add     esi, 4
[022]	414b0e	1		inc ecx
[023]	414b0f	6		cmp     ecx, 100h							; do it for 400bytes
[024]	414b15	2	jnz -28h (target: 414aed)
[025]	414b17	3	push    dword ptr [ebp+0Ch]
[026]	414b1a	3	push    dword ptr [ebp+8]
[027]	414b1d	5	mov esi, 00406767h
[028]	414b22	5	mov edi, FFFFFFFFh
[029]	414b27	2	mov ecx, 0
[030]	414b29	3		mov     eax, [esp]
[031]	414b2c	4		movzx   eax, byte ptr [ecx+eax]
[032]	414b30	2		mov edx, edi
[033]	414b32	6		and     edx, 0FFh
[034]	414b38	2		xor eax, edx
[035]	414b3a	3		mov     ebx, [esi+eax*4]
[036]	414b3d	3		shr     edi, 8
[037]	414b40	2		xor     edi, ebx
[038]	414b42	1		inc ecx
[039]	414b43	4		cmp     ecx, [esp+4]						; do 0xa0 (arg2) loops
[040]	414b47	2	jnz -1Eh (target: 414b29)
[041]	414b49	3	add     esp, 8
[042]	414b4c	2	not     edi
[043]	414b4e	2	mov eax, edi
[044]	414b50	1	pop edx
[045]	414b51	1	pop ecx
[046]	414b52	1	pop ebx
[047]	414b53	1	pop edi
[048]	414b54	1	pop esi
[049]	414b55	1	leave
[050]	414b56	3	retn    8

