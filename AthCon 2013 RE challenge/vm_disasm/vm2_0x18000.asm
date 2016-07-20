[001]	424ade	1	push ebp										; function prolog
[002]	424adf	2	mov ebp, esp
[003]	424ae1	6	sub     esp, 200h
[004]	424ae7	1	push eax
[005]	424ae8	1	push edx
[006]	424ae9	6	mov     eax, large fs:30h
[007]	424aef	3	mov     eax, [eax+0Ch]
[008]	424af2	3	mov     eax, [eax+0Ch]
[009]	424af5	3	mov     edx, [eax+28h]
[010]	424af8	3	mov     byte ptr [edx], 0
[011]	424afb	3	mov     edx, [eax+30h]
[012]	424afe	3	mov     byte ptr [edx], 0
[013]	424b01	2	int     2Ah										; KiGetTickCount()
[014]	424b03	3	mov     [ebp-64h], eax
[015]	424b06	6	mov     eax, large fs:30h
[016]	424b0c	3	mov     [ebp-24h], eax
[017]	424b0f	3	mov     eax, [ebp-24h]
[018]	424b12	3	mov     eax, [eax+0Ch]
[019]	424b15	3	mov     eax, [eax+1Ch]
[020]	424b18	1	push ebx
[021]	424b19	3	mov     ebx, [eax+8]
[022]	424b1c	3	mov     edx, [eax+20h]
[023]	424b1f	2	mov     eax, [eax]
[024]	424b21	3	mov     [ebp-30h], ebx							; entry point of ntdll.dll
[025]	424b24	7	mov     dword ptr [ebp-70h], 0
[026]	424b2b	2	mov eax, ebx
[027]	424b2d	1	push ecx
[028]	424b2e	2	mov ecx, ebx
[029]	424b30	3	mov     edx, [ecx+3Ch]
[030]	424b33	4	lea     ecx, [edx+eax+4]
[031]	424b37	2	mov edx, ecx
[032]	424b39	3	add     edx, 14h
[033]	424b3c	2	mov eax, edx
[034]	424b3e	3	mov     ecx, [ebp-30h]
[035]	424b41	3	add     ecx, [eax+60h]
[036]	424b44	3	mov     [ebp-34h], ecx
[037]	424b47	3	add     ecx, 24h
[038]	424b4a	2	mov     ecx, [ecx]
[039]	424b4c	3	mov     eax, [ebp-34h]
[040]	424b4f	3	add     eax, 20h
[041]	424b52	2	mov     eax, [eax]
[042]	424b54	2	sub     ecx, eax
[043]	424b56	3	sub     ecx, 4
[044]	424b59	4	or      dword ptr [ebp-74h], 0FFFFFFFFh
[045]	424b5d	3	mov     [ebp-38h], ecx
[046]	424b60	3	mov     eax, [ebp-34h]
[047]	424b63	3	add     eax, 24h
[048]	424b66	2	mov     eax, [eax]
[049]	424b68	3	sub     eax, 4
[050]	424b6b	3	add     eax, [ebp-30h]
[051]	424b6e	3	mov     [ebp-3Ch], eax
[052]	424b71	3	sub     eax, [ebp-38h]
[053]	424b74	3	sub     eax, 4
[054]	424b77	3	mov     [ebp-40h], eax
[055]	424b7a	1	push esi
[056]	424b7b	2	int     2Ah										; KiGetTickCount()
[057]	424b7d	2	test eax, eax
[058]	424b7f	2	jz 0Fh (target: 424b90)
[059]	424b81	3	cmp     eax, [ebp-64h]
[060]	424b84	2	jl 0Ah (target: 424b90)							; time moves backward??? :P
[061]	424b86	3	sub     eax, [ebp-64h]							; find time interval
[062]	424b89	5	cmp     eax, 1388h								; less than 0x1388 ticks? If yes probably process being debugged
[063]	424b8e	2	jle 36h (target: 424bc6)						
[064]	424b90	2		add     eax, edx							; execute it on error
[065]	424b92	2		add     eax, esi
[066]	424b94	2		add     eax, esp
[067]	424b96	2		add     eax, ebp
[068]	424b98	1		cld
[069]	424b99	7		mov     edi, large fs:18h
[070]	424ba0	2		mov eax, 0									; eax = 0
[071]	424ba2	5		mov ecx, 00000400h
[072]	424ba7	2		rep stosd
[073]	424ba9	3		mov     edi, [ebp-24h]
[074]	424bac	3		mov     edi, [edi+0Ch]
[075]	424baf	5		mov ecx, 00000400h
[076]	424bb4	2		rep stosd
[077]	424bb6	3		mov     edi, [ebp-24h]
[078]	424bb9	5		mov ecx, 00000400h
[079]	424bbe	2		rep stosd
[080]	424bc0	2		or      esp, ebp
[081]	424bc2	2		or      ebp, esp
[082]	424bc4	2		jmp eax										; jump to 0 => Segmentation fault
; -----------------------------------------------------------------------------------------------------------
[083]	424bc6	2	jmp 08h (target: 424bd0)
[084]	424bc8	4	sub     dword ptr [ebp-3Ch], 4
[085]	424bcc	4	sub     dword ptr [ebp-40h], 4
[086]	424bd0	3	mov     eax, [ebp-30h]
[087]	424bd3	3	mov     ebx, [ebp-3Ch]
[088]	424bd6	2	mov     ebx, [ebx]
[089]	424bd8	2	add     eax, ebx
[090]	424bda	3	mov     ebx, [ebp-30h]
[091]	424bdd	3	mov     edx, [ebp-40h]
[092]	424be0	2	mov     edx, [edx]
[093]	424be2	2	add     ebx, edx
[094]	424be4	3	mov     [ebp-44h], ebx
[095]	424be7	2	mov esi, 0
[096]	424be9	2	mov ebx, 0
[097]	424beb	1	push eax
[098]	424bec	1	push edx
[099]	424bed	2	int     2Ah										; KiGetTickCount()
[100]	424bef	2	test eax, eax
[101]	424bf1	2	jz 0Fh (target: 424c02)
[102]	424bf3	3	cmp     eax, [ebp-64h]
[103]	424bf6	2	jl 0Ah (target: 424c02)
[104]	424bf8	3	sub     eax, [ebp-64h]
[105]	424bfb	5	cmp     eax, 4E20h
[106]	424c00	2	jle 38h (target: 424c3a)
[107]	424c02	2		add     edx, eax
[108]	424c04	2		add     edx, ebx
[109]	424c06	2		add     edx, ecx
[110]	424c08	2		add     edx, esp
[111]	424c0a	2		add     edx, ebp
[112]	424c0c	1		cld
[113]	424c0d	7		mov     edi, large fs:18h
[114]	424c14	2		mov eax, 0
[115]	424c16	5		mov ecx, 00000400h
[116]	424c1b	2		rep stosd
[117]	424c1d	3		mov     edi, [ebp-24h]
[118]	424c20	3		mov     edi, [edi+0Ch]
[119]	424c23	5		mov ecx, 00000400h
[120]	424c28	2		rep stosd
[121]	424c2a	3		mov     edi, [ebp-24h]
[122]	424c2d	5		mov ecx, 00000400h
[123]	424c32	2		rep stosd
[124]	424c34	2		or      esp, ebp
[125]	424c36	2		or      ebp, esp
[126]	424c38	2		jmp     edx									; cause a segmentation fault
[127]	424c3a	1	pop edx
[128]	424c3b	1	pop eax											; eax = last exported function name from ntdll ("wcstoul")
[129]	424c3c	3		movzx   ebx, byte ptr [eax]					; find a checksum
[130]	424c3f	3		rol     ebx, 7
[131]	424c42	2		add     esi, ebx
[132]	424c44	3		rol     esi, 7
[133]	424c47	2		xor     esi, ebx
[134]	424c49	1		inc eax
[135]	424c4a	3		cmp     byte ptr [eax], 0
[136]	424c4d	2		jz 02h (target: 424c51)
[137]	424c4f	2	jmp -13h (target: 424c3c)
[138]	424c51	2	mov eax, esi
[139]	424c53	2	mul     esi
[140]	424c55	2	add     eax, edx								; avoid hardcode function names. Use onlly their checksum
; -----------------------------------------------------------------------------------------------------------
[141]	424c57	5	cmp     eax, 318A50B7h							; ZwSetInformationThread() ?
[142]	424c5c	6	jnz 0000012Ch (target: 424d8e)
[143]	424c62	3	mov     eax, [ebp-44h]
[144]	424c65	3	movzx   ebx, byte ptr [eax]						; check the 1st byte of the code?
[145]	424c68	3	add     bl, 10h
[146]	424c6b	3	cmp     bl, 0C8h								; is it 0xb8? (mov eax, dword)
[147]	424c6e	2	jz 32h (target: 424ca2)
[148]	424c70	2		xor     ebx, eax							; if not cause segmentation fault
[149]	424c72	2		add     ebx, ebp
[150]	424c74	1		cld
[151]	424c75	7		mov     edi, large fs:18h
[152]	424c7c	2		mov eax, 0
[153]	424c7e	5		mov ecx, 00000400h
[154]	424c83	2		rep stosd
[155]	424c85	3		mov     edi, [ebp-24h]
[156]	424c88	3		mov     edi, [edi+0Ch]
[157]	424c8b	5		mov ecx, 00000400h
[158]	424c90	2		rep stosd
[159]	424c92	3		mov     edi, [ebp-24h]
[160]	424c95	5		mov ecx, 00000400h
[161]	424c9a	2		rep stosd
[162]	424c9c	2		add     esp, ebp
[163]	424c9e	2		xor     ebp, esp
[164]	424ca0	2		jmp     ebx
[165]	424ca2	1	inc eax
[166]	424ca3	2	mov     eax, [eax]								; get that dword (sysenter number)
[167]	424ca5	2	test eax, eax									; if its 0 cause segmentation fault
[168]	424ca7	2	jnz 34h (target: 424cdd)
[169]	424ca9	1		cld
[170]	424caa	7		mov     edi, large fs:18h
[171]	424cb1	2		mov eax, 0
[172]	424cb3	5		mov ecx, 00000400h
[173]	424cb8	2		rep stosd
[174]	424cba	3		mov     edi, [ebp-24h]
[175]	424cbd	3		mov     edi, [edi+0Ch]
[176]	424cc0	5		mov ecx, 00000400h
[177]	424cc5	2		rep stosd
[178]	424cc7	3		mov     edi, [ebp-24h]
[179]	424cca	5		mov ecx, 00000400h
[180]	424ccf	2		rep stosd
[181]	424cd1	2		add     esp, ebp
[182]	424cd3	2		add     ebp, esp
[183]	424cd5	2		add     esp, eax
[184]	424cd7	2		add     esp, esp
[185]	424cd9	2		xor     ebp, esp
[186]	424cdb	2		jmp     esp
[187]	424cdd	3	mov     [ebp-60h], eax							; store service number on loc60
[188]	424ce0	2	push 00h
[189]	424ce2	2	push 00h
[190]	424ce4	2	push 11h										; arg3: ThreadHideFromDebugger
[191]	424ce6	2	push FEh										; arg2
[192]	424ce8	5	call VM (402CACh)	 (push    427730h                         ; GO TO VM3)
[193]	424ced	3	add     eax, 0Dh
[194]	424cf0	2	push 00h										; arg1
[195]	424cf2	1	push eax
[196]	424cf3	3	mov     eax, [ebp-60h]
[197]	424cf6	2	mov edx, esp
[198]	424cf8	2	sysenter										; call ZwSetInformationThread()
[199]	424cfa	2	test eax, eax
[200]	424cfc	2	jz 34h (target: 424d32)
[201]	424cfe	2		add     esi, edi							; if function fails cause SEGFAULT
[202]	424d00	2		add     edi, esi
[203]	424d02	1		cld
[204]	424d03	7		mov     edi, large fs:18h
[205]	424d0a	2		mov eax, 0
[206]	424d0c	5		mov ecx, 00000400h
[207]	424d11	2		rep stosd
[208]	424d13	3		mov     edi, [ebp-24h]
[209]	424d16	3		mov     edi, [edi+0Ch]
[210]	424d19	5		mov ecx, 00000400h
[211]	424d1e	2		rep stosd
[212]	424d20	3		mov     edi, [ebp-24h]
[213]	424d23	5		mov ecx, 00000400h
[214]	424d28	2		rep stosd
[215]	424d2a	2		add     esp, ebp
[216]	424d2c	2		or      esp, edi
[217]	424d2e	2		xor     ebp, esi
[218]	424d30	2		jmp     esp
[219]	424d32	4	cmp     dword ptr [esp], 0						; check if the values you push before are still there
[220]	424d36	2	jnz -38h (target: 424cfe)
[221]	424d38	5	cmp     dword ptr [esp+4], 0FFFFFFFEh
[222]	424d3d	2	jnz -3Fh (target: 424cfe)
[223]	424d3f	5	cmp     dword ptr [esp+8], 11h
[224]	424d44	2	jnz -46h (target: 424cfe)
[225]	424d46	5	cmp     dword ptr [esp+0Ch], 0
[226]	424d4b	2	jnz -4Dh (target: 424cfe)
[227]	424d4d	5	cmp     dword ptr [esp+10h], 0
[228]	424d52	2	jnz -54h (target: 424cfe)
[229]	424d54	6	add     dword_4065A8+4Bh, eax					; update VECTOR (eax = 0)
[230]	424d5a	6	inc     dword_4065A8+4Bh
[231]	424d60	6	add     dword_4065A8+4Fh, eax
[232]	424d66	6	inc     dword_4065A8+4Fh
[233]	424d6c	6	add     dword_4065A8+53h, eax
[234]	424d72	6	inc     dword_4065A8+53h
[235]	424d78	6	add     dword_4065A8+57h, eax
[236]	424d7e	6	inc     dword_4065A8+57h
[237]	424d84	3	add     esp, 14h
[238]	424d87	7	mov     dword ptr [ebp-44h], 0
; -----------------------------------------------------------------------------------------------------------
[239]	424d8e	5	cmp     eax, 0E27847F7h							; ZwFreeVirtualMemory() ?
[240]	424d93	6	jnz 0000008Ch (target: 424e25)
[241]	424d99	3	mov     eax, [ebp-44h]
[242]	424d9c	3	movzx   ebx, byte ptr [eax]
[243]	424d9f	3	add     bl, 20h
[244]	424da2	3	cmp     bl, 0D8h								; 1st byte is b8
[245]	424da5	2	jz 36h (target: 424ddd)							; cause segmentation fault
[246]	424da7	1	cld
[247]	424da8	7	mov     edi, large fs:18h
[248]	424daf	2	mov eax, 0
[249]	424db1	5	mov ecx, 00000400h
[250]	424db6	2	rep stosd
[251]	424db8	3	mov     edi, [ebp-24h]
[252]	424dbb	3	mov     edi, [edi+0Ch]
[253]	424dbe	5	mov ecx, 00000400h
[254]	424dc3	2	rep stosd
[255]	424dc5	3	mov     edi, [ebp-24h]
[256]	424dc8	5	mov ecx, 00000400h
[257]	424dcd	2	rep stosd
[258]	424dcf	2	add     esp, esp
[259]	424dd1	2	add     ebp, ebp
[260]	424dd3	2	add     esp, esp
[261]	424dd5	2	add     ebp, ebp
[262]	424dd7	2	add     eax, esp
[263]	424dd9	2	add     eax, ebp
[264]	424ddb	2	jmp eax
[265]	424ddd	1	inc eax
[266]	424dde	2	mov     eax, [eax]
[267]	424de0	3	and     eax, 0FFFFFFFFh
[268]	424de3	2	jnz 36h (target: 424e1b)						; cause segmentation fault
[269]	424de5	1	cld
[270]	424de6	7	mov     edi, large fs:18h
[271]	424ded	2	mov eax, 0
[272]	424def	5	mov ecx, 00000400h
[273]	424df4	2	rep stosd
[274]	424df6	3	mov     edi, [ebp-24h]
[275]	424df9	3	mov     edi, [edi+0Ch]
[276]	424dfc	5	mov ecx, 00000400h
[277]	424e01	2	rep stosd
[278]	424e03	3	mov     edi, [ebp-24h]
[279]	424e06	5	mov ecx, 00000400h
[280]	424e0b	2	rep stosd
[281]	424e0d	2	add     esp, eax
[282]	424e0f	2	add     esp, ebx
[283]	424e11	2	add     esp, edx
[284]	424e13	2	add     ebp, esi
[285]	424e15	2	add     ebp, edx
[286]	424e17	2	add     edx, esp
[287]	424e19	2	jmp     edx
[288]	424e1b	3	mov     [ebp-48h], eax
[289]	424e1e	7	mov     dword ptr [ebp-44h], 0
; -----------------------------------------------------------------------------------------------------------
[290]	424e25	5	cmp     eax, 95AAF2E1h							; ZwDelayExecution() ?
[291]	424e2a	2	jnz 7Fh (target: 424eab)
[292]	424e2c	3	mov     eax, [ebp-44h]
[293]	424e2f	3	movzx   ebx, byte ptr [eax]
[294]	424e32	3	add     bl, 30h
[295]	424e35	3	cmp     bl, 0E8h
[296]	424e38	2	jz 2Eh (target: 424e68)
[297]	424e3a	1	cld
[298]	424e3b	7	mov     edi, large fs:18h
[299]	424e42	2	mov eax, 0
[300]	424e44	5	mov ecx, 00000400h
[301]	424e49	2	rep stosd
[302]	424e4b	3	mov     edi, [ebp-24h]
[303]	424e4e	3	mov     edi, [edi+0Ch]
[304]	424e51	5	mov ecx, 00000400h
[305]	424e56	2	rep stosd
[306]	424e58	3	mov     edi, [ebp-24h]
[307]	424e5b	5	mov ecx, 00000400h
[308]	424e60	2	rep stosd
[309]	424e62	2	mov esp, ebx
[310]	424e64	2	mov ebp, esi
[311]	424e66	2	jmp     edx
[312]	424e68	1	inc eax
[313]	424e69	2	mov     eax, [eax]
[314]	424e6b	2	test eax, eax
[315]	424e6d	2	jnz 32h (target: 424ea1)
[316]	424e6f	1	cld
[317]	424e70	7	mov     edi, large fs:18h
[318]	424e77	2	mov eax, 0
[319]	424e79	5	mov ecx, 00000400h
[320]	424e7e	2	rep stosd
[321]	424e80	3	mov     edi, [ebp-24h]
[322]	424e83	3	mov     edi, [edi+0Ch]
[323]	424e86	5	mov ecx, 00000400h
[324]	424e8b	2	rep stosd
[325]	424e8d	3	mov     edi, [ebp-24h]
[326]	424e90	5	mov ecx, 00000400h
[327]	424e95	2	rep stosd
[328]	424e97	2	mov esp, edi
[329]	424e99	2	mov ebp, esi
[330]	424e9b	2	mov edi, 0
[331]	424e9d	2	mov esi, 0
[332]	424e9f	2	jmp     ecx
[333]	424ea1	3	mov     [ebp-68h], eax
[334]	424ea4	7	mov     dword ptr [ebp-44h], 0
; -----------------------------------------------------------------------------------------------------------
[335]	424eab	5	cmp     eax, 8AFA4D6Dh							; ZwQueryInformationProcess() ?
[336]	424eb0	6	jnz 0000019Bh (target: 425051)
[337]	424eb6	3	mov     eax, [ebp-44h]
[338]	424eb9	3	movzx   ebx, byte ptr [eax]
[339]	424ebc	3	add     bl, 40h
[340]	424ebf	3	cmp     bl, 0F8h
[341]	424ec2	2	jz 2Eh (target: 424ef2)
[342]	424ec4	1	cld
[343]	424ec5	7	mov     edi, large fs:18h
[344]	424ecc	2	mov eax, 0
[345]	424ece	5	mov ecx, 00000400h
[346]	424ed3	2	rep stosd
[347]	424ed5	3	mov     edi, [ebp-24h]
[348]	424ed8	3	mov     edi, [edi+0Ch]
[349]	424edb	5	mov ecx, 00000400h
[350]	424ee0	2	rep stosd
[351]	424ee2	3	mov     edi, [ebp-24h]
[352]	424ee5	5	mov ecx, 00000400h
[353]	424eea	2	rep stosd
[354]	424eec	2	add     esp, ebp
[355]	424eee	2	add     ebp, esp
[356]	424ef0	2	jmp     ebp
[357]	424ef2	1	inc eax
[358]	424ef3	2	mov     eax, [eax]
[359]	424ef5	2	test eax, eax
[360]	424ef7	2	jnz 32h (target: 424f2b)
[361]	424ef9	1	cld
[362]	424efa	7	mov     edi, large fs:18h
[363]	424f01	2	mov eax, 0
[364]	424f03	5	mov ecx, 00000400h
[365]	424f08	2	rep stosd
[366]	424f0a	3	mov     edi, [ebp-24h]
[367]	424f0d	3	mov     edi, [edi+0Ch]
[368]	424f10	5	mov ecx, 00000400h
[369]	424f15	2	rep stosd
[370]	424f17	3	mov     edi, [ebp-24h]
[371]	424f1a	5	mov ecx, 00000400h
[372]	424f1f	2	rep stosd
[373]	424f21	2	add     esp, esi
[374]	424f23	2	add     esp, edi
[375]	424f25	2	add     ebp, esi
[376]	424f27	2	add     ebp, edi
[377]	424f29	2	jmp     esp
[378]	424f2b	3	mov     [ebp-78h], eax
[379]	424f2e	2	push 00h
[380]	424f30	2	push 04h
[381]	424f32	3	lea     eax, [ebp-74h]
[382]	424f35	1	push eax
[383]	424f36	2	push 07h
[384]	424f38	2	push FFh
[385]	424f3a	2	push 00h
[386]	424f3c	5	call VM (402CACh)	 (push    427730h                         ; GO TO VM3)
[387]	424f41	3	add     eax, 0Bh
[388]	424f44	1	push eax
[389]	424f45	3	mov     eax, [ebp-78h]
[390]	424f48	2	mov edx, esp
[391]	424f4a	2	sysenter
[392]	424f4c	2	test eax, eax
[393]	424f4e	2	jz 32h (target: 424f82)
[394]	424f50	1	cld
[395]	424f51	7	mov     edi, large fs:18h
[396]	424f58	2	mov eax, 0
[397]	424f5a	5	mov ecx, 00000400h
[398]	424f5f	2	rep stosd
[399]	424f61	3	mov     edi, [ebp-24h]
[400]	424f64	3	mov     edi, [edi+0Ch]
[401]	424f67	5	mov ecx, 00000400h
[402]	424f6c	2	rep stosd
[403]	424f6e	3	mov     edi, [ebp-24h]
[404]	424f71	5	mov ecx, 00000400h
[405]	424f76	2	rep stosd
[406]	424f78	2	or      esp, ebp
[407]	424f7a	2	or      ebp, ebp
[408]	424f7c	2	sub     eax, edi
[409]	424f7e	2	sub     ebx, esi
[410]	424f80	2	jmp     ebx
[411]	424f82	4	cmp     dword ptr [esp], 0
[412]	424f86	2	jnz -36h (target: 424f50)
[413]	424f88	5	cmp     dword ptr [esp+4], 0FFFFFFFFh
[414]	424f8d	2	jnz -3Dh (target: 424f50)
[415]	424f8f	5	cmp     dword ptr [esp+8], 7
[416]	424f94	2	jnz -44h (target: 424f50)
[417]	424f96	5	cmp     dword ptr [esp+10h], 4
[418]	424f9b	2	jnz -4Bh (target: 424f50)
[419]	424f9d	5	cmp     dword ptr [esp+14h], 0
[420]	424fa2	2	jnz -52h (target: 424f50)
[421]	424fa4	3	add     esp, 18h
[422]	424fa7	6	add     dword_4065A8+4Bh, eax
[423]	424fad	6	inc     dword_4065A8+4Bh
[424]	424fb3	6	add     dword_4065A8+4Fh, eax
[425]	424fb9	6	inc     dword_4065A8+4Fh
[426]	424fbf	6	add     dword_4065A8+53h, eax
[427]	424fc5	6	inc     dword_4065A8+53h
[428]	424fcb	6	add     dword_4065A8+57h, eax
[429]	424fd1	6	inc     dword_4065A8+57h
[430]	424fd7	3	mov     eax, [ebp-74h]
[431]	424fda	2	test eax, eax
[432]	424fdc	2	jz 32h (target: 425010)
[433]	424fde	1	cld
[434]	424fdf	7	mov     edi, large fs:18h
[435]	424fe6	2	mov eax, 0
[436]	424fe8	5	mov ecx, 00000400h
[437]	424fed	2	rep stosd
[438]	424fef	3	mov     edi, [ebp-24h]
[439]	424ff2	3	mov     edi, [edi+0Ch]
[440]	424ff5	5	mov ecx, 00000400h
[441]	424ffa	2	rep stosd
[442]	424ffc	3	mov     edi, [ebp-24h]
[443]	424fff	5	mov ecx, 00000400h
[444]	425004	2	rep stosd
[445]	425006	2	or      esp, esi
[446]	425008	2	xor     ebp, edi
[447]	42500a	2	sub     ecx, esi
[448]	42500c	2	sub     ebx, eax
[449]	42500e	2	jmp     ecx
[450]	425010	3	add     eax, 2
[451]	425013	3	shl     eax, 10h
[452]	425016	6	add     dword_4065A8+4Bh, eax
[453]	42501c	6	inc     dword_4065A8+4Bh
[454]	425022	6	add     dword_4065A8+4Fh, eax
[455]	425028	6	inc     dword_4065A8+4Fh
[456]	42502e	6	add     dword_4065A8+53h, eax
[457]	425034	6	inc     dword_4065A8+53h
[458]	42503a	6	add     dword_4065A8+57h, eax
[459]	425040	6	inc     dword_4065A8+57h
[460]	425046	4	or      dword ptr [ebp-74h], 0FFFFFFFFh
[461]	42504a	7	mov     dword ptr [ebp-44h], 0
; -----------------------------------------------------------------------------------------------------------
[462]	425051	5	cmp     eax, 0D2950638h							; ZwGetContextThread() ?
[463]	425056	6	jnz 0000008Ah (target: 4250e6)
[464]	42505c	3	mov     eax, [ebp-44h]
[465]	42505f	3	movzx   ebx, byte ptr [eax]
[466]	425062	3	sub     bl, 10h
[467]	425065	3	cmp     bl, 0A8h
[468]	425068	2	jz 38h (target: 4250a2)
[469]	42506a	1	cld
[470]	42506b	7	mov     edi, large fs:18h
[471]	425072	2	mov eax, 0
[472]	425074	5	mov ecx, 00000400h
[473]	425079	2	rep stosd
[474]	42507b	3	mov     edi, [ebp-24h]
[475]	42507e	3	mov     edi, [edi+0Ch]
[476]	425081	5	mov ecx, 00000400h
[477]	425086	2	rep stosd
[478]	425088	3	mov     edi, [ebp-24h]
[479]	42508b	5	mov ecx, 00000400h
[480]	425090	2	rep stosd
[481]	425092	2	add     esp, ecx
[482]	425094	2	add     esp, ebx
[483]	425096	2	add     esp, eax
[484]	425098	2	add     ebp, esi
[485]	42509a	2	add     ebp, edi
[486]	42509c	2	add     ebp, esp
[487]	42509e	2	add     esi, ebp
[488]	4250a0	2	jmp     esi
[489]	4250a2	1	inc eax
[490]	4250a3	2	mov     eax, [eax]
[491]	4250a5	3	and     eax, 0FFFFFFFFh
[492]	4250a8	2	jnz 32h (target: 4250dc)
[493]	4250aa	1	cld
[494]	4250ab	7	mov     edi, large fs:18h
[495]	4250b2	2	mov eax, 0
[496]	4250b4	5	mov ecx, 00000400h
[497]	4250b9	2	rep stosd
[498]	4250bb	3	mov     edi, [ebp-24h]
[499]	4250be	3	mov     edi, [edi+0Ch]
[500]	4250c1	5	mov ecx, 00000400h
[501]	4250c6	2	rep stosd
[502]	4250c8	3	mov     edi, [ebp-24h]
[503]	4250cb	5	mov ecx, 00000400h
[504]	4250d0	2	rep stosd
[505]	4250d2	2	add     eax, esp
[506]	4250d4	2	add     ebp, esp
[507]	4250d6	2	add     ebx, ebp
[508]	4250d8	2	add     esi, esp
[509]	4250da	2	jmp eax
[510]	4250dc	3	mov     [ebp-7Ch], eax
[511]	4250df	7	mov     dword ptr [ebp-44h], 0
; -----------------------------------------------------------------------------------------------------------
[512]	4250e6	5	cmp     eax, 25F2995Dh							; ZwQueryVirtualMemory() ?
[513]	4250eb	6	jnz 00000083h (target: 425174)
[514]	4250f1	3	mov     eax, [ebp-44h]
[515]	4250f4	3	movzx   ebx, byte ptr [eax]
[516]	4250f7	3	sub     bl, 20h
[517]	4250fa	3	cmp     bl, 98h
[518]	4250fd	2	jz 34h (target: 425133)
[519]	4250ff	1	cld
[520]	425100	7	mov     edi, large fs:18h
[521]	425107	2	mov eax, 0
[522]	425109	5	mov ecx, 00000400h
[523]	42510e	2	rep stosd
[524]	425110	3	mov     edi, [ebp-24h]
[525]	425113	3	mov     edi, [edi+0Ch]
[526]	425116	5	mov ecx, 00000400h
[527]	42511b	2	rep stosd
[528]	42511d	3	mov     edi, [ebp-24h]
[529]	425120	5	mov ecx, 00000400h
[530]	425125	2	rep stosd
[531]	425127	2	add     esp, ebp
[532]	425129	2	add     ebp, esp
[533]	42512b	2	add     eax, esi
[534]	42512d	2	add     ebx, edi
[535]	42512f	2	add     ecx, eax
[536]	425131	2	jmp     ecx
[537]	425133	1	inc eax
[538]	425134	2	mov     eax, [eax]
[539]	425136	2	test eax, eax
[540]	425138	2	jnz 30h (target: 42516a)
[541]	42513a	1	cld
[542]	42513b	7	mov     edi, large fs:18h
[543]	425142	2	mov eax, 0
[544]	425144	5	mov ecx, 00000400h
[545]	425149	2	rep stosd
[546]	42514b	3	mov     edi, [ebp-24h]
[547]	42514e	3	mov     edi, [edi+0Ch]
[548]	425151	5	mov ecx, 00000400h
[549]	425156	2	rep stosd
[550]	425158	3	mov     edi, [ebp-24h]
[551]	42515b	5	mov ecx, 00000400h
[552]	425160	2	rep stosd
[553]	425162	2	add     esp, esp
[554]	425164	2	add     ebp, ebp
[555]	425166	2	add     edx, esp
[556]	425168	2	jmp     edx
[557]	42516a	3	mov     [ebp-80h], eax
[558]	42516d	7	mov     dword ptr [ebp-44h], 0
; -----------------------------------------------------------------------------------------------------------
[559]	425174	5	cmp     eax, 217A4264h							; ZwAllocateVirtualMemory() ?
[560]	425179	6	jnz FFFFFA49h (target: 424bc8)
[561]	42517f	3	mov     eax, [ebp-44h]
[562]	425182	3	movzx   ebx, byte ptr [eax]
[563]	425185	3	sub     bl, 30h
[564]	425188	3	cmp     bl, 88h
[565]	42518b	2	jz 32h (target: 4251bf)
[566]	42518d	1	cld
[567]	42518e	7	mov     edi, large fs:18h
[568]	425195	2	mov eax, 0
[569]	425197	5	mov ecx, 00000400h
[570]	42519c	2	rep stosd
[571]	42519e	3	mov     edi, [ebp-24h]
[572]	4251a1	3	mov     edi, [edi+0Ch]
[573]	4251a4	5	mov ecx, 00000400h
[574]	4251a9	2	rep stosd
[575]	4251ab	3	mov     edi, [ebp-24h]
[576]	4251ae	5	mov ecx, 00000400h
[577]	4251b3	2	rep stosd
[578]	4251b5	2	add     esp, esp
[579]	4251b7	2	add     ebp, ebp
[580]	4251b9	2	add     eax, esp
[581]	4251bb	2	add     ecx, ebp
[582]	4251bd	2	jmp eax
[583]	4251bf	1	inc eax
[584]	4251c0	2	mov     eax, [eax]
[585]	4251c2	2	test eax, eax
[586]	4251c4	2	jnz 32h (target: 4251f8)
[587]	4251c6	1	cld
[588]	4251c7	7	mov     edi, large fs:18h
[589]	4251ce	2	mov eax, 0
[590]	4251d0	5	mov ecx, 00000400h
[591]	4251d5	2	rep stosd
[592]	4251d7	3	mov     edi, [ebp-24h]
[593]	4251da	3	mov     edi, [edi+0Ch]
[594]	4251dd	5	mov ecx, 00000400h
[595]	4251e2	2	rep stosd
[596]	4251e4	3	mov     edi, [ebp-24h]
[597]	4251e7	5	mov ecx, 00000400h
[598]	4251ec	2	rep stosd
[599]	4251ee	2	add     esp, esp
[600]	4251f0	2	add     ebp, ebp
[601]	4251f2	2	add     eax, esp
[602]	4251f4	2	add     ecx, ebp
[603]	4251f6	2	jmp eax
[604]	4251f8	3	mov     [ebp-4Ch], eax
[605]	4251fb	7	mov     dword ptr [ebp-44h], 0
[606]	425202	7	mov     dword ptr [ebp-6Ch], 0FFFD8F00h
[607]	425209	6	mov     eax, large fs:30h
[608]	42520f	1	push eax
[609]	425210	3	lea     eax, [eax+8]
[610]	425213	3	mov     [ebp-8], eax
[611]	425216	1	pop eax
[612]	425217	3	mov     eax, [eax+0Ch]
[613]	42521a	3	mov     eax, [eax+0Ch]
[614]	42521d	1	push eax
[615]	42521e	3	lea     eax, [eax+18h]
[616]	425221	3	mov     [ebp-0Ch], eax
[617]	425224	1	pop eax
[618]	425225	1	push eax
[619]	425226	3	mov     eax, [eax+18h]
[620]	425229	3	mov     [ebp-4], eax
[621]	42522c	1	pop eax
[622]	42522d	3	mov     eax, [eax+20h]
[623]	425230	3	mov     [ebp-10h], eax
[624]	425233	2	push 40h
[625]	425235	5	push 00003000h
[626]	42523a	3	lea     eax, [ebp-10h]
[627]	42523d	1	push eax
[628]	42523e	2	push 00h
[629]	425240	3	lea     eax, [ebp-14h]
[630]	425243	6	mov     dword ptr [eax], 0
[631]	425249	1	push eax
[632]	42524a	2	push FFh
[633]	42524c	2	push 00h
[634]	42524e	5	call VM (402CACh)	 (push    427730h                         ; GO TO VM3)
[635]	425253	3	add     eax, 0Bh
[636]	425256	1	push eax
[637]	425257	3	mov     eax, [ebp-4Ch]
[638]	42525a	2	mov edx, esp
[639]	42525c	2	sysenter
[640]	42525e	3	add     esp, 1Ch
[641]	425261	2	test eax, eax
[642]	425263	2	jz 2Eh (target: 425293)
[643]	425265	1	cld
[644]	425266	7	mov     edi, large fs:18h
[645]	42526d	2	mov eax, 0
[646]	42526f	5	mov ecx, 00000400h
[647]	425274	2	rep stosd
[648]	425276	3	mov     edi, [ebp-24h]
[649]	425279	3	mov     edi, [edi+0Ch]
[650]	42527c	5	mov ecx, 00000400h
[651]	425281	2	rep stosd
[652]	425283	3	mov     edi, [ebp-24h]
[653]	425286	5	mov ecx, 00000400h
[654]	42528b	2	rep stosd
[655]	42528d	2	add     esp, ebp
[656]	42528f	2	add     ebp, esp
[657]	425291	2	jmp     esp
[658]	425293	1	push eax
[659]	425294	1	push edx
[660]	425295	2	int     2Ah										; KiGetTickCount()
[661]	425297	3	and     eax, 0FFFFFFFFh
[662]	42529a	2	jz 0Fh (target: 4252ab)
[663]	42529c	3	cmp     eax, [ebp-64h]
[664]	42529f	2	jl 0Ah (target: 4252ab)
[665]	4252a1	3	sub     eax, [ebp-64h]
[666]	4252a4	5	cmp     eax, 7530h
[667]	4252a9	2	jle 38h (target: 4252e3)
[668]	4252ab	2	add     ebx, eax
[669]	4252ad	2	add     ebx, ebx
[670]	4252af	2	add     ebx, ecx
[671]	4252b1	2	add     ebx, esp
[672]	4252b3	2	add     ebx, ebp
[673]	4252b5	1	cld
[674]	4252b6	7	mov     edi, large fs:18h
[675]	4252bd	2	mov eax, 0
[676]	4252bf	5	mov ecx, 00000400h
[677]	4252c4	2	rep stosd
[678]	4252c6	3	mov     edi, [ebp-24h]
[679]	4252c9	3	mov     edi, [edi+0Ch]
[680]	4252cc	5	mov ecx, 00000400h
[681]	4252d1	2	rep stosd
[682]	4252d3	3	mov     edi, [ebp-24h]
[683]	4252d6	5	mov ecx, 00000400h
[684]	4252db	2	rep stosd
[685]	4252dd	2	or      esp, ebp
[686]	4252df	2	or      ebp, esp
[687]	4252e1	2	jmp     ebx
[688]	4252e3	1	pop edx
[689]	4252e4	1	pop eax
[690]	4252e5	1	cld
[691]	4252e6	1	push edi
[692]	4252e7	3	mov     esi, [ebp-4]
[693]	4252ea	3	mov     [ebp-50h], esi
[694]	4252ed	3	mov     edi, [ebp-14h]
[695]	4252f0	3	mov     ecx, [ebp-10h]
[696]	4252f3	2	rep movsb
[697]	4252f5	1	pop edi
[698]	4252f6	3	mov     eax, [ebp+4]
[699]	4252f9	3	mov     [ebp-18h], eax
[700]	4252fc	3	mov     eax, [ebp-4]
[701]	4252ff	3	sub     [ebp-18h], eax
[702]	425302	3	mov     eax, [ebp-14h]
[703]	425305	3	add     eax, [ebp-18h]
[704]	425308	3	mov     [ebp+4], eax
[705]	42530b	3	mov     eax, [ebp-0Ch]
[706]	42530e	3	push    dword ptr [ebp-14h]
[707]	425311	2	pop     dword ptr [eax]
[708]	425313	3	mov     eax, [ebp-8]
[709]	425316	3	push    dword ptr [ebp-14h]
[710]	425319	2	pop     dword ptr [eax]
[711]	42531b	1	push eax
[712]	42531c	1	push edx
[713]	42531d	2	int     2Ah										; KiGetTickCount()
[714]	42531f	3	and     eax, 0FFFFFFFFh
[715]	425322	2	jz 0Fh (target: 425333)
[716]	425324	3	cmp     eax, [ebp-64h]
[717]	425327	2	jl 0Ah (target: 425333)
[718]	425329	3	sub     eax, [ebp-64h]
[719]	42532c	5	cmp     eax, 88B8h
[720]	425331	2	jle 38h (target: 42536b)
[721]	425333	2	add     esi, eax
[722]	425335	2	add     esi, ebx
[723]	425337	2	add     esi, ecx
[724]	425339	2	add     esi, esp
[725]	42533b	2	add     esi, ebp
[726]	42533d	1	cld
[727]	42533e	7	mov     edi, large fs:18h
[728]	425345	2	mov eax, 0
[729]	425347	5	mov ecx, 00000400h
[730]	42534c	2	rep stosd
[731]	42534e	3	mov     edi, [ebp-24h]
[732]	425351	3	mov     edi, [edi+0Ch]
[733]	425354	5	mov ecx, 00000400h
[734]	425359	2	rep stosd
[735]	42535b	3	mov     edi, [ebp-24h]
[736]	42535e	5	mov ecx, 00000400h
[737]	425363	2	rep stosd
[738]	425365	2	or      esp, ebp
[739]	425367	2	or      ebp, esp
[740]	425369	2	jmp     esi
[741]	42536b	1	pop edx
[742]	42536c	1	pop eax
[743]	42536d	6	mov     eax, large fs:30h
[744]	425373	3	add     eax, 34h
[745]	425376	3	mov     ecx, [ebp-4]
[746]	425379	2	mov     [eax], ecx
[747]	42537b	3	sub     eax, 4
[748]	42537e	2	mov     eax, [eax]
[749]	425380	3	mov     [ebp-58h], eax
[750]	425383	3	lea     eax, [ebp-6Ch]
[751]	425386	1	push eax
[752]	425387	2	push 00h
[753]	425389	5	call VM (402CACh)	 (push    427730h                         ; GO TO VM3)
[754]	42538e	3	add     eax, 14h
[755]	425391	2	push 00h
[756]	425393	1	push eax
[757]	425394	3	mov     eax, [ebp-68h]
[758]	425397	7	mov     dword ptr [ebp-68h], 0FFFFFFFFh
[759]	42539e	2	mov edx, esp
[760]	4253a0	2	sysenter
[761]	4253a2	2	test eax, eax
[762]	4253a4	2	jz 34h (target: 4253da)
[763]	4253a6	1	cld
[764]	4253a7	7	mov     edi, large fs:18h
[765]	4253ae	2	mov eax, 0
[766]	4253b0	5	mov ecx, 00000400h
[767]	4253b5	2	rep stosd
[768]	4253b7	3	mov     edi, [ebp-24h]
[769]	4253ba	3	mov     edi, [edi+0Ch]
[770]	4253bd	5	mov ecx, 00000400h
[771]	4253c2	2	rep stosd
[772]	4253c4	3	mov     edi, [ebp-24h]
[773]	4253c7	5	mov ecx, 00000400h
[774]	4253cc	2	rep stosd
[775]	4253ce	2	add     esp, edi
[776]	4253d0	2	add     esp, esi
[777]	4253d2	2	add     ebp, esi
[778]	4253d4	2	add     ebp, edi
[779]	4253d6	2	add     edx, ebp
[780]	4253d8	2	jmp     edx
[781]	4253da	3	add     esp, 0Ch
[782]	4253dd	6	lea     eax, [ebp-200h]
[783]	4253e3	1	push eax
[784]	4253e4	6	mov     dword ptr [eax], offset unk_10010
[785]	4253ea	2	push FEh
[786]	4253ec	2	push 00h
[787]	4253ee	5	call VM (402CACh)	 (push    427730h                         ; GO TO VM3)
[788]	4253f3	3	add     eax, 0Bh
[789]	4253f6	1	push eax
[790]	4253f7	3	mov     eax, [ebp-7Ch]
[791]	4253fa	2	mov edx, esp
[792]	4253fc	2	sysenter
[793]	4253fe	2	test eax, eax
[794]	425400	2	jz 34h (target: 425436)
[795]	425402	1	cld
[796]	425403	7	mov     edi, large fs:18h
[797]	42540a	2	mov eax, 0
[798]	42540c	5	mov ecx, 00000400h
[799]	425411	2	rep stosd
[800]	425413	3	mov     edi, [ebp-24h]
[801]	425416	3	mov     edi, [edi+0Ch]
[802]	425419	5	mov ecx, 00000400h
[803]	42541e	2	rep stosd
[804]	425420	3	mov     edi, [ebp-24h]
[805]	425423	5	mov ecx, 00000400h
[806]	425428	2	rep stosd
[807]	42542a	2	add     eax, edx
[808]	42542c	2	add     eax, esi
[809]	42542e	2	add     ebx, edi
[810]	425430	2	add     esp, eax
[811]	425432	2	add     ebp, ebx
[812]	425434	2	jmp     esp
[813]	425436	3	add     esp, 0Ch
[814]	425439	6	add     dword_4065A8+4Bh, eax
[815]	42543f	6	inc     dword_4065A8+4Bh
[816]	425445	6	add     dword_4065A8+4Fh, eax
[817]	42544b	6	inc     dword_4065A8+4Fh
[818]	425451	6	add     dword_4065A8+53h, eax
[819]	425457	6	inc     dword_4065A8+53h
[820]	42545d	6	add     dword_4065A8+57h, eax
[821]	425463	6	inc     dword_4065A8+57h
[822]	425469	6	lea     eax, [ebp-200h]
[823]	42546f	3	add     eax, 4
[824]	425472	2	mov     eax, [eax]
[825]	425474	2	test eax, eax
[826]	425476	2	jz 30h (target: 4254a8)
[827]	425478	1	cld
[828]	425479	7	mov     edi, large fs:18h
[829]	425480	2	mov eax, 0
[830]	425482	5	mov ecx, 00000400h
[831]	425487	2	rep stosd
[832]	425489	3	mov     edi, [ebp-24h]
[833]	42548c	3	mov     edi, [edi+0Ch]
[834]	42548f	5	mov ecx, 00000400h
[835]	425494	2	rep stosd
[836]	425496	3	mov     edi, [ebp-24h]
[837]	425499	5	mov ecx, 00000400h
[838]	42549e	2	rep stosd
[839]	4254a0	2	add     esp, esp
[840]	4254a2	2	add     esp, esp
[841]	4254a4	2	add     ebp, esp
[842]	4254a6	1	push ebp
[843]	4254a7	1	retn
[844]	4254a8	6	add     dword_4065A8+4Bh, eax
[845]	4254ae	6	inc     dword_4065A8+4Bh
[846]	4254b4	6	add     dword_4065A8+4Fh, eax
[847]	4254ba	6	inc     dword_4065A8+4Fh
[848]	4254c0	6	add     dword_4065A8+53h, eax
[849]	4254c6	6	inc     dword_4065A8+53h
[850]	4254cc	6	add     dword_4065A8+57h, eax
[851]	4254d2	6	inc     dword_4065A8+57h
[852]	4254d8	1	push eax
[853]	4254d9	1	push edx
[854]	4254da	2	int     2Ah										; KiGetTickCount()
[855]	4254dc	3	cmp     eax, 0
[856]	4254df	2	jz 0Fh (target: 4254f0)
[857]	4254e1	3	cmp     eax, [ebp-64h]
[858]	4254e4	2	jl 0Ah (target: 4254f0)
[859]	4254e6	3	sub     eax, [ebp-64h]
[860]	4254e9	5	cmp     eax, 9C40h
[861]	4254ee	2	jle 38h (target: 425528)
[862]	4254f0	2	add     ebx, eax
[863]	4254f2	2	add     ebx, ebx
[864]	4254f4	2	add     ebx, ecx
[865]	4254f6	2	add     ebx, esp
[866]	4254f8	2	add     ebx, ebp
[867]	4254fa	1	cld
[868]	4254fb	7	mov     edi, large fs:18h
[869]	425502	2	mov eax, 0
[870]	425504	5	mov ecx, 00000400h
[871]	425509	2	rep stosd
[872]	42550b	3	mov     edi, [ebp-24h]
[873]	42550e	3	mov     edi, [edi+0Ch]
[874]	425511	5	mov ecx, 00000400h
[875]	425516	2	rep stosd
[876]	425518	3	mov     edi, [ebp-24h]
[877]	42551b	5	mov ecx, 00000400h
[878]	425520	2	rep stosd
[879]	425522	2	or      esp, ebp
[880]	425524	2	or      ebp, esp
[881]	425526	2	jmp     ebx
[882]	425528	1	pop edx
[883]	425529	1	pop eax
[884]	42552a	6	lea     eax, [ebp-200h]
[885]	425530	3	add     eax, 8
[886]	425533	2	mov     eax, [eax]
[887]	425535	2	test eax, eax
[888]	425537	2	jz 30h (target: 425569)
[889]	425539	1	cld
[890]	42553a	7	mov     edi, large fs:18h
[891]	425541	2	mov eax, 0
[892]	425543	5	mov ecx, 00000400h
[893]	425548	2	rep stosd
[894]	42554a	3	mov     edi, [ebp-24h]
[895]	42554d	3	mov     edi, [edi+0Ch]
[896]	425550	5	mov ecx, 00000400h
[897]	425555	2	rep stosd
[898]	425557	3	mov     edi, [ebp-24h]
[899]	42555a	5	mov ecx, 00000400h
[900]	42555f	2	rep stosd
[901]	425561	2	add     ebp, ebp
[902]	425563	2	add     ebp, ebp
[903]	425565	2	add     esp, ebp
[904]	425567	1	push esp
[905]	425568	1	retn
[906]	425569	6	add     dword_4065A8+4Bh, eax
[907]	42556f	6	inc     dword_4065A8+4Bh
[908]	425575	6	add     dword_4065A8+4Fh, eax
[909]	42557b	6	inc     dword_4065A8+4Fh
[910]	425581	6	add     dword_4065A8+53h, eax
[911]	425587	6	inc     dword_4065A8+53h
[912]	42558d	6	add     dword_4065A8+57h, eax
[913]	425593	6	inc     dword_4065A8+57h
[914]	425599	3	mov     eax, [ebp-24h]
[915]	42559c	3	lea     eax, [eax+8]
[916]	42559f	2	mov     eax, [eax]
[917]	4255a1	1	push eax
[918]	4255a2	3	lea     eax, [eax+3Ch]
[919]	4255a5	2	mov     eax, [eax]
[920]	4255a7	1	pop ebx
[921]	4255a8	2	add     eax, ebx
[922]	4255aa	5	add     eax, 104h
[923]	4255af	1	push ebx
[924]	4255b0	1	push eax
[925]	4255b1	2	add     ebx, [eax]
[926]	4255b3	2	jmp 04h (target: 4255b9)
[927]	4255b5	4	add     ebx, [esp+4]
[928]	4255b9	7	mov     dword ptr [ebp-10h], 1Ch
[929]	4255c0	3	lea     eax, [ebp-10h]
[930]	4255c3	1	push eax
[931]	4255c4	3	push    dword ptr [ebp-10h]
[932]	4255c7	6	lea     eax, [ebp-150h]
[933]	4255cd	1	push eax
[934]	4255ce	2	push 00h
[935]	4255d0	1	push ebx
[936]	4255d1	2	push FFh
[937]	4255d3	2	push 00h
[938]	4255d5	5	call VM (402CACh)	 (push    427730h                         ; GO TO VM3)
[939]	4255da	3	add     eax, 0Bh
[940]	4255dd	1	push eax
[941]	4255de	3	mov     eax, [ebp-80h]
[942]	4255e1	2	mov edx, esp
[943]	4255e3	2	sysenter
[944]	4255e5	2	test eax, eax
[945]	4255e7	2	jz 3Ah (target: 425623)
[946]	4255e9	1	cld
[947]	4255ea	7	mov     edi, large fs:18h
[948]	4255f1	2	mov eax, 0
[949]	4255f3	5	mov ecx, 00000400h
[950]	4255f8	2	rep stosd
[951]	4255fa	3	mov     edi, [ebp-24h]
[952]	4255fd	3	mov     edi, [edi+0Ch]
[953]	425600	5	mov ecx, 00000400h
[954]	425605	2	rep stosd
[955]	425607	3	mov     edi, [ebp-24h]
[956]	42560a	5	mov ecx, 00000400h
[957]	42560f	2	rep stosd
[958]	425611	2	add     ebx, esi
[959]	425613	2	add     edx, edi
[960]	425615	2	or      esi, ebx
[961]	425617	2	add     edi, edx
[962]	425619	2	xor     eax, esp
[963]	42561b	2	add     eax, ebp
[964]	42561d	2	or      esp, ebp
[965]	42561f	2	or      ebp, esp
[966]	425621	2	jmp     esi
[967]	425623	6	lea     eax, [ebp-13Ch]
[968]	425629	2	mov     eax, [eax]
[969]	42562b	2	mov ebx, eax
[970]	42562d	1	dec ebx
[971]	42562e	2	jnz 38h (target: 425668)
[972]	425630	1	cld
[973]	425631	7	mov     edi, large fs:18h
[974]	425638	2	mov eax, 0
[975]	42563a	5	mov ecx, 00000400h
[976]	42563f	2	rep stosd
[977]	425641	3	mov     edi, [ebp-24h]
[978]	425644	3	mov     edi, [edi+0Ch]
[979]	425647	5	mov ecx, 00000400h
[980]	42564c	2	rep stosd
[981]	42564e	3	mov     edi, [ebp-24h]
[982]	425651	5	mov ecx, 00000400h
[983]	425656	2	rep stosd
[984]	425658	2	or      esp, esi
[985]	42565a	2	or      ebp, edi
[986]	42565c	2	add     eax, eax
[987]	42565e	2	add     ebx, ebx
[988]	425660	2	add     ecx, ecx
[989]	425662	2	add     edi, edi
[990]	425664	2	add     esi, esi
[991]	425666	2	jmp     ebx
[992]	425668	2	mov ebx, eax
[993]	42566a	6	and     ebx, 100h
[994]	425670	2	jz 38h (target: 4256aa)
[995]	425672	1	cld
[996]	425673	7	mov     edi, large fs:18h
[997]	42567a	2	mov eax, 0
[998]	42567c	5	mov ecx, 00000400h
[999]	425681	2	rep stosd
[1000]	425683	3	mov     edi, [ebp-24h]
[1001]	425686	3	mov     edi, [edi+0Ch]
[1002]	425689	5	mov ecx, 00000400h
[1003]	42568e	2	rep stosd
[1004]	425690	3	mov     edi, [ebp-24h]
[1005]	425693	5	mov ecx, 00000400h
[1006]	425698	2	rep stosd
[1007]	42569a	2	or      esp, esi
[1008]	42569c	2	or      ebp, edi
[1009]	42569e	2	add     eax, eax
[1010]	4256a0	2	add     ebx, ebx
[1011]	4256a2	2	add     ecx, ecx
[1012]	4256a4	2	add     edi, edi
[1013]	4256a6	2	add     esi, esi
[1014]	4256a8	2	jmp     ecx
[1015]	4256aa	3	shl     eax, 14h
[1016]	4256ad	6	add     dword_4065A8+4Bh, eax
[1017]	4256b3	6	add     dword_4065A8+4Fh, eax
[1018]	4256b9	6	add     dword_4065A8+53h, eax
[1019]	4256bf	6	add     dword_4065A8+57h, eax
[1020]	4256c5	3	add     esp, 1Ch
[1021]	4256c8	1	pop eax
[1022]	4256c9	3	add     eax, 28h
[1023]	4256cc	1	push eax
[1024]	4256cd	2	mov     ebx, [eax]
[1025]	4256cf	2	test ebx, ebx
[1026]	4256d1	6	jnz FFFFFEDEh (target: 4255b5)
[1027]	4256d7	3	add     esp, 8
[1028]	4256da	3	mov     eax, [ebp-24h]
[1029]	4256dd	3	lea     eax, [eax+8]
[1030]	4256e0	2	mov     eax, [eax]
[1031]	4256e2	1	push eax
[1032]	4256e3	3	lea     eax, [eax+3Ch]
[1033]	4256e6	2	mov     eax, [eax]
[1034]	4256e8	1	pop ebx
[1035]	4256e9	2	add     eax, ebx
[1036]	4256eb	1	push eax
[1037]	4256ec	3	add     eax, 34h
[1038]	4256ef	2	mov     eax, [eax]
[1039]	4256f1	2	mov ebx, eax
[1040]	4256f3	1	pop eax
[1041]	4256f4	5	add     eax, 104h
[1042]	4256f9	1	push ebx
[1043]	4256fa	1	push eax
[1044]	4256fb	2	add     ebx, [eax]
[1045]	4256fd	2	jmp 04h (target: 425703)
[1046]	4256ff	4	add     ebx, [esp+4]
[1047]	425703	7	mov     dword ptr [ebp-10h], 1Ch
[1048]	42570a	3	lea     eax, [ebp-10h]
[1049]	42570d	1	push eax
[1050]	42570e	3	push    dword ptr [ebp-10h]
[1051]	425711	6	lea     eax, [ebp-150h]
[1052]	425717	1	push eax
[1053]	425718	2	push 00h
[1054]	42571a	1	push ebx
[1055]	42571b	2	push FFh
[1056]	42571d	2	push 00h
[1057]	42571f	5	call VM (402CACh)	 (push    427730h                         ; GO TO VM3)
[1058]	425724	3	add     eax, 0Bh
[1059]	425727	1	push eax
[1060]	425728	3	mov     eax, [ebp-80h]
[1061]	42572b	2	mov edx, esp
[1062]	42572d	2	sysenter
[1063]	42572f	2	test eax, eax
[1064]	425731	2	jz 36h (target: 425769)
[1065]	425733	1	cld
[1066]	425734	7	mov     edi, large fs:18h
[1067]	42573b	2	mov eax, 0
[1068]	42573d	5	mov ecx, 00000400h
[1069]	425742	2	rep stosd
[1070]	425744	3	mov     edi, [ebp-24h]
[1071]	425747	3	mov     edi, [edi+0Ch]
[1072]	42574a	5	mov ecx, 00000400h
[1073]	42574f	2	rep stosd
[1074]	425751	3	mov     edi, [ebp-24h]
[1075]	425754	5	mov ecx, 00000400h
[1076]	425759	2	rep stosd
[1077]	42575b	2	add     esp, esi
[1078]	42575d	2	add     ebp, edi
[1079]	42575f	2	add     eax, esp
[1080]	425761	2	or      ebx, edx
[1081]	425763	2	xor     ecx, ebp
[1082]	425765	2	add     ebx, esp
[1083]	425767	2	jmp     ebx
[1084]	425769	6	lea     eax, [ebp-13Ch]
[1085]	42576f	2	mov     eax, [eax]
[1086]	425771	2	mov ebx, eax
[1087]	425773	1	dec ebx
[1088]	425774	2	jnz 38h (target: 4257ae)
[1089]	425776	1	cld
[1090]	425777	7	mov     edi, large fs:18h
[1091]	42577e	2	mov eax, 0
[1092]	425780	5	mov ecx, 00000400h
[1093]	425785	2	rep stosd
[1094]	425787	3	mov     edi, [ebp-24h]
[1095]	42578a	3	mov     edi, [edi+0Ch]
[1096]	42578d	5	mov ecx, 00000400h
[1097]	425792	2	rep stosd
[1098]	425794	3	mov     edi, [ebp-24h]
[1099]	425797	5	mov ecx, 00000400h
[1100]	42579c	2	rep stosd
[1101]	42579e	2	or      esp, esi
[1102]	4257a0	2	or      ebp, edi
[1103]	4257a2	2	add     eax, eax
[1104]	4257a4	2	add     ebx, ebx
[1105]	4257a6	2	add     ecx, ecx
[1106]	4257a8	2	add     edi, edi
[1107]	4257aa	2	add     esi, esi
[1108]	4257ac	2	jmp     ebx
[1109]	4257ae	2	mov ebx, eax
[1110]	4257b0	6	and     ebx, 100h
[1111]	4257b6	2	jz 38h (target: 4257f0)
[1112]	4257b8	1	cld
[1113]	4257b9	7	mov     edi, large fs:18h
[1114]	4257c0	2	mov eax, 0
[1115]	4257c2	5	mov ecx, 00000400h
[1116]	4257c7	2	rep stosd
[1117]	4257c9	3	mov     edi, [ebp-24h]
[1118]	4257cc	3	mov     edi, [edi+0Ch]
[1119]	4257cf	5	mov ecx, 00000400h
[1120]	4257d4	2	rep stosd
[1121]	4257d6	3	mov     edi, [ebp-24h]
[1122]	4257d9	5	mov ecx, 00000400h
[1123]	4257de	2	rep stosd
[1124]	4257e0	2	or      esp, esi
[1125]	4257e2	2	or      ebp, edi
[1126]	4257e4	2	add     eax, eax
[1127]	4257e6	2	add     ebx, ebx
[1128]	4257e8	2	add     ecx, ecx
[1129]	4257ea	2	add     edi, edi
[1130]	4257ec	2	add     esi, esi
[1131]	4257ee	2	jmp     ecx
[1132]	4257f0	3	shl     eax, 14h
[1133]	4257f3	6	add     dword_4065A8+4Bh, eax
[1134]	4257f9	6	add     dword_4065A8+4Fh, eax
[1135]	4257ff	6	add     dword_4065A8+53h, eax
[1136]	425805	6	add     dword_4065A8+57h, eax
[1137]	42580b	3	add     esp, 1Ch
[1138]	42580e	1	pop eax
[1139]	42580f	3	add     eax, 28h
[1140]	425812	1	push eax
[1141]	425813	2	mov     ebx, [eax]
[1142]	425815	2	test ebx, ebx
[1143]	425817	6	jnz FFFFFEE2h (target: 4256ff)
[1144]	42581d	3	add     esp, 8
[1145]	425820	1	push eax
[1146]	425821	1	push edx
[1147]	425822	2	int     2Ah										; KiGetTickCount()
[1148]	425824	2	or      eax, eax
[1149]	425826	2	jz 0Fh (target: 425837)
[1150]	425828	3	cmp     eax, [ebp-64h]
[1151]	42582b	2	jl 0Ah (target: 425837)
[1152]	42582d	3	sub     eax, [ebp-64h]
[1153]	425830	5	cmp     eax, 0C350h
[1154]	425835	2	jle 38h (target: 42586f)
[1155]	425837	2	add     edi, eax
[1156]	425839	2	add     edi, ebx
[1157]	42583b	2	add     edi, ecx
[1158]	42583d	2	or      edi, esp
[1159]	42583f	2	or      edi, ebp
[1160]	425841	1	cld
[1161]	425842	7	mov     edi, large fs:18h
[1162]	425849	2	mov eax, 0
[1163]	42584b	5	mov ecx, 00000400h
[1164]	425850	2	rep stosd
[1165]	425852	3	mov     edi, [ebp-24h]
[1166]	425855	3	mov     edi, [edi+0Ch]
[1167]	425858	5	mov ecx, 00000400h
[1168]	42585d	2	rep stosd
[1169]	42585f	3	mov     edi, [ebp-24h]
[1170]	425862	5	mov ecx, 00000400h
[1171]	425867	2	rep stosd
[1172]	425869	2	or      esp, ebp
[1173]	42586b	2	or      ebp, esp
[1174]	42586d	2	jmp     edi
[1175]	42586f	1	pop edx
[1176]	425870	1	pop eax
[1177]	425871	2	push 00h
[1178]	425873	2	push 00h
[1179]	425875	2	push 11h
[1180]	425877	2	push FEh
[1181]	425879	6	lea     eax, [ebp-200h]
[1182]	42587f	3	add     eax, 0Ch
[1183]	425882	2	mov     eax, [eax]
[1184]	425884	2	test eax, eax
[1185]	425886	2	jz 32h (target: 4258ba)
[1186]	425888	1	cld
[1187]	425889	7	mov     edi, large fs:18h
[1188]	425890	2	mov eax, 0
[1189]	425892	5	mov ecx, 00000400h
[1190]	425897	2	rep stosd
[1191]	425899	3	mov     edi, [ebp-24h]
[1192]	42589c	3	mov     edi, [edi+0Ch]
[1193]	42589f	5	mov ecx, 00000400h
[1194]	4258a4	2	rep stosd
[1195]	4258a6	3	mov     edi, [ebp-24h]
[1196]	4258a9	5	mov ecx, 00000400h
[1197]	4258ae	2	rep stosd
[1198]	4258b0	2	add     esp, edi
[1199]	4258b2	2	add     ebp, edi
[1200]	4258b4	2	add     ebp, esp
[1201]	4258b6	2	add     esp, ebp
[1202]	4258b8	1	push edx
[1203]	4258b9	1	retn
[1204]	4258ba	6	add     dword_4065A8+4Bh, eax
[1205]	4258c0	6	inc     dword_4065A8+4Bh
[1206]	4258c6	6	add     dword_4065A8+4Fh, eax
[1207]	4258cc	6	inc     dword_4065A8+4Fh
[1208]	4258d2	6	add     dword_4065A8+53h, eax
[1209]	4258d8	6	inc     dword_4065A8+53h
[1210]	4258de	6	add     dword_4065A8+57h, eax
[1211]	4258e4	6	inc     dword_4065A8+57h
[1212]	4258ea	5	call VM (402CACh)	 (push    427730h                         ; GO TO VM3)
[1213]	4258ef	3	add     eax, 0Dh
[1214]	4258f2	2	push 00h
[1215]	4258f4	1	push eax
[1216]	4258f5	3	mov     eax, [ebp-60h]
[1217]	4258f8	2	mov edx, esp
[1218]	4258fa	2	sysenter
[1219]	4258fc	2	test eax, eax
[1220]	4258fe	2	jz 34h (target: 425934)
[1221]	425900	2	add     esi, edi
[1222]	425902	2	add     edi, esi
[1223]	425904	1	cld
[1224]	425905	7	mov     edi, large fs:18h
[1225]	42590c	2	mov eax, 0
[1226]	42590e	5	mov ecx, 00000400h
[1227]	425913	2	rep stosd
[1228]	425915	3	mov     edi, [ebp-24h]
[1229]	425918	3	mov     edi, [edi+0Ch]
[1230]	42591b	5	mov ecx, 00000400h
[1231]	425920	2	rep stosd
[1232]	425922	3	mov     edi, [ebp-24h]
[1233]	425925	5	mov ecx, 00000400h
[1234]	42592a	2	rep stosd
[1235]	42592c	2	add     esp, ebp
[1236]	42592e	2	or      esp, edi
[1237]	425930	2	xor     ebp, esi
[1238]	425932	2	jmp     esp
[1239]	425934	4	cmp     dword ptr [esp], 0
[1240]	425938	2	jnz -38h (target: 425900)
[1241]	42593a	5	cmp     dword ptr [esp+4], 0FFFFFFFEh
[1242]	42593f	2	jnz -3Fh (target: 425900)
[1243]	425941	5	cmp     dword ptr [esp+8], 11h
[1244]	425946	2	jnz -46h (target: 425900)
[1245]	425948	5	cmp     dword ptr [esp+0Ch], 0
[1246]	42594d	2	jnz -4Dh (target: 425900)
[1247]	42594f	5	cmp     dword ptr [esp+10h], 0
[1248]	425954	2	jnz -54h (target: 425900)
[1249]	425956	6	add     dword_4065A8+4Bh, eax
[1250]	42595c	6	inc     dword_4065A8+4Bh
[1251]	425962	6	add     dword_4065A8+4Fh, eax
[1252]	425968	6	inc     dword_4065A8+4Fh
[1253]	42596e	6	add     dword_4065A8+53h, eax
[1254]	425974	6	inc     dword_4065A8+53h
[1255]	42597a	6	add     dword_4065A8+57h, eax
[1256]	425980	6	inc     dword_4065A8+57h
[1257]	425986	3	add     esp, 14h
[1258]	425989	2	push 00h
[1259]	42598b	5	push 00004000h
[1260]	425990	7	mov     dword ptr [ebp-54h], 0
[1261]	425997	3	lea     eax, [ebp-54h]
[1262]	42599a	1	push eax
[1263]	42599b	3	lea     eax, [ebp-58h]
[1264]	42599e	1	push eax
[1265]	42599f	2	push FFh
[1266]	4259a1	2	push 00h
[1267]	4259a3	5	call VM (402CACh)	 (push    427730h                         ; GO TO VM3)
[1268]	4259a8	3	add     eax, 0Bh
[1269]	4259ab	1	push eax
[1270]	4259ac	3	mov     eax, [ebp-48h]
[1271]	4259af	2	mov edx, esp
[1272]	4259b1	2	sysenter
[1273]	4259b3	5	cmp     eax, 0C00000A0h
[1274]	4259b8	2	jz 45h (target: 4259ff)
[1275]	4259ba	5	cmp     eax, 0C000001Bh
[1276]	4259bf	2	jz 3Eh (target: 4259ff)
[1277]	4259c1	2	test eax, eax
[1278]	4259c3	2	jz 3Ah (target: 4259ff)
[1279]	4259c5	2	add     eax, esi
[1280]	4259c7	2	add     esi, edi
[1281]	4259c9	2	or      edi, eax
[1282]	4259cb	2	add     ebx, esp
[1283]	4259cd	1	cld
[1284]	4259ce	7	mov     edi, large fs:18h
[1285]	4259d5	2	mov eax, 0
[1286]	4259d7	5	mov ecx, 00000400h
[1287]	4259dc	2	rep stosd
[1288]	4259de	3	mov     edi, [ebp-24h]
[1289]	4259e1	3	mov     edi, [edi+0Ch]
[1290]	4259e4	5	mov ecx, 00000400h
[1291]	4259e9	2	rep stosd
[1292]	4259eb	3	mov     edi, [ebp-24h]
[1293]	4259ee	5	mov ecx, 00000400h
[1294]	4259f3	2	rep stosd
[1295]	4259f5	2	or      ebp, ebx
[1296]	4259f7	2	add     esp, ebp
[1297]	4259f9	2	add     esp, edi
[1298]	4259fb	2	add     ebp, esp
[1299]	4259fd	2	jmp     ebp
[1300]	4259ff	3	mov     eax, [ebp-70h]
[1301]	425a02	6	add     dword_4065A8+4Bh, eax
[1302]	425a08	6	inc     dword_4065A8+4Bh
[1303]	425a0e	6	add     dword_4065A8+4Fh, eax
[1304]	425a14	6	inc     dword_4065A8+4Fh
[1305]	425a1a	6	add     dword_4065A8+53h, eax
[1306]	425a20	6	inc     dword_4065A8+53h
[1307]	425a26	6	add     dword_4065A8+57h, eax
[1308]	425a2c	6	inc     dword_4065A8+57h
[1309]	425a32	3	add     esp, 18h
[1310]	425a35	6	mov     eax, large fs:30h
[1311]	425a3b	3	mov     ebx, [eax+34h]
[1312]	425a3e	3	mov     [eax+30h], ebx
[1313]	425a41	2	int     2Ah										; KiGetTickCount()
[1314]	425a43	1	push eax
[1315]	425a44	3	sub     eax, [ebp-64h]
[1316]	425a47	2	test eax, eax
[1317]	425a49	2	jnz 4Dh (target: 425a98)
[1318]	425a4b	1	pop eax
[1319]	425a4c	6	add     dword_4065A8+4Bh, eax
[1320]	425a52	6	add     dword_4065A8+4Fh, eax
[1321]	425a58	6	add     dword_4065A8+53h, eax
[1322]	425a5e	6	add     dword_4065A8+57h, eax
[1323]	425a64	1	cld
[1324]	425a65	7	mov     edi, large fs:18h
[1325]	425a6c	2	mov eax, 0
[1326]	425a6e	5	mov ecx, 00000400h
[1327]	425a73	2	rep stosd
[1328]	425a75	3	mov     edi, [ebp-24h]
[1329]	425a78	3	mov     edi, [edi+0Ch]
[1330]	425a7b	5	mov ecx, 00000400h
[1331]	425a80	2	rep stosd
[1332]	425a82	3	mov     edi, [ebp-24h]
[1333]	425a85	5	mov ecx, 00000400h
[1334]	425a8a	2	rep stosd
[1335]	425a8c	2	xor     eax, esp
[1336]	425a8e	2	xor     esi, ebp
[1337]	425a90	2	or      esp, esi
[1338]	425a92	2	or      ebp, esp
[1339]	425a94	2	jmp     esp
[1340]	425a96	2	jmp 6Eh (target: 425b06)
[1341]	425a98	5	cmp     eax, 0D6D8h
[1342]	425a9d	2	jl 4Eh (target: 425aed)
[1343]	425a9f	6	add     dword_4065A8+4Bh, eax
[1344]	425aa5	6	add     dword_4065A8+4Fh, eax
[1345]	425aab	6	add     dword_4065A8+53h, eax
[1346]	425ab1	6	add     dword_4065A8+57h, eax
[1347]	425ab7	1	cld
[1348]	425ab8	7	mov     edi, large fs:18h
[1349]	425abf	2	mov eax, 0
[1350]	425ac1	5	mov ecx, 00000400h
[1351]	425ac6	2	rep stosd
[1352]	425ac8	3	mov     edi, [ebp-24h]
[1353]	425acb	3	mov     edi, [edi+0Ch]
[1354]	425ace	5	mov ecx, 00000400h
[1355]	425ad3	2	rep stosd
[1356]	425ad5	3	mov     edi, [ebp-24h]
[1357]	425ad8	5	mov ecx, 00000400h
[1358]	425add	2	rep stosd
[1359]	425adf	2	or      esp, esi
[1360]	425ae1	2	xor     ebp, edi
[1361]	425ae3	2	or      esp, ebp
[1362]	425ae5	2	or      ebp, ebp
[1363]	425ae7	2	add     eax, esp
[1364]	425ae9	2	add     eax, ebp
[1365]	425aeb	2	jmp eax
[1366]	425aed	1	pop eax
[1367]	425aee	6	inc     dword_4065A8+4Bh
[1368]	425af4	6	inc     dword_4065A8+4Fh
[1369]	425afa	6	inc     dword_4065A8+53h
[1370]	425b00	6	inc     dword_4065A8+57h
[1371]	425b06	2	push 00h
[1372]	425b08	2	push 04h
[1373]	425b0a	3	lea     eax, [ebp-74h]
[1374]	425b0d	1	push eax
[1375]	425b0e	2	push 07h
[1376]	425b10	2	push FFh
[1377]	425b12	2	push 00h
[1378]	425b14	5	call VM (402CACh)	 (push    427730h                         ; GO TO VM3)
[1379]	425b19	3	add     eax, 0Bh
[1380]	425b1c	1	push eax
[1381]	425b1d	3	mov     eax, [ebp-78h]
[1382]	425b20	2	mov edx, esp
[1383]	425b22	2	sysenter
[1384]	425b24	2	test eax, eax
[1385]	425b26	2	jz 32h (target: 425b5a)
[1386]	425b28	1	cld
[1387]	425b29	7	mov     edi, large fs:18h
[1388]	425b30	2	mov eax, 0
[1389]	425b32	5	mov ecx, 00000400h
[1390]	425b37	2	rep stosd
[1391]	425b39	3	mov     edi, [ebp-24h]
[1392]	425b3c	3	mov     edi, [edi+0Ch]
[1393]	425b3f	5	mov ecx, 00000400h
[1394]	425b44	2	rep stosd
[1395]	425b46	3	mov     edi, [ebp-24h]
[1396]	425b49	5	mov ecx, 00000400h
[1397]	425b4e	2	rep stosd
[1398]	425b50	2	or      esp, ebp
[1399]	425b52	2	or      ebp, ebp
[1400]	425b54	2	sub     eax, edi
[1401]	425b56	2	sub     ebx, esi
[1402]	425b58	2	jmp     ebx
[1403]	425b5a	4	cmp     dword ptr [esp], 0
[1404]	425b5e	2	jnz -36h (target: 425b28)
[1405]	425b60	5	cmp     dword ptr [esp+4], 0FFFFFFFFh
[1406]	425b65	2	jnz -3Dh (target: 425b28)
[1407]	425b67	5	cmp     dword ptr [esp+8], 7
[1408]	425b6c	2	jnz -44h (target: 425b28)
[1409]	425b6e	5	cmp     dword ptr [esp+10h], 4
[1410]	425b73	2	jnz -4Bh (target: 425b28)
[1411]	425b75	5	cmp     dword ptr [esp+14h], 0
[1412]	425b7a	2	jnz -52h (target: 425b28)
[1413]	425b7c	3	add     esp, 18h
[1414]	425b7f	6	add     dword_4065A8+4Bh, eax
[1415]	425b85	6	inc     dword_4065A8+4Bh
[1416]	425b8b	6	add     dword_4065A8+4Fh, eax
[1417]	425b91	6	inc     dword_4065A8+4Fh
[1418]	425b97	6	add     dword_4065A8+53h, eax
[1419]	425b9d	6	inc     dword_4065A8+53h
[1420]	425ba3	6	add     dword_4065A8+57h, eax
[1421]	425ba9	6	inc     dword_4065A8+57h
[1422]	425baf	3	mov     eax, [ebp-74h]
[1423]	425bb2	2	test eax, eax
[1424]	425bb4	2	jz 32h (target: 425be8)
[1425]	425bb6	1	cld
[1426]	425bb7	7	mov     edi, large fs:18h
[1427]	425bbe	2	mov eax, 0
[1428]	425bc0	5	mov ecx, 00000400h
[1429]	425bc5	2	rep stosd
[1430]	425bc7	3	mov     edi, [ebp-24h]
[1431]	425bca	3	mov     edi, [edi+0Ch]
[1432]	425bcd	5	mov ecx, 00000400h
[1433]	425bd2	2	rep stosd
[1434]	425bd4	3	mov     edi, [ebp-24h]
[1435]	425bd7	5	mov ecx, 00000400h
[1436]	425bdc	2	rep stosd
[1437]	425bde	2	or      esp, esi
[1438]	425be0	2	xor     ebp, edi
[1439]	425be2	2	sub     ecx, esi
[1440]	425be4	2	sub     ebx, eax
[1441]	425be6	2	jmp     ecx
[1442]	425be8	3	add     eax, 2
[1443]	425beb	3	shl     eax, 10h
[1444]	425bee	6	add     dword_4065A8+4Bh, eax
[1445]	425bf4	6	inc     dword_4065A8+4Bh
[1446]	425bfa	6	add     dword_4065A8+4Fh, eax
[1447]	425c00	6	inc     dword_4065A8+4Fh
[1448]	425c06	6	add     dword_4065A8+53h, eax
[1449]	425c0c	6	inc     dword_4065A8+53h
[1450]	425c12	6	add     dword_4065A8+57h, eax
[1451]	425c18	6	inc     dword_4065A8+57h
[1452]	425c1e	6	lea     eax, [ebp-200h]
[1453]	425c24	3	add     eax, 10h
[1454]	425c27	2	mov     eax, [eax]
[1455]	425c29	2	test eax, eax
[1456]	425c2b	2	jz 32h (target: 425c5f)
[1457]	425c2d	1	cld
[1458]	425c2e	7	mov     edi, large fs:18h
[1459]	425c35	2	mov eax, 0
[1460]	425c37	5	mov ecx, 00000400h
[1461]	425c3c	2	rep stosd
[1462]	425c3e	3	mov     edi, [ebp-24h]
[1463]	425c41	3	mov     edi, [edi+0Ch]
[1464]	425c44	5	mov ecx, 00000400h
[1465]	425c49	2	rep stosd
[1466]	425c4b	3	mov     edi, [ebp-24h]
[1467]	425c4e	5	mov ecx, 00000400h
[1468]	425c53	2	rep stosd
[1469]	425c55	2	add     esp, edi
[1470]	425c57	2	add     esp, edi
[1471]	425c59	2	add     ebp, edi
[1472]	425c5b	2	add     ebp, esp
[1473]	425c5d	1	push ebx
[1474]	425c5e	1	retn
[1475]	425c5f	6	add     dword_4065A8+4Bh, eax
[1476]	425c65	6	inc     dword_4065A8+4Bh
[1477]	425c6b	6	add     dword_4065A8+4Fh, eax
[1478]	425c71	6	inc     dword_4065A8+4Fh
[1479]	425c77	6	add     dword_4065A8+53h, eax
[1480]	425c7d	6	inc     dword_4065A8+53h
[1481]	425c83	6	add     dword_4065A8+57h, eax
[1482]	425c89	6	inc     dword_4065A8+57h
[1483]	425c8f	3	lea     eax, [ebp-4]
[1484]	425c92	5	mov ecx, 00000080h
[1485]	425c97	7	mov     dword ptr ss:[eax], 0
[1486]	425c9e	3	sub     eax, 4
[1487]	425ca1	2	loop -0Ah (target: 425c97)
[1488]	425ca3	1	pop esi
[1489]	425ca4	1	pop ecx
[1490]	425ca5	1	pop ebx
[1491]	425ca6	1	pop edx
[1492]	425ca7	1	pop eax
[1493]	425ca8	2	mov esp, ebp
[1494]	425caa	1	pop ebp
[1495]	425cab	1	retn