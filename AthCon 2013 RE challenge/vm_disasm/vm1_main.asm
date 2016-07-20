[001]	40cade	1	push edi										; many garbage instructions
[002]	40cadf	2	ja 00h (target: 40cae1)
[003]	40cae1	1	push ecx
[004]	40cae2	5	mov ecx, 00000100h
[005]	40cae7	2		mov edi, ebx
[006]	40cae9	1		dec ecx
[007]	40caea	2		jp 00h (target: 40caec)
[008]	40caec	2		jz 00h (target: 40caee)
[009]	40caee	2		jo 00h (target: 40caf0)
[010]	40caf0	2	loop -09h (target: 40cae7)
[011]	40caf2	1	pop ecx
[012]	40caf3	1	pop edi
[013]	40caf4	1	push eax
[014]	40caf5	1	push ebx
[015]	40caf6	1	push ecx
[016]	40caf7	1	push edx
[017]	40caf8	1	push esi
[018]	40caf9	1	push edi
[019]	40cafa	5	mov eax, 00000074h
[020]	40caff	2	mov ebx, ecx
[021]	40cb01	5	push 78657464h
[022]	40cb06	1	pop esi
[023]	40cb07	2	jmp 02h (target: 40cb0b)
[024]	40cb09	2	mov eax, 0
[025]	40cb0b	2	mov byte al, 0
[026]	40cb0d	1	push edi
[027]	40cb0e	1	xchg    eax, ebx
[028]	40cb0f	1	pop ecx
[029]	40cb10	1	dec ecx
[030]	40cb11	1	pop edi
[031]	40cb12	1	pop esi
[032]	40cb13	1	pop edx
[033]	40cb14	1	pop ecx
[034]	40cb15	1	pop ebx
[035]	40cb16	1	pop eax
[036]	40cb17	5	call VM (401ADEh)	 (push    offset loc_41F730               ; initialize virtual registers (VM2))
[037]	40cb1c	1	push ebp
[038]	40cb1d	2	mov ebp, esp
[039]	40cb1f	3	sub     esp, 34h
[040]	40cb22	1	push eax
[041]	40cb23	6	mov     eax, large fs:30h
[042]	40cb29	3	mov     [ebp-4], eax
[043]	40cb2c	3	mov     eax, [eax+0Ch]
[044]	40cb2f	3	mov     eax, [eax+1Ch]
[045]	40cb32	1	push ebx
[046]	40cb33	3	mov     ebx, [eax+8]
[047]	40cb36	1	push edx
[048]	40cb37	3	mov     edx, [eax+20h]
[049]	40cb3a	2	mov     eax, [eax]
[050]	40cb3c	3	mov     [ebp-8], ebx
[051]	40cb3f	2	mov eax, ebx
[052]	40cb41	1	push ecx
[053]	40cb42	2	mov ecx, ebx
[054]	40cb44	3	mov     edx, [ecx+3Ch]
[055]	40cb47	4	lea     ecx, [edx+eax+4]
[056]	40cb4b	2	mov edx, ecx
[057]	40cb4d	3	add     edx, 14h
[058]	40cb50	2	mov eax, edx
[059]	40cb52	3	mov     ecx, [ebp-8]
[060]	40cb55	3	add     ecx, [eax+60h]
[061]	40cb58	3	mov     [ebp-0Ch], ecx
[062]	40cb5b	3	add     ecx, 24h
[063]	40cb5e	2	mov     ecx, [ecx]
[064]	40cb60	3	mov     eax, [ebp-0Ch]
[065]	40cb63	3	add     eax, 20h
[066]	40cb66	2	mov     eax, [eax]
[067]	40cb68	2	sub     ecx, eax
[068]	40cb6a	3	sub     ecx, 4
[069]	40cb6d	3	mov     [ebp-10h], ecx
[070]	40cb70	3	mov     eax, [ebp-0Ch]
[071]	40cb73	3	add     eax, 24h
[072]	40cb76	2	mov     eax, [eax]
[073]	40cb78	3	sub     eax, 4
[074]	40cb7b	3	add     eax, [ebp-8]
[075]	40cb7e	3	mov     [ebp-14h], eax
[076]	40cb81	3	sub     eax, [ebp-10h]
[077]	40cb84	3	sub     eax, 4
[078]	40cb87	3	mov     [ebp-18h], eax
[079]	40cb8a	1	push esi
[080]	40cb8b	5	call VM (401ADEh)	 (push    offset loc_41F730               ; initialize virtual registers (VM2))
; -----------------------------------------------------------------------------------------------------------
[081]	40cb90	2	jmp 08h (target: 40cb9a)
[082]	40cb92	4	sub     dword ptr [ebp-14h], 4
[083]	40cb96	4	sub     dword ptr [ebp-18h], 4					; move on the next function name
[084]	40cb9a	3	mov     eax, [ebp-8]
[085]	40cb9d	3	mov     ebx, [ebp-14h]
[086]	40cba0	2	mov     ebx, [ebx]
[087]	40cba2	2	add     eax, ebx								; get exported function name
[088]	40cba4	3	mov     ebx, [ebp-8]
[089]	40cba7	3	mov     edx, [ebp-18h]
[090]	40cbaa	2	mov     edx, [edx]
[091]	40cbac	2	add     ebx, edx
[092]	40cbae	3	mov     [ebp-1Ch], ebx							; address of current function
[093]	40cbb1	2	mov esi, 0
[094]	40cbb3	2	mov ebx, 0
[095]	40cbb5	3		movzx   ebx, byte ptr [eax]					; get a checksum of current name
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
; -----------------------------------------------------------------------------------------------------------
[107]	40cbd0	5	cmp     eax, 946CE828h							; ZwUnmapViewOfSection() ?
[108]	40cbd5	2	jnz 15h (target: 40cbec)
[109]	40cbd7	5	call VM (401ADEh)	 (push    offset loc_41F730               ; initialize virtual registers (VM2))
[110]	40cbdc	3	mov     eax, [ebp-1Ch]
[111]	40cbdf	1	inc eax
[112]	40cbe0	2	mov     eax, [eax]
[113]	40cbe2	3	mov     [ebp-30h], eax							; store service number on loc30 
[114]	40cbe5	7	mov     dword ptr [ebp-1Ch], 0
; -----------------------------------------------------------------------------------------------------------
[115]	40cbec	5	cmp     eax, 5F43B254h							; ZwMapViewOfSection() ?
[116]	40cbf1	2	jnz 15h (target: 40cc08)
[117]	40cbf3	5	call VM (401ADEh)	 (push    offset loc_41F730               ; initialize virtual registers (VM2))
[118]	40cbf8	3	mov     eax, [ebp-1Ch]
[119]	40cbfb	1	inc eax
[120]	40cbfc	2	mov     eax, [eax]
[121]	40cbfe	3	mov     [ebp-20h], eax
[122]	40cc01	7	mov     dword ptr [ebp-1Ch], 0
; -----------------------------------------------------------------------------------------------------------
[123]	40cc08	5	cmp     eax, 0A7AFD948h							; ZwCreateSection() ?
[124]	40cc0d	2	jnz 15h (target: 40cc24)
[125]	40cc0f	5	call VM (401ADEh)	 (push    offset loc_41F730               ; initialize virtual registers (VM2))
[126]	40cc14	3	mov     eax, [ebp-1Ch]
[127]	40cc17	1	inc eax
[128]	40cc18	2	mov     eax, [eax]
[129]	40cc1a	3	mov     [ebp-24h], eax
[130]	40cc1d	7	mov     dword ptr [ebp-1Ch], 0
; -----------------------------------------------------------------------------------------------------------
[131]	40cc24	5	cmp     eax, 848955ACh							; ZwCreateFile() ?
[132]	40cc29	2	jnz 15h (target: 40cc40)
[133]	40cc2b	5	call VM (401ADEh)	 (push    offset loc_41F730               ; initialize virtual registers (VM2))
[134]	40cc30	3	mov     eax, [ebp-1Ch]
[135]	40cc33	1	inc eax
[136]	40cc34	2	mov     eax, [eax]
[137]	40cc36	3	mov     [ebp-28h], eax
[138]	40cc39	7	mov     dword ptr [ebp-1Ch], 0
; -----------------------------------------------------------------------------------------------------------
[139]	40cc40	5	cmp     eax, 67F17733h							; ZwClose() ?
[140]	40cc45	6	jnz FFFFFF47h (target: 40cb92)
[141]	40cc4b	5	call VM (401ADEh)	 (push    offset loc_41F730               ; initialize virtual registers (VM2))
[142]	40cc50	3	mov     eax, [ebp-1Ch]
[143]	40cc53	1	inc eax
[144]	40cc54	2	mov     eax, [eax]
[145]	40cc56	3	mov     [ebp-34h], eax
[146]	40cc59	7	mov     dword ptr [ebp-1Ch], 0
; -----------------------------------------------------------------------------------------------------------
[147]	40cc60	2	push 01h
[148]	40cc62	2	push 00h
[149]	40cc64	2	push 00h
[150]	40cc66	5	push 00406320h
[151]	40cc6b	2	push 00h
[152]	40cc6d	2	push 00h
[153]	40cc6f	2	push 00h
[154]	40cc71	2	push 00h
[155]	40cc73	5	push 00406320h									; *0x406320 = "g"
[156]	40cc78	5	push 00020002h
[157]	40cc7d	5	mov ecx, 0000000Ch
[158]	40cc82	2	push 00h
[159]	40cc84	2	loop -02h (target: 40cc82)
[160]	40cc86	4	lea     eax, [esp+30h]
[161]	40cc8a	1	push eax
[162]	40cc8b	3	mov     eax, [ebp-4]
[163]	40cc8e	4	mov     eax, ss:[eax+10h]
[164]	40cc92	3	push    dword ptr [eax+2Ch]
[165]	40cc95	2	push 18h
[166]	40cc97	5	mov ecx, 00000006h
[167]	40cc9c	2		push 00h
[168]	40cc9e	2	loop -02h (target: 40cc9c)
[169]	40cca0	2	push 01h
[170]	40cca2	2	push 00h
[171]	40cca4	5	push 00000080h
[172]	40cca9	2	push 00h
[173]	40ccab	4	lea     eax, [esp+6Ch]
[174]	40ccaf	1	push eax
[175]	40ccb0	4	lea     eax, [esp+2Ch]
[176]	40ccb4	1	push eax
[177]	40ccb5	5	push 80100080h	
[178]	40ccba	7	lea     eax, [esp+90h]
[179]	40ccc1	1	push eax	
[180]	40ccc2	2	mov ebx, eax
[181]	40ccc4	2	push 00h
[182]	40ccc6	5	call VM (402CACh)	 (push    offset loc_427730               ; initialize virtual registers (VM3))
[183]	40cccb	3	add     eax, 0Dh
[184]	40ccce	1	push eax
[185]	40cccf	3	lea     eax, [ebp-28h]							; eax service number
[186]	40ccd2	2	mov     eax, [eax]
[187]	40ccd4	2	mov edx, esp									; edx = user space stack
[188]	40ccd6	2	sysenter										; ZwCreateFile() 
[189]	40ccd8	2	mov     eax, [ebx]
[190]	40ccda	6	add     esp, 0A0h
[191]	40cce0	5	mov     dword_4074B0, eax
[192]	40cce5	5	call VM (401ADEh)	 (push    offset loc_41F730               ; initialize virtual registers (VM2))
[193]	40ccea	7	mov     dword ptr [ebp-2Ch], 2
[194]	40ccf1	3	lea     ebx, [ebp-2Ch]
[195]	40ccf4	6	push    dword_4074B0
[196]	40ccfa	5	push 08000000h
[197]	40ccff	2	push 02h
[198]	40cd01	2	push 00h
[199]	40cd03	2	push 00h
[200]	40cd05	5	push 000F0005h
[201]	40cd0a	3	lea     eax, [ebp-2Ch]
[202]	40cd0d	1	push eax
[203]	40cd0e	2	push 00h
[204]	40cd10	5	call VM (402CACh)	 (push    offset loc_427730               ; initialize virtual registers (VM3))
[205]	40cd15	3	add     eax, 0Dh
[206]	40cd18	1	push eax
[207]	40cd19	3	lea     eax, [ebp-24h]
[208]	40cd1c	2	mov     eax, [eax]
[209]	40cd1e	2	mov edx, esp
[210]	40cd20	2	sysenter										; ZwCreateSection()
[211]	40cd22	2	mov     eax, [ebx]
[212]	40cd24	3	add     esp, 20h
[213]	40cd27	5	mov     dword_4074B4, eax
[214]	40cd2c	5	call VM (401ADEh)	 (push    offset loc_41F730               ; initialize virtual registers (VM2))
[215]	40cd31	2	push 00h
[216]	40cd33	2	push 00h
[217]	40cd35	2	push 04h
[218]	40cd37	6	push    dword_4074B4
[219]	40cd3d	2	push 00h
[220]	40cd3f	2	push 00h
[221]	40cd41	2	push 00h
[222]	40cd43	2	push 00h
[223]	40cd45	2	push 02h
[224]	40cd47	2	push 00h
[225]	40cd49	2	push 01h
[226]	40cd4b	4	lea     eax, [esp+24h]
[227]	40cd4f	1	push eax
[228]	40cd50	4	lea     eax, [esp+10h]
[229]	40cd54	1	push eax
[230]	40cd55	2	push 00h
[231]	40cd57	2	push 00h
[232]	40cd59	4	lea     eax, [esp+38h]
[233]	40cd5d	2	mov ebx, eax
[234]	40cd5f	1	push eax
[235]	40cd60	2	push FFh
[236]	40cd62	6	push    dword_4074B4
[237]	40cd68	2	push 00h
[238]	40cd6a	5	call VM (402CACh)	 (push    offset loc_427730               ; initialize virtual registers (VM3))
[239]	40cd6f	3	add     eax, 0Dh
[240]	40cd72	1	push eax
[241]	40cd73	3	lea     eax, [ebp-20h]							; service number
[242]	40cd76	2	mov     eax, [eax]
[243]	40cd78	2	mov edx, esp
[244]	40cd7a	2	sysenter										; ZwMapViewOfSection()
[245]	40cd7c	2	mov     eax, [ebx]
[246]	40cd7e	3	add     esp, 4Ch
[247]	40cd81	3	cmp     eax, 0									; if map fails
[248]	40cd84	6	jz 00000633h (target: 40d3bd)					; jump to bad boy message
[249]	40cd8a	2	jp 00h (target: 40cd8c)
[250]	40cd8c	2	mov esi, eax									; esi points to file contents
[251]	40cd8e	2	jno 00h (target: 40cd90)						; Let KEY be that array
[252]	40cd90	2	ja 00h (target: 40cd92)
[253]	40cd92	1	push ebp
[254]	40cd93	2	mov ebp, esp
[255]	40cd95	2	jb 00h (target: 40cd97)
[256]	40cd97	2	jno 00h (target: 40cd99)
[257]	40cd99	2	jns 00h (target: 40cd9b)
[258]	40cd9b	2	mov esp, esi
[259]	40cd9d	2	jz 00h (target: 40cd9f)
[260]	40cd9f	2	jmp 00h (target: 40cda1)
[261]	40cda1	1	pop eax											; eax = KEY[0:3]
[262]	40cda2	2	jp 00h (target: 40cda4)
[263]	40cda4	2	jo 00h (target: 40cda6)
[264]	40cda6	2	mov esp, ebp
[265]	40cda8	2	jnz 00h (target: 40cdaa)
[266]	40cdaa	2	jno 00h (target: 40cdac)
[267]	40cdac	1	push eax
[268]	40cdad	1	push ebx
[269]	40cdae	1	push ecx
[270]	40cdaf	1	push edx
[271]	40cdb0	1	push esi
[272]	40cdb1	1	push edi
[273]	40cdb2	2	mov ecx, 0
[274]	40cdb4	3	shl     eax, 16h
[275]	40cdb7	6	add     ecx, 100h
[276]	40cdbd	2	mov edi, ebx
[277]	40cdbf	1	dec ecx
[278]	40cdc0	2	loop -03h (target: 40cdbd)
[279]	40cdc2	1	pop edi
[280]	40cdc3	1	pop esi
[281]	40cdc4	1	pop edx
[282]	40cdc5	1	pop ecx
[283]	40cdc6	1	pop ebx
[284]	40cdc7	1	pop eax
[285]	40cdc8	1	inc esi
[286]	40cdc9	2	jb 00h (target: 40cdcb)
[287]	40cdcb	2	jnz 00h (target: 40cdcd)
[288]	40cdcd	2	jnz 00h (target: 40cdcf)
[289]	40cdcf	1	inc esi
[290]	40cdd0	2	jns 00h (target: 40cdd2)
[291]	40cdd2	1	inc esi
[292]	40cdd3	2	jz 00h (target: 40cdd5)
[293]	40cdd5	2	jmp 00h (target: 40cdd7)
[294]	40cdd7	1	inc esi
[295]	40cdd8	2	ja 00h (target: 40cdda)
[296]	40cdda	2	jo 00h (target: 40cddc)
[297]	40cddc	2	mov ebp, esp
[298]	40cdde	2	jmp 00h (target: 40cde0)
[299]	40cde0	2	mov esp, esi
[300]	40cde2	2	jz 00h (target: 40cde4)
[301]	40cde4	2	jno 00h (target: 40cde6)
[302]	40cde6	2	jns 00h (target: 40cde8)
[303]	40cde8	1	pop ebx											; ebx = KEY[4:7]
[304]	40cde9	2	jnz 00h (target: 40cdeb)
[305]	40cdeb	2	mov esp, ebp
[306]	40cded	2	ja 00h (target: 40cdef)
[307]	40cdef	2	ja 00h (target: 40cdf1)
[308]	40cdf1	1	push eax
[309]	40cdf2	1	push ebx
[310]	40cdf3	1	push ecx
[311]	40cdf4	1	push edx
[312]	40cdf5	1	push esi
[313]	40cdf6	1	push edi
[314]	40cdf7	2	mov ebx, edi
[315]	40cdf9	2	mov byte al, cl
[316]	40cdfb	2	jbe 00h (target: 40cdfd)
[317]	40cdfd	2	mov ecx, 0
[318]	40cdff	3	add     edx, 4
[319]	40ce02	2	ja 00h (target: 40ce04)
[320]	40ce04	5	push 87364563h
[321]	40ce09	1	pop edx
[322]	40ce0a	1	pop edi
[323]	40ce0b	1	pop esi
[324]	40ce0c	1	pop edx
[325]	40ce0d	1	pop ecx
[326]	40ce0e	1	pop ebx
[327]	40ce0f	1	pop eax
[328]	40ce10	1	inc esi
[329]	40ce11	2	jz 00h (target: 40ce13)
[330]	40ce13	1	inc esi
[331]	40ce14	2	jns 00h (target: 40ce16)
[332]	40ce16	1	inc esi
[333]	40ce17	2	jp 00h (target: 40ce19)
[334]	40ce19	2	jnz 00h (target: 40ce1b)
[335]	40ce1b	1	inc esi
[336]	40ce1c	2	jb 00h (target: 40ce1e)
[337]	40ce1e	2	jp 00h (target: 40ce20)
[338]	40ce20	2	mov ebp, esp
[339]	40ce22	2	ja 00h (target: 40ce24)
[340]	40ce24	2	jno 00h (target: 40ce26)
[341]	40ce26	2	jns 00h (target: 40ce28)
[342]	40ce28	2	mov esp, esi
[343]	40ce2a	2	jz 00h (target: 40ce2c)
[344]	40ce2c	2	jnz 00h (target: 40ce2e)
[345]	40ce2e	1	pop ecx											; ecx = KEY[8:11]
[346]	40ce2f	2	jmp 00h (target: 40ce31)
[347]	40ce31	2	mov esp, ebp
[348]	40ce33	2	jnz 00h (target: 40ce35)
[349]	40ce35	1	push eax
[350]	40ce36	1	push ebx
[351]	40ce37	1	push ecx
[352]	40ce38	1	push edx
[353]	40ce39	1	push esi
[354]	40ce3a	1	push edi
[355]	40ce3b	2	mov eax, ebx
[356]	40ce3d	1	dec ebx
[357]	40ce3e	2	js 00h (target: 40ce40)
[358]	40ce40	2	mov ecx, 0
[359]	40ce42	2	mov byte bl, dl
[360]	40ce44	2	jns 00h (target: 40ce46)
[361]	40ce46	5	push 37846235h
[362]	40ce4b	3	add     edi, 10h
[363]	40ce4e	1	pop ecx
[364]	40ce4f	1	pop edi
[365]	40ce50	1	pop esi
[366]	40ce51	1	pop edx
[367]	40ce52	1	pop ecx
[368]	40ce53	1	pop ebx
[369]	40ce54	1	pop eax
[370]	40ce55	1	inc esi
[371]	40ce56	2	jnp 00h (target: 40ce58)
[372]	40ce58	2	jz 00h (target: 40ce5a)
[373]	40ce5a	1	inc esi
[374]	40ce5b	2	jo 00h (target: 40ce5d)
[375]	40ce5d	1	inc esi
[376]	40ce5e	2	jb 00h (target: 40ce60)
[377]	40ce60	1	inc esi
[378]	40ce61	2	jz 00h (target: 40ce63)
[379]	40ce63	2	jnz 00h (target: 40ce65)
[380]	40ce65	2	mov ebp, esp
[381]	40ce67	2	jno 00h (target: 40ce69)
[382]	40ce69	2	mov esp, esi
[383]	40ce6b	2	jb 00h (target: 40ce6d)
[384]	40ce6d	2	jnz 00h (target: 40ce6f)
[385]	40ce6f	1	pop edx											; ecx = KEY[12:15]
[386]	40ce70	2	jnp 00h (target: 40ce72)
[387]	40ce72	2	jz 00h (target: 40ce74)
[388]	40ce74	2	mov esp, ebp
[389]	40ce76	2	jnz 00h (target: 40ce78)
[390]	40ce78	1	pop ebp
[391]	40ce79	1	push eax
[392]	40ce7a	1	push ebx
[393]	40ce7b	1	push ecx
[394]	40ce7c	1	push edx
[395]	40ce7d	1	push esi
[396]	40ce7e	1	push edi
[397]	40ce7f	1	dec ebx
[398]	40ce80	1	dec ecx
[399]	40ce81	1	push ecx
[400]	40ce82	1	pop edx
[401]	40ce83	2	jl 00h (target: 40ce85)
[402]	40ce85	3	sub     eax, 4
[403]	40ce88	2	jmp 00h (target: 40ce8a)
[404]	40ce8a	2	jge 00h (target: 40ce8c)
[405]	40ce8c	5	push 90909090h
[406]	40ce91	1	pop eax
[407]	40ce92	1	pop edi
[408]	40ce93	1	pop esi
[409]	40ce94	1	pop edx
[410]	40ce95	1	pop ecx
[411]	40ce96	1	pop ebx
[412]	40ce97	1	pop eax
[413]	40ce98	1	push eax
[414]	40ce99	2	jz 00h (target: 40ce9b)
[415]	40ce9b	2	jz 00h (target: 40ce9d)
[416]	40ce9d	1	push ebx
[417]	40ce9e	2	ja 00h (target: 40cea0)
[418]	40cea0	2	jnz 00h (target: 40cea2)
[419]	40cea2	2	jmp 00h (target: 40cea4)
[420]	40cea4	1	push ecx
[421]	40cea5	2	jnp 00h (target: 40cea7)
[422]	40cea7	2	jns 00h (target: 40cea9)
[423]	40cea9	1	push edx
[424]	40ceaa	2	jo 00h (target: 40ceac)
[425]	40ceac	3	sub     esi, 0Ch
[426]	40ceaf	2	push 00h
[427]	40ceb1	1	push esi
[428]	40ceb2	2	push FFh
[429]	40ceb4	2	push 00h
[430]	40ceb6	5	call VM (402CACh)	 (push    offset loc_427730               ; initialize virtual registers (VM3))
[431]	40cebb	3	add     eax, 0Dh
[432]	40cebe	1	push eax
[433]	40cebf	3	lea     eax, [ebp-30h]
[434]	40cec2	2	mov     eax, [eax]
[435]	40cec4	2	mov edx, esp
[436]	40cec6	2	sysenter										; ZwUnmapViewOfSection()
[437]	40cec8	3	add     esp, 10h
[438]	40cecb	2	mov esi, 0
[439]	40cecd	5	call VM (401ADEh)	 (push    offset loc_41F730               ; initialize virtual registers (VM2))
[440]	40ced2	6	push    dword_4074B4
[441]	40ced8	2	push 00h
[442]	40ceda	5	call VM (402CACh)	 (push    offset loc_427730               ; initialize virtual registers (VM3))
[443]	40cedf	3	add     eax, 0Dh
[444]	40cee2	1	push eax
[445]	40cee3	3	lea     eax, [ebp-34h]
[446]	40cee6	2	mov     eax, [eax]
[447]	40cee8	2	mov edx, esp
[448]	40ceea	2	sysenter										; ZwClose()
[449]	40ceec	3	add     esp, 8
[450]	40ceef	10	mov     dword_4074B4, 0
[451]	40cef9	5	call VM (401ADEh)	 (push    offset loc_41F730               ; initialize virtual registers (VM2))
[452]	40cefe	6	push    dword_4074B0
[453]	40cf04	2	push 00h
[454]	40cf06	5	call VM (402CACh)	 (push    offset loc_427730               ; initialize virtual registers (VM3))
[455]	40cf0b	3	add     eax, 0Dh
[456]	40cf0e	1	push eax
[457]	40cf0f	3	lea     eax, [ebp-34h]
[458]	40cf12	2	mov     eax, [eax]
[459]	40cf14	2	mov edx, esp
[460]	40cf16	2	sysenter										; ZwClose()
[461]	40cf18	3	add     esp, 8
[462]	40cf1b	5	call VM (401ADEh)	 (push    offset loc_41F730               ; initialize virtual registers (VM2))
[463]	40cf20	2	ja 00h (target: 40cf22)
[464]	40cf22	2	jnz 00h (target: 40cf24)
[465]	40cf24	1	pop edx											; edx = KEY[12:15]
[466]	40cf25	2	jo 00h (target: 40cf27)
[467]	40cf27	1	pop ecx											; ecx = KEY[8:11]	
[468]	40cf28	2	jnp 00h (target: 40cf2a)
[469]	40cf2a	2	jno 00h (target: 40cf2c)
[470]	40cf2c	2	jnz 00h (target: 40cf2e)
[471]	40cf2e	1	pop ebx											; ebx = KEY[4:7]
[472]	40cf2f	2	jz 00h (target: 40cf31)
[473]	40cf31	1	pop eax											; eax = KEY[0:3]
[474]	40cf32	2	jb 00h (target: 40cf34)
[475]	40cf34	2	jmp 00h (target: 40cf36)
[476]	40cf36	2	mov esp, ebp
[477]	40cf38	1	pop ebp
[478]	40cf39	5	call VM (401ADEh)	 (push    offset loc_41F730               ; initialize virtual registers (VM2))
[479]	40cf3e	1	push eax
[480]	40cf3f	1	push ebx
[481]	40cf40	1	push ecx
[482]	40cf41	1	push edx
[483]	40cf42	1	push esi
[484]	40cf43	1	push edi
[485]	40cf44	5	mov eax, 00000074h
[486]	40cf49	2	mov ebx, ecx
[487]	40cf4b	5	push 78657464h
[488]	40cf50	1	pop esi
[489]	40cf51	2	jmp 02h (target: 40cf55)
[490]	40cf53	2	mov eax, 0
[491]	40cf55	2	mov byte al, 0
[492]	40cf57	1	push edi
[493]	40cf58	1	xchg    eax, ebx
[494]	40cf59	1	pop ecx
[495]	40cf5a	1	dec ecx
[496]	40cf5b	1	pop edi
[497]	40cf5c	1	pop esi
[498]	40cf5d	1	pop edx
[499]	40cf5e	1	pop ecx
[500]	40cf5f	1	pop ebx
[501]	40cf60	1	pop eax
[502]	40cf61	1	push eax
[503]	40cf62	2	jz 00h (target: 40cf64)
[504]	40cf64	1	push ebx
[505]	40cf65	2	jb 00h (target: 40cf67)
[506]	40cf67	1	push ecx
[507]	40cf68	2	jnz 00h (target: 40cf6a)
[508]	40cf6a	1	push edx
[509]	40cf6b	2	jnp 00h (target: 40cf6d)
[510]	40cf6d	2	jnz 00h (target: 40cf6f)
[511]	40cf6f	2	mov byte dl, 0
[512]	40cf71	2	jnz 00h (target: 40cf73)
[513]	40cf73	1	push eax
[514]	40cf74	1	push ebx
[515]	40cf75	1	push ecx
[516]	40cf76	1	push edx
[517]	40cf77	1	push esi
[518]	40cf78	1	push edi
[519]	40cf79	2	mov ecx, 0
[520]	40cf7b	3	shl     eax, 16h
[521]	40cf7e	6	add     ecx, 100h
[522]	40cf84	2	mov edi, ebx
[523]	40cf86	1	dec ecx
[524]	40cf87	2	loop -03h (target: 40cf84)
[525]	40cf89	1	pop edi
[526]	40cf8a	1	pop esi
[527]	40cf8b	1	pop edx
[528]	40cf8c	1	pop ecx
[529]	40cf8d	1	pop ebx
[530]	40cf8e	1	pop eax
; -----------------------------------------------------------------------------------------------------------
[531]	40cf8f	2	cmp     al, dl									; KEY[0] == NULL ?
[532]	40cf91	2	jnp 00h (target: 40cf93)
[533]	40cf93	6	jz 00000424h (target: 40d3bd)					; jump to bad boy
[534]	40cf99	2	jz 00h (target: 40cf9b)
[535]	40cf9b	3	rol     dword ptr [ecx], 8
[536]	40cf9e	2	ja 00h (target: 40cfa0)
[537]	40cfa0	2	jns 00h (target: 40cfa2)
[538]	40cfa2	2	cmp     al, dl									; KEY[1] == NULL ?
[539]	40cfa4	6	jz 00000413h (target: 40d3bd)					; jump to bad boy
[540]	40cfaa	2	jmp 00h (target: 40cfac)
[541]	40cfac	3	rol     dword ptr [ecx], 8
[542]	40cfaf	2	jz 00h (target: 40cfb1)
[543]	40cfb1	2	jno 00h (target: 40cfb3)
[544]	40cfb3	2	cmp     al, dl									; KEY[2] == NULL ?
[545]	40cfb5	2	jnp 00h (target: 40cfb7)
[546]	40cfb7	2	jz 00h (target: 40cfb9)
[547]	40cfb9	6	jz 000003FEh (target: 40d3bd)					; jump to bad boy
[548]	40cfbf	3	rol     dword ptr [ecx], 8
[549]	40cfc2	2	jo 00h (target: 40cfc4)
[550]	40cfc4	2	cmp     al, dl									; KEY[3] == NULL ?
[551]	40cfc6	2	jl 00h (target: 40cfc8)
[552]	40cfc8	2	jnz 00h (target: 40cfca)
[553]	40cfca	2	jnz 00h (target: 40cfcc)
[554]	40cfcc	6	jz 000003EBh (target: 40d3bd)					; jump to bad boy
[555]	40cfd2	1	push eax
[556]	40cfd3	1	push ebx
[557]	40cfd4	1	push ecx
[558]	40cfd5	1	push edx
[559]	40cfd6	1	push esi
[560]	40cfd7	1	push edi
[561]	40cfd8	2	mov ebx, edi
[562]	40cfda	2	mov byte al, cl
[563]	40cfdc	2	jbe 00h (target: 40cfde)
[564]	40cfde	2	mov ecx, 0
[565]	40cfe0	3	add     edx, 4
[566]	40cfe3	2	ja 00h (target: 40cfe5)
[567]	40cfe5	5	push 87364563h
[568]	40cfea	1	pop edx
[569]	40cfeb	1	pop edi
[570]	40cfec	1	pop esi
[571]	40cfed	1	pop edx
[572]	40cfee	1	pop ecx
[573]	40cfef	1	pop ebx
[574]	40cff0	1	pop eax
[575]	40cff1	2	jz 00h (target: 40cff3)
[576]	40cff3	2	cmp     bl, dl									; if KEY[4:7] is 0 jump to bad boy
[577]	40cff5	2	jl 00h (target: 40cff7)
[578]	40cff7	2	jo 00h (target: 40cff9)
[579]	40cff9	6	jz 000003BEh (target: 40d3bd)
[580]	40cfff	3	shr     ebx, 8
[581]	40d002	2	jnz 00h (target: 40d004)
[582]	40d004	2	jnz 00h (target: 40d006)
[583]	40d006	2	cmp     bl, dl
[584]	40d008	2	ja 00h (target: 40d00a)
[585]	40d00a	6	jz 000003ADh (target: 40d3bd)
[586]	40d010	3	shr     ebx, 8
[587]	40d013	2	jl 00h (target: 40d015)
[588]	40d015	2	cmp     bl, dl
[589]	40d017	2	jb 00h (target: 40d019)
[590]	40d019	6	jz 0000039Eh (target: 40d3bd)
[591]	40d01f	3	shr     ebx, 8
[592]	40d022	2	jl 00h (target: 40d024)
[593]	40d024	2	jns 00h (target: 40d026)
[594]	40d026	2	cmp     bl, dl
[595]	40d028	6	jz 0000038Fh (target: 40d3bd)
[596]	40d02e	1	push eax
[597]	40d02f	1	push ebx
[598]	40d030	1	push ecx
[599]	40d031	1	push edx
[600]	40d032	1	push esi
[601]	40d033	1	push edi
[602]	40d034	1	dec ebx
[603]	40d035	1	dec ecx
[604]	40d036	1	push ecx
[605]	40d037	1	pop edx
[606]	40d038	2	jl 00h (target: 40d03a)
[607]	40d03a	3	sub     eax, 4
[608]	40d03d	2	jmp 00h (target: 40d03f)
[609]	40d03f	2	jge 00h (target: 40d041)
[610]	40d041	5	push 90909090h
[611]	40d046	1	pop eax
[612]	40d047	1	pop edi
[613]	40d048	1	pop esi
[614]	40d049	1	pop edx
[615]	40d04a	1	pop ecx
[616]	40d04b	1	pop ebx
[617]	40d04c	1	pop eax
[618]	40d04d	2	jz 00h (target: 40d04f)
[619]	40d04f	2	cmp     cl, dl									; if KEY[8:11] is 0 jump to bad boy
[620]	40d051	2	jo 00h (target: 40d053)
[621]	40d053	6	jz 00000364h (target: 40d3bd)
[622]	40d059	3	shr     ecx, 8
[623]	40d05c	2	jl 00h (target: 40d05e)
[624]	40d05e	2	jnz 00h (target: 40d060)
[625]	40d060	2	jno 00h (target: 40d062)
[626]	40d062	2	cmp     cl, dl
[627]	40d064	6	jz 00000353h (target: 40d3bd)
[628]	40d06a	3	shr     ecx, 8
[629]	40d06d	2	ja 00h (target: 40d06f)
[630]	40d06f	2	cmp     cl, dl
[631]	40d071	2	jns 00h (target: 40d073)
[632]	40d073	2	jnz 00h (target: 40d075)
[633]	40d075	2	jl 00h (target: 40d077)
[634]	40d077	6	jz 00000340h (target: 40d3bd)
[635]	40d07d	2	jo 00h (target: 40d07f)
[636]	40d07f	3	shr     ecx, 8
[637]	40d082	2	ja 00h (target: 40d084)
[638]	40d084	2	jnz 00h (target: 40d086)
[639]	40d086	2	jmp 00h (target: 40d088)
[640]	40d088	2	cmp     cl, dl
[641]	40d08a	2	jl 00h (target: 40d08c)
[642]	40d08c	2	jge 00h (target: 40d08e)
[643]	40d08e	6	jz 00000329h (target: 40d3bd)
[644]	40d094	1	push eax
[645]	40d095	1	push ebx
[646]	40d096	1	push ecx
[647]	40d097	1	push edx
[648]	40d098	1	push esi
[649]	40d099	1	push edi
[650]	40d09a	2	mov eax, ebx
[651]	40d09c	1	dec ebx
[652]	40d09d	2	js 00h (target: 40d09f)
[653]	40d09f	2	mov ecx, 0
[654]	40d0a1	2	mov byte bl, dl
[655]	40d0a3	2	jns 00h (target: 40d0a5)
[656]	40d0a5	5	push 37846235h
[657]	40d0aa	3	add     edi, 10h
[658]	40d0ad	1	pop ecx
[659]	40d0ae	1	pop edi
[660]	40d0af	1	pop esi
[661]	40d0b0	1	pop edx
[662]	40d0b1	1	pop ecx
[663]	40d0b2	1	pop ebx
[664]	40d0b3	1	pop eax
[665]	40d0b4	2	jz 00h (target: 40d0b6)
[666]	40d0b6	1	pop edx
[667]	40d0b7	2	jge 00h (target: 40d0b9)
[668]	40d0b9	2	jnz 00h (target: 40d0bb)
[669]	40d0bb	1	push edx
[670]	40d0bc	2	jge 00h (target: 40d0be)
[671]	40d0be	2	jge 00h (target: 40d0c0)
[672]	40d0c0	2	mov byte al, 0
[673]	40d0c2	2	js 00h (target: 40d0c4)
[674]	40d0c4	2	jo 00h (target: 40d0c6)
[675]	40d0c6	2	cmp     dl, al									; if KEY[12:15] is 0 jump to bad boy
[676]	40d0c8	6	jz 000002EFh (target: 40d3bd)
[677]	40d0ce	2	jge 00h (target: 40d0d0)
[678]	40d0d0	3	shr     edx, 8
[679]	40d0d3	2	jmp 00h (target: 40d0d5)
[680]	40d0d5	2	cmp     dl, al
[681]	40d0d7	2	jnz 00h (target: 40d0d9)
[682]	40d0d9	6	jz 000002DEh (target: 40d3bd)
[683]	40d0df	3	shr     edx, 8
[684]	40d0e2	2	js 00h (target: 40d0e4)
[685]	40d0e4	2	jz 00h (target: 40d0e6)
[686]	40d0e6	2	cmp     dl, al
[687]	40d0e8	2	jge 00h (target: 40d0ea)
[688]	40d0ea	6	jz 000002CDh (target: 40d3bd)
[689]	40d0f0	3	shr     edx, 8
[690]	40d0f3	2	jns 00h (target: 40d0f5)
[691]	40d0f5	2	cmp     dl, al
[692]	40d0f7	2	jnz 00h (target: 40d0f9)
[693]	40d0f9	6	jz 000002BEh (target: 40d3bd)					; CONCLUSION: KEY must be 16 characters
; -----------------------------------------------------------------------------------------------------------
[694]	40d0ff	2	jge 00h (target: 40d101)
[695]	40d101	2	jz 00h (target: 40d103)
[696]	40d103	1	pop edx
[697]	40d104	2	jnz 00h (target: 40d106)
[698]	40d106	2	js 00h (target: 40d108)
[699]	40d108	1	pop ecx
[700]	40d109	2	jle 00h (target: 40d10b)
[701]	40d10b	2	jmp 00h (target: 40d10d)
[702]	40d10d	1	pop ebx
[703]	40d10e	2	jo 00h (target: 40d110)
[704]	40d110	1	pop eax
[705]	40d111	1	push edi
[706]	40d112	2	jle 00h (target: 40d114)
[707]	40d114	1	push ecx
[708]	40d115	2	jb 00h (target: 40d117)
[709]	40d117	5	mov ecx, 00000100h
[710]	40d11c	2	mov edi, ebx
[711]	40d11e	2	js 00h (target: 40d120)
[712]	40d120	1	dec ecx
[713]	40d121	2	loop -05h (target: 40d11c)
[714]	40d123	1	pop ecx
[715]	40d124	2	jns 00h (target: 40d126)
[716]	40d126	1	pop edi
[717]	40d127	2	xor eax, ebx									; eax = KEY[0:3] ^ KEY[4:7]
[718]	40d129	2	jz 00h (target: 40d12b)
[719]	40d12b	1	push eax
[720]	40d12c	1	push ebx
[721]	40d12d	1	push ecx
[722]	40d12e	1	push edx
[723]	40d12f	1	push esi
[724]	40d130	1	push edi
[725]	40d131	2	mov ecx, 0
[726]	40d133	3	shl     eax, 16h
[727]	40d136	6	add     ecx, 100h
[728]	40d13c	2	mov edi, ebx
[729]	40d13e	1	dec ecx
[730]	40d13f	2	loop -03h (target: 40d13c)
[731]	40d141	1	pop edi
[732]	40d142	1	pop esi
[733]	40d143	1	pop edx
[734]	40d144	1	pop ecx
[735]	40d145	1	pop ebx
[736]	40d146	1	pop eax
[737]	40d147	2	xor eax, ecx									; eax = KEY[0:3] ^ KEY[4:7] ^ KEY[8:11]
[738]	40d149	1	push eax
[739]	40d14a	1	push ebx
[740]	40d14b	1	push ecx
[741]	40d14c	1	push edx
[742]	40d14d	1	push esi
[743]	40d14e	1	push edi
[744]	40d14f	5	mov eax, 00000074h
[745]	40d154	2	mov ebx, ecx
[746]	40d156	5	push 78657464h
[747]	40d15b	1	pop esi
[748]	40d15c	2	jmp 02h (target: 40d160)
[749]	40d15e	2	mov eax, 0
[750]	40d160	2	mov byte al, 0
[751]	40d162	1	push edi
[752]	40d163	1	xchg    eax, ebx
[753]	40d164	1	pop ecx
[754]	40d165	1	dec ecx
[755]	40d166	1	pop edi
[756]	40d167	1	pop esi
[757]	40d168	1	pop edx
[758]	40d169	1	pop ecx
[759]	40d16a	5	call VM (401ADEh)	 (push    offset loc_41F730               ; initialize virtual registers (VM2))
[760]	40d16f	1	pop ebx
[761]	40d170	1	pop eax
[762]	40d171	2	jle 00h (target: 40d173)
[763]	40d173	2	jb 00h (target: 40d175)
[764]	40d175	2	xor eax, edx									; eax = KEY[0:3] ^ KEY[4:7] ^ KEY[8:11] ^ KEY[12:15] = KEYS
[765]	40d177	1	push eax
[766]	40d178	1	push ebx
[767]	40d179	1	push ecx
[768]	40d17a	1	push edx
[769]	40d17b	1	push esi
[770]	40d17c	1	push edi
[771]	40d17d	1	dec ebx
[772]	40d17e	1	dec ecx
[773]	40d17f	1	push ecx
[774]	40d180	1	pop edx
[775]	40d181	2	jl 00h (target: 40d183)
[776]	40d183	3	sub     eax, 4
[777]	40d186	2	jmp 00h (target: 40d188)
[778]	40d188	2	jge 00h (target: 40d18a)
[779]	40d18a	5	push 90909090h
[780]	40d18f	1	pop eax
[781]	40d190	1	pop edi
[782]	40d191	1	pop esi
[783]	40d192	1	pop edx
[784]	40d193	1	pop ecx
[785]	40d194	1	pop ebx
[786]	40d195	1	pop eax
; -----------------------------------------------------------------------------------------------------------
[787]	40d196	2	jnz 00h (target: 40d198)
[788]	40d198	5	push 004065F3h
[789]	40d19d	2	jno 00h (target: 40d19f)
[790]	40d19f	1	pop edi											; edi = 0x4065f3
[791]	40d1a0	2	jle 00h (target: 40d1a2)
[792]	40d1a2	2	jmp 00h (target: 40d1a4)
[793]	40d1a4	2	mov ebp, esp
[794]	40d1a6	2	jle 00h (target: 40d1a8)
[795]	40d1a8	2	jnz 00h (target: 40d1aa)
[796]	40d1aa	2	mov esp, edi
[797]	40d1ac	2	js 00h (target: 40d1ae)
[798]	40d1ae	1	pop ebx											; ebx = *0x4065f3
[799]	40d1af	2	jz 00h (target: 40d1b1)
[800]	40d1b1	2	mov esp, ebp
[801]	40d1b3	1	push eax
[802]	40d1b4	1	push ebx
[803]	40d1b5	1	push ecx
[804]	40d1b6	1	push edx
[805]	40d1b7	1	push esi
[806]	40d1b8	1	push edi
[807]	40d1b9	5	mov eax, 00000074h
[808]	40d1be	2	mov ebx, ecx
[809]	40d1c0	5	push 78657464h
[810]	40d1c5	1	pop esi
[811]	40d1c6	2	jmp 02h (target: 40d1ca)
[812]	40d1c8	2	mov eax, 0
[813]	40d1ca	2	mov byte al, 0
[814]	40d1cc	1	push edi
[815]	40d1cd	1	xchg    eax, ebx
[816]	40d1ce	1	pop ecx
[817]	40d1cf	1	dec ecx
[818]	40d1d0	1	pop edi
[819]	40d1d1	1	pop esi
[820]	40d1d2	1	pop edx
[821]	40d1d3	1	pop ecx
[822]	40d1d4	1	pop ebx
[823]	40d1d5	1	pop eax
[824]	40d1d6	2	xor eax, ebx									; eax = KEYS ^ *0x4065f3
[825]	40d1d8	1	push eax
[826]	40d1d9	1	push ebx
[827]	40d1da	1	push ecx
[828]	40d1db	1	push edx
[829]	40d1dc	1	push esi
[830]	40d1dd	1	push edi
[831]	40d1de	2	mov ecx, 0
[832]	40d1e0	3	shl     eax, 16h
[833]	40d1e3	6	add     ecx, 100h
[834]	40d1e9	2	mov edi, ebx
[835]	40d1eb	1	dec ecx
[836]	40d1ec	2	loop -03h (target: 40d1e9)
[837]	40d1ee	1	pop edi
[838]	40d1ef	1	pop esi
[839]	40d1f0	1	pop edx
[840]	40d1f1	1	pop ecx
[841]	40d1f2	1	pop ebx
[842]	40d1f3	1	pop eax
[843]	40d1f4	2	jle 00h (target: 40d1f6)
[844]	40d1f6	1	inc edi
[845]	40d1f7	2	jb 00h (target: 40d1f9)
[846]	40d1f9	1	inc edi
[847]	40d1fa	2	jns 00h (target: 40d1fc)
[848]	40d1fc	1	inc edi
[849]	40d1fd	2	jnz 00h (target: 40d1ff)
[850]	40d1ff	1	inc edi
[851]	40d200	2	jbe 00h (target: 40d202)
[852]	40d202	2	jle 00h (target: 40d204)
[853]	40d204	2	mov ebp, esp
[854]	40d206	2	jno 00h (target: 40d208)
[855]	40d208	2	mov esp, edi									; edi = 0x4065f3+4
[856]	40d20a	2	jbe 00h (target: 40d20c)
[857]	40d20c	1	pop ecx											; ecx = *0x4065f7
[858]	40d20d	2	jg 00h (target: 40d20f)
[859]	40d20f	2	jns 00h (target: 40d211)
[860]	40d211	2	mov esp, ebp
[861]	40d213	2	jbe 00h (target: 40d215)
[862]	40d215	1	push eax
[863]	40d216	1	push ebx
[864]	40d217	1	push ecx
[865]	40d218	1	push edx
[866]	40d219	1	push esi
[867]	40d21a	1	push edi
[868]	40d21b	2	mov ebx, edi
[869]	40d21d	2	mov byte al, cl
[870]	40d21f	2	jbe 00h (target: 40d221)
[871]	40d221	2	mov ecx, 0
[872]	40d223	3	add     edx, 4
[873]	40d226	2	ja 00h (target: 40d228)
[874]	40d228	5	push 87364563h
[875]	40d22d	1	pop edx
[876]	40d22e	1	pop edi
[877]	40d22f	1	pop esi
[878]	40d230	1	pop edx
[879]	40d231	1	pop ecx
[880]	40d232	1	pop ebx
[881]	40d233	1	pop eax
[882]	40d234	2	xor eax, ecx									; eax = KEYS ^ *0x4065f3 ^ *0x4065f7
[883]	40d236	2	jg 00h (target: 40d238)
[884]	40d238	1	push eax
[885]	40d239	1	push ebx
[886]	40d23a	1	push ecx
[887]	40d23b	1	push edx
[888]	40d23c	1	push esi
[889]	40d23d	1	push edi
[890]	40d23e	2	mov eax, ebx
[891]	40d240	1	dec ebx
[892]	40d241	2	js 00h (target: 40d243)
[893]	40d243	2	mov ecx, 0
[894]	40d245	2	mov byte bl, dl
[895]	40d247	2	jns 00h (target: 40d249)
[896]	40d249	5	push 37846235h
[897]	40d24e	5	call VM (401ADEh)	 (push    offset loc_41F730               ; initialize virtual registers (VM2))
[898]	40d253	3	add     edi, 10h
[899]	40d256	1	pop ecx
[900]	40d257	1	pop edi
[901]	40d258	1	pop esi
[902]	40d259	1	pop edx
[903]	40d25a	1	pop ecx
[904]	40d25b	1	pop ebx
[905]	40d25c	1	pop eax
[906]	40d25d	1	inc edi
[907]	40d25e	2	js 00h (target: 40d260)
[908]	40d260	1	inc edi
[909]	40d261	2	jbe 00h (target: 40d263)
[910]	40d263	2	jmp 00h (target: 40d265)
[911]	40d265	1	inc edi
[912]	40d266	2	jg 00h (target: 40d268)
[913]	40d268	1	inc edi
[914]	40d269	2	jbe 00h (target: 40d26b)
[915]	40d26b	2	jg 00h (target: 40d26d)
[916]	40d26d	2	mov ebp, esp
[917]	40d26f	2	jz 00h (target: 40d271)
[918]	40d271	2	jb 00h (target: 40d273)
[919]	40d273	2	mov esp, edi									; edi = 0x4065f7+4
[920]	40d275	2	js 00h (target: 40d277)
[921]	40d277	1	pop edx											; edx = *0x4065fb
[922]	40d278	2	jno 00h (target: 40d27a)
[923]	40d27a	2	mov esp, ebp
[924]	40d27c	1	push eax
[925]	40d27d	1	push ebx
[926]	40d27e	1	push ecx
[927]	40d27f	1	push edx
[928]	40d280	1	push esi
[929]	40d281	1	push edi
[930]	40d282	2	mov eax, ebx
[931]	40d284	1	dec ebx
[932]	40d285	2	js 00h (target: 40d287)
[933]	40d287	2	mov ecx, 0
[934]	40d289	2	mov byte bl, dl
[935]	40d28b	2	jns 00h (target: 40d28d)
[936]	40d28d	5	push 37846235h
[937]	40d292	3	add     edi, 10h
[938]	40d295	1	pop ecx
[939]	40d296	1	pop edi
[940]	40d297	1	pop esi
[941]	40d298	1	pop edx
[942]	40d299	1	pop ecx
[943]	40d29a	1	pop ebx
[944]	40d29b	1	pop eax
[945]	40d29c	2	xor eax, edx									; eax = KEYS ^ *0x4065f3 ^ *0x4065f7 ^ *0x4065fb
[946]	40d29e	1	push eax
[947]	40d29f	1	push ebx
[948]	40d2a0	1	push ecx
[949]	40d2a1	1	push edx
[950]	40d2a2	1	push esi
[951]	40d2a3	1	push edi
[952]	40d2a4	1	dec ebx
[953]	40d2a5	1	dec ecx
[954]	40d2a6	1	push ecx
[955]	40d2a7	1	pop edx
[956]	40d2a8	2	jl 00h (target: 40d2aa)
[957]	40d2aa	3	sub     eax, 4
[958]	40d2ad	2	jmp 00h (target: 40d2af)
[959]	40d2af	2	jge 00h (target: 40d2b1)
[960]	40d2b1	5	push 90909090h
[961]	40d2b6	1	pop eax
[962]	40d2b7	1	pop edi
[963]	40d2b8	1	pop esi
[964]	40d2b9	1	pop edx
[965]	40d2ba	1	pop ecx
[966]	40d2bb	1	pop ebx
[967]	40d2bc	1	pop eax
[968]	40d2bd	2	jmp 00h (target: 40d2bf)
[969]	40d2bf	1	inc edi
[970]	40d2c0	2	jb 00h (target: 40d2c2)
[971]	40d2c2	1	inc edi
[972]	40d2c3	2	jo 00h (target: 40d2c5)
[973]	40d2c5	1	inc edi
[974]	40d2c6	2	jg 00h (target: 40d2c8)
[975]	40d2c8	2	jbe 00h (target: 40d2ca)
[976]	40d2ca	1	inc edi
[977]	40d2cb	2	mov ebp, esp
[978]	40d2cd	2	js 00h (target: 40d2cf)
[979]	40d2cf	2	mov esp, edi									; edi = 0x4065fb+4
[980]	40d2d1	2	jz 00h (target: 40d2d3)
[981]	40d2d3	1	pop edx											; edx = *0x4065ff
[982]	40d2d4	2	jg 00h (target: 40d2d6)
[983]	40d2d6	2	mov esp, ebp
[984]	40d2d8	1	push eax
[985]	40d2d9	1	push ebx
[986]	40d2da	1	push ecx
[987]	40d2db	1	push edx
[988]	40d2dc	1	push esi
[989]	40d2dd	1	push edi
[990]	40d2de	5	call VM (401ADEh)	 (push    offset loc_41F730               ; initialize virtual registers (VM2))
[991]	40d2e3	5	mov eax, 00000074h
[992]	40d2e8	2	mov ebx, ecx
[993]	40d2ea	5	push 78657464h
[994]	40d2ef	1	pop esi
[995]	40d2f0	2	jmp 02h (target: 40d2f4)
[996]	40d2f2	2	mov eax, 0
[997]	40d2f4	2	mov byte al, 0
[998]	40d2f6	1	push edi
[999]	40d2f7	1	xchg    eax, ebx
[1000]	40d2f8	1	pop ecx
[1001]	40d2f9	1	dec ecx
[1002]	40d2fa	1	pop edi
[1003]	40d2fb	1	pop esi
[1004]	40d2fc	1	pop edx
[1005]	40d2fd	1	pop ecx
[1006]	40d2fe	1	pop ebx
[1007]	40d2ff	1	pop eax
[1008]	40d300	2	xor eax, edx									; eax = KEYS ^ *0x4065f3 ^ *0x4065f7 ^ *0x4065fb ^ *0x4065ff
[1009]	40d302	2	jg 00h (target: 40d304)
[1010]	40d304	2	jbe 00h (target: 40d306)
[1011]	40d306	1	push eax
[1012]	40d307	1	push ecx
[1013]	40d308	1	push edx
[1014]	40d309	1	push esi
[1015]	40d30a	1	push edi
[1016]	40d30b	2	mov ecx, 0
[1017]	40d30d	6	add     ecx, 100h
[1018]	40d313	2	mov edi, ebx
[1019]	40d315	1	dec ecx
[1020]	40d316	2	loop -03h (target: 40d313)
[1021]	40d318	1	pop edi
[1022]	40d319	1	pop esi
[1023]	40d31a	1	pop edx
[1024]	40d31b	1	pop ecx
[1025]	40d31c	1	pop eax
[1026]	40d31d	5	sub     eax, 4E1A9001h							; eax = KEYS^*0x4065f3^*0x4065f7^*0x4065fb^*0x4065ff - 0x4e1a9001
; -----------------------------------------------------------------------------------------------------------
[1027]	40d322	2	mov ebp, esp
[1028]	40d324	2	jbe 00h (target: 40d326)
[1029]	40d326	2	js 00h (target: 40d328)
[1030]	40d328	5	push 004066B3h
[1031]	40d32d	2	jb 00h (target: 40d32f)
[1032]	40d32f	1	pop esp
[1033]	40d330	2	jg 00h (target: 40d332)
[1034]	40d332	1	inc esp
[1035]	40d333	1	inc esp
[1036]	40d334	2	jg 00h (target: 40d336)
[1037]	40d336	1	inc esp
[1038]	40d337	2	jg 00h (target: 40d339)
[1039]	40d339	2	jg 00h (target: 40d33b)
[1040]	40d33b	1	inc esp
[1041]	40d33c	2	jmp 00h (target: 40d33e)
[1042]	40d33e	1	push eax
[1043]	40d33f	2	jz 00h (target: 40d341)
[1044]	40d341	2	mov esp, ebp
[1045]	40d343	2	jns 00h (target: 40d345)
[1046]	40d345	2	jns 00h (target: 40d347)
[1026]	40d31d	5	sub     eax, 4E1A9001h
[1027]	40d322	2	mov ebp, esp
[1028]	40d324	2	jbe 00h (target: 40d326)
[1029]	40d326	2	js 00h (target: 40d328)
[1030]	40d328	5	push 004066B3h
[1031]	40d32d	2	jb 00h (target: 40d32f)
[1032]	40d32f	1	pop esp
[1033]	40d330	2	jg 00h (target: 40d332)
[1034]	40d332	1	inc esp
[1035]	40d333	1	inc esp
[1036]	40d334	2	jg 00h (target: 40d336)
[1037]	40d336	1	inc esp
[1038]	40d337	2	jg 00h (target: 40d339)
[1039]	40d339	2	jg 00h (target: 40d33b)
[1040]	40d33b	1	inc esp
[1041]	40d33c	2	jmp 00h (target: 40d33e)
[1042]	40d33e	1	push eax
[1043]	40d33f	2	jz 00h (target: 40d341)
[1044]	40d341	2	mov esp, ebp
[1045]	40d343	2	jns 00h (target: 40d345)
[1046]	40d345	2	jns 00h (target: 40d347)
[1047]	40d347	1	push eax										; arg3: eax
[1048]	40d348	2	jb 00h (target: 40d34a)
[1049]	40d34a	2	jnb 00h (target: 40d34c)
[1050]	40d34c	2	jns 00h (target: 40d34e)
[1051]	40d34e	5	push 000000A0h
[1052]	40d353	2	jno 00h (target: 40d355)
[1053]	40d355	1	pop eax
[1054]	40d356	2	jnb 00h (target: 40d358)
[1055]	40d358	2	jmp 00h (target: 40d35a)
[1056]	40d35a	5	call VM (401ADEh)	 (push    offset START_EMULATOR_2_41F730  ; XREF FROM 0x0040C9BA)
[1057]	40d35f	1	push eax										; arg2: 0xa0
[1058]	40d360	5	push 00406663h									; arg1: 0x406663
[1059]	40d365	5	call VM (401000h)	 (push    offset loc_40F730               ; initialize virtual registers (VM4))
[1060]	40d36a	1	push eax										; store return value
[1061]	40d36b	1	push ebx
[1062]	40d36c	1	push ecx
[1063]	40d36d	1	push edx
[1064]	40d36e	1	push esi
[1065]	40d36f	1	push edi
[1066]	40d370	2	mov eax, ebx
[1067]	40d372	1	dec ebx
[1068]	40d373	2	js 00h (target: 40d375)
[1069]	40d375	2	mov ecx, 0
[1070]	40d377	2	mov byte bl, dl
[1071]	40d379	2	jns 00h (target: 40d37b)
[1072]	40d37b	5	push 37846235h
[1073]	40d380	3	add     edi, 10h
[1074]	40d383	1	pop ecx
[1075]	40d384	1	pop edi
[1076]	40d385	1	pop esi
[1077]	40d386	1	pop edx
[1078]	40d387	1	pop ecx
[1079]	40d388	1	pop ebx
[1080]	40d389	1	pop eax
[1081]	40d38a	5	push A640740Eh
[1082]	40d38f	1	push eax
[1083]	40d390	1	push ebx
[1084]	40d391	1	push ecx
[1085]	40d392	1	push edx
[1086]	40d393	1	push esi
[1087]	40d394	1	push edi
[1088]	40d395	1	dec ebx
[1089]	40d396	1	dec ecx
[1090]	40d397	1	push ecx
[1091]	40d398	1	pop edx
[1092]	40d399	2	jl 00h (target: 40d39b)
[1093]	40d39b	3	sub     eax, 4
[1094]	40d39e	2	jmp 00h (target: 40d3a0)
[1095]	40d3a0	2	jge 00h (target: 40d3a2)
[1096]	40d3a2	5	push 90909090h
[1097]	40d3a7	1	pop eax
[1098]	40d3a8	1	pop edi
[1099]	40d3a9	1	pop esi
[1100]	40d3aa	1	pop edx
[1101]	40d3ab	1	pop ecx
[1102]	40d3ac	1	pop ebx
[1103]	40d3ad	1	pop eax
[1104]	40d3ae	2	jnb 00h (target: 40d3b0)
[1105]	40d3b0	1	pop ebx											; ebx = 0xA640740E
[1106]	40d3b1	2	jg 00h (target: 40d3b3)
[1107]	40d3b3	2	jns 00h (target: 40d3b5)
[1108]	40d3b5	2	jz 00h (target: 40d3b7)
[1109]	40d3b7	2	cmp eax, ebx									; ret value from 0x401000 == 0xA640740E ?
[1110]	40d3b9	2	jno 00h (target: 40d3bb)
[1111]	40d3bb	2	jz 07h (target: 40d3c4)							; if not go to the bad boy message
[1112]	40d3bd	2		jnb 00h (target: 40d3bf)					; the bad boy message
[1113]	40d3bf	5		call VM (401A5Eh)	 (push    offset loc_417730               ; initialize virtual registers (VM5))
[1114]	40d3c4	2	jbe 00h (target: 40d3c6)
[1115]	40d3c6	2	jnb 00h (target: 40d3c8)
[1116]	40d3c8	2	jno 00h (target: 40d3ca)
[1117]	40d3ca	2	jns 00h (target: 40d3cc)
[1118]	40d3cc	2	jg 00h (target: 40d3ce)
[1119]	40d3ce	1	push eax
[1120]	40d3cf	2	jbe 00h (target: 40d3d1)
[1121]	40d3d1	1	push ecx
[1122]	40d3d2	2	jnb 00h (target: 40d3d4)
[1123]	40d3d4	5	mov ecx, 00000034h
[1124]	40d3d9	2		mov eax, edi
[1125]	40d3db	1		dec ecx
[1126]	40d3dc	2		jg 00h (target: 40d3de)
[1127]	40d3de	2		jns 00h (target: 40d3e0)
[1128]	40d3e0	2	loopne -07h (target: 40d3d9)
[1129]	40d3e2	2	jnb 00h (target: 40d3e4)
[1130]	40d3e4	1	pop ecx
[1131]	40d3e5	2	jg 00h (target: 40d3e7)
[1132]	40d3e7	1	pop eax
[1133]	40d3e8	2	jg 00h (target: 40d3ea)
[1134]	40d3ea	2	jge 00h (target: 40d3ec)
[1135]	40d3ec	1	pop eax
[1136]	40d3ed	2	jge 00h (target: 40d3ef)
[1137]	40d3ef	3	add     ecx, 4
[1138]	40d3f2	1	push eax
[1139]	40d3f3	1	push ebx
[1140]	40d3f4	1	push ecx
[1141]	40d3f5	1	push edx
[1142]	40d3f6	1	push esi
[1143]	40d3f7	1	push edi
[1144]	40d3f8	1	dec ebx
[1145]	40d3f9	1	dec ecx
[1146]	40d3fa	1	push ecx
[1147]	40d3fb	1	pop edx
[1148]	40d3fc	2	jl 00h (target: 40d3fe)
[1149]	40d3fe	3	sub     eax, 4
[1150]	40d401	2	jmp 00h (target: 40d403)
[1151]	40d403	2	jge 00h (target: 40d405)
[1152]	40d405	5	push 90909090h
[1153]	40d40a	1	pop eax
[1154]	40d40b	1	pop edi
[1155]	40d40c	1	pop esi
[1156]	40d40d	1	pop edx
[1157]	40d40e	1	pop ecx
[1158]	40d40f	1	pop ebx
[1159]	40d410	1	pop eax
[1160]	40d411	2	jmp eax											; jump to the good boy (0x40d44e)
; -----------------------------------------------------------------------------------------------------------
[1161]	40d413	2	jmp -56h (target: 40d3bd)
[1162]	40d415	1	inc eax
[1163]	40d416	2	jnb 00h (target: 40d418)
[1164]	40d418	1	dec edi
[1165]	40d419	2	jns 00h (target: 40d41b)
[1166]	40d41b	2	mov esi, eax
[1167]	40d41d	2	jns 00h (target: 40d41f)
[1168]	40d41f	2	xor     esi, edi
[1169]	40d421	2	jz 00h (target: 40d423)
[1170]	40d423	2	mov byte al, 01h
[1171]	40d425	3	sub     edi, 34h
[1172]	40d428	2	jbe 00h (target: 40d42a)
[1173]	40d42a	1	push esp
[1174]	40d42b	1	pop eax
[1175]	40d42c	1	pop edi
[1176]	40d42d	2	jo 00h (target: 40d42f)
[1177]	40d42f	2	mov edi, 0
[1178]	40d431	2	jg 00h (target: 40d433)
[1179]	40d433	2	xor eax, edx
[1180]	40d435	2	jnb 00h (target: 40d437)
[1181]	40d437	2	add     eax, esi
[1182]	40d439	5	mov edi, 00000093h
[1183]	40d43e	1	dec eax
[1184]	40d43f	2	jz 00h (target: 40d441)
[1185]	40d441	1	push ebx
[1186]	40d442	1	pop ecx
[1187]	40d443	2	add     edi, ecx
[1188]	40d445	2	xor eax, ebx
[1189]	40d447	2	xor     ecx, esi
[1190]	40d449	5	jmp FFFFFF6Fh (target: 40d3bd)					; jump to the bad boy
; -----------------------------------------------------------------------------------------------------------
[1191]	40d44e	2	push 00h										; arg4: MB_OK
[1192]	40d450	5	mov eax, 004063EAh
[1193]	40d455	1	push eax										; arg3: AthCon 2013 - RE Challenge
[1194]	40d456	5	mov eax, 00406405h
[1195]	40d45b	1	push eax										; arg2: The only place success comes before work is in the dictionary (Vince Lombardi). Congratulations, you have done it!!!
[1196]	40d45c	2	push 00h										; arg1: HWND
[1197]	40d45e	5	mov esi, 004063DAh								; The goodboy message
[1198]	40d463	5	mov ecx, 000000B0h								
[1199]	40d468	2		mov     eax, [esi]
[1200]	40d46a	5		xor     eax, 7573894Ch
[1201]	40d46f	2		mov     [esi], eax
[1202]	40d471	3		add     esi, 4
[1203]	40d474	3		sub     ecx, 4
[1204]	40d477	2	jnz -0Fh (target: 40d468)
[1205]	40d479	5	call VM (40303Eh)	 (jmp     ds:MessageBoxA)
[1206]	40d47e	2	push 00h
[1207]	40d480	5	call VM (40304Ah)	 (jmp     ds:ExitProcess)
[1208]	40d485	5	jmp FFFFFF33h (target: 40d3bd)
[1209]	40d48a	2	mov ecx, edx
[1210]	40d48c	2	jns 00h (target: 40d48e)
[1211]	40d48e	2	jnb 00h (target: 40d490)
[1212]	40d490	1	inc edi
[1213]	40d491	1	push eax
[1214]	40d492	2	jz 00h (target: 40d494)
[1215]	40d494	1	pop edi
[1216]	40d495	5	mov edi, 00000003h
[1217]	40d49a	2	xor eax, ecx
[1218]	40d49c	2	jo 00h (target: 40d49e)
[1219]	40d49e	1	pop eax
[1220]	40d49f	1	pop ebx
[1221]	40d4a0	2	jns 00h (target: 40d4a2)
[1222]	40d4a2	1	pop ecx
[1223]	40d4a3	2	jbe 00h (target: 40d4a5)
[1224]	40d4a5	2	xor eax, ebx
[1225]	40d4a7	2	jg 00h (target: 40d4a9)
[1226]	40d4a9	2	xor eax, edx
[1227]	40d4ab	2	jnb 00h (target: 40d4ad)
[1228]	40d4ad	3	sub     edi, 3
[1229]	40d4b0	1	pop ebp
[1230]	40d4b1	5	jmp FFFFFF07h (target: 40d3bd)
[1231]	40d4b6	1	retn
