// ------------------------------------------------------------------------------------------------
/*
**	Athcon 2013 reverse engineering challenge
**
** 	debug.c: Execute the emulated instructions.
*/
// ------------------------------------------------------------------------------------------------
#include <idc.idc>

extern vm_offst;

// ------------------------------------------------------------------------------------------------
static disasm( addr, opcd )		/* check where program stops and return the instruction */
{
	if(addr == 0x4077DF) return "jmp eax";
	if(addr == 0x40A80F) return sprintf("shr eax, 0x%02x", Byte(opcd+2));
	if(addr == 0x40C613) return "sysenter";
	if(addr == 0x40C692) return sprintf("jo 0x%08x",      Dword(opcd+2));
	if(addr == 0x40C6B9) return sprintf("jno 0x%08x",     Dword(opcd+2));
	if(addr == 0x40C6E0) return sprintf("jb 0x%08x",      Dword(opcd+2));
	if(addr == 0x40C705) return sprintf("jnb 0x%08x",     Dword(opcd+2));
	if(addr == 0x40C72A) return sprintf("jz 0x%08x",      Dword(opcd+2));
	if(addr == 0x40C74F) return sprintf("jnz 0x%08x",     Dword(opcd+2));
	if(addr == 0x40C774) return sprintf("jbe 0x%08x",     Dword(opcd+2));
	if(addr == 0x40C799) return sprintf("ja 0x%08x",      Dword(opcd+2));
	if(addr == 0x40C7BE) return sprintf("js 0x%08x",      Dword(opcd+2));
	if(addr == 0x40C7E5) return sprintf("jns 0x%08x",     Dword(opcd+2));
	if(addr == 0x40C80C) return sprintf("jp 0x%08x",      Dword(opcd+2));
	if(addr == 0x40C831) return sprintf("jnp 0x%08x",     Dword(opcd+2));
	if(addr == 0x40C856) return sprintf("jge 0x%08x",     Dword(opcd+2));
	if(addr == 0x40C888) return sprintf("jl 0x%08x",      Dword(opcd+2));
	if(addr == 0x40C8BA) return sprintf("jle 0x%08x",     Dword(opcd+2));
	if(addr == 0x40C8F4) return sprintf("jg 0x%08x",      Dword(opcd+2));
	if(addr == 0x40A834) return sprintf("mov al, 0x%02x",  Byte(opcd+1));
	if(addr == 0x40A87A) return sprintf("mov cl, 0x%02x",  Byte(opcd+1));
	if(addr == 0x40A89D) return sprintf("mov dl, 0x%02x",  Byte(opcd+1));
	if(addr == 0x40A857) return sprintf("mov bl, 0x%02x",  Byte(opcd+1));
	if(addr == 0x40A8C0) return sprintf("mov eax, 0x%08x",Dword(opcd+1));
	if(addr == 0x40A906) return sprintf("mov ecx, 0x%08x",Dword(opcd+1));
	if(addr == 0x40A929) return sprintf("mov edx, 0x%08x",Dword(opcd+1));
	if(addr == 0x40A8E3) return sprintf("mov ebx, 0x%08x",Dword(opcd+1));
	if(addr == 0x40A94C) return sprintf("mov ebp, 0x%08x",Dword(opcd+1));
	if(addr == 0x40A992) return sprintf("mov esi, 0x%08x",Dword(opcd+1));
	if(addr == 0x40A9B5) return sprintf("mov edi, 0x%08x",Dword(opcd+1));
	if(addr == 0x40C127) return sprintf("jo 0x%02x",       Byte(opcd+1));
	if(addr == 0x40C167) return sprintf("jno 0x%02x",      Byte(opcd+1));
	if(addr == 0x40C1A7) return sprintf("jb 0x%02x",       Byte(opcd+1));
	if(addr == 0x40C1E5) return sprintf("jnb 0x%02x",      Byte(opcd+1));
	if(addr == 0x40C223) return sprintf("jz 0x%02x",       Byte(opcd+1));
	if(addr == 0x40C261) return sprintf("jnz 0x%02x",      Byte(opcd+1));
	if(addr == 0x40C29F) return sprintf("jbe 0x%02x",      Byte(opcd+1));
	if(addr == 0x40C2DD) return sprintf("ja 0x%02x",       Byte(opcd+1));
	if(addr == 0x40C31B) return sprintf("js 0x%02x",       Byte(opcd+1));
	if(addr == 0x40C35B) return sprintf("jns 0x%02x",      Byte(opcd+1));
	if(addr == 0x40C39B) return sprintf("jp 0x%02x",       Byte(opcd+1));
	if(addr == 0x40C3D9) return sprintf("jnp 0x%02x",      Byte(opcd+1));
	if(addr == 0x40C462) return sprintf("jge 0x%02x",      Byte(opcd+1));
	if(addr == 0x40C417) return sprintf("jl 0x%02x",       Byte(opcd+1));
	if(addr == 0x40C4AD) return sprintf("jle 0x%02x",      Byte(opcd+1));
	if(addr == 0x40C500) return sprintf("jg 0x%02x",       Byte(opcd+1));
	if(addr == 0x40CA02) return sprintf("loopne 0x%02x",   Byte(opcd+1));
	if(addr == 0x40CA4E) return sprintf("loope 0x%02x",    Byte(opcd+1));
	if(addr == 0x40CA9A) return sprintf("loop 0x%02x",     Byte(opcd+1));
	if(addr == 0x40C969) return sprintf("call 0x%08x",     Dword(opcd+1));
	if(addr == 0x40C958) return sprintf("jmp 0x%08x", 	   Dword(opcd+1));
	if(addr == 0x40C92E) return sprintf("jmp 0x%02x",      Byte(opcd+1));
	if(addr == 0x40BF78) return "push eax";
	if(addr == 0x40BF93) return "push ecx";
	if(addr == 0x40BFAE) return "push edx";
	if(addr == 0x40BFC9) return "push ebx";
	if(addr == 0x40BFE4) return "push esp";
	if(addr == 0x40BFFF) return "push ebp";
	if(addr == 0x40C01A) return "push esi";
	if(addr == 0x40C035) return "push edi";
	if(addr == 0x40C050) return "pop eax";
	if(addr == 0x40C06C) return "pop ecx";
	if(addr == 0x40C088) return "pop edx";
	if(addr == 0x40C0A4) return "pop ebx";
	if(addr == 0x40C0C0) return "pop esp";
	if(addr == 0x40C0D3) return "pop ebp";
	if(addr == 0x40C0EF) return "pop esi";
	if(addr == 0x40C10B) return "pop edi";
	if(addr == 0x40BDA8) return "inc eax";
	if(addr == 0x40BDC5) return "inc ecx";
	if(addr == 0x40BDE2) return "inc edx";
	if(addr == 0x40BDFF) return "inc ebx";
	if(addr == 0x40BE1C) return "inc esp";
	if(addr == 0x40BE39) return "inc ebp";
	if(addr == 0x40BE56) return "inc esi";
	if(addr == 0x40BE73) return "inc edi";
	if(addr == 0x40BE90) return "dec eax";
	if(addr == 0x40BEAD) return "dec ecx";
	if(addr == 0x40BECA) return "de edx";
	if(addr == 0x40BEE7) return "dec ebx";
	if(addr == 0x40BF04) return "dec esp";
	if(addr == 0x40BF21) return "dec ebp";
	if(addr == 0x40BF3E) return "dec esi";
	if(addr == 0x40BF5B) return "dec edi";
	if(addr == 0x40BD5E) return sprintf("push 0x%08x",     Dword(opcd+1));
	if(addr == 0x40BD7B) return sprintf("push 0x%02x",      Byte(opcd+1));
	if(addr == 0x40A071) return GetDisasm(addr+vm_offst+0x03);
	if(addr == 0x40BBBD) return "mov byte al, 0";
	if(addr == 0x40BBD8) return "mov byte bl, 0";
	if(addr == 0x40BBF3) return "mov byte cl, 0";
	if(addr == 0x40BC0E) return "mov byte dl, 0";
	if(addr == 0x40BC29) return "mov eax, 0";
	if(addr == 0x40BC47) return "mov ebx, 0";
	if(addr == 0x40BC65) return "mov ecx, 0";
	if(addr == 0x40BC83) return "mov edx, 0";
	if(addr == 0x40BCA1) return "mov ebp, 0";
	if(addr == 0x40BCBF) return "mov esi, 0";
	if(addr == 0x40BCDD) return "mov edi, 0";
	if(addr == 0x40BCFB) return "xor eax, ebx";
	if(addr == 0x40BD1C) return "xor eax, ecx";
	if(addr == 0x40BD3D) return "xor eax, edx";
	if(addr == 0x40BB51) return "test byte al, al";
	if(addr == 0x40BB6C) return "test byte bl, bl";
	if(addr == 0x40BB87) return "test byte cl, cl";
	if(addr == 0x40BBA2) return "test byte dl, dl";
	if(addr == 0x40BA94) return "test eax, eax";
	if(addr == 0x40BAAF) return "test ebx, ebx";
	if(addr == 0x40BACA) return "test ecx, ecx";
	if(addr == 0x40BAE5) return "test edx, edx";
	if(addr == 0x40BB00) return "test ebp, ebp";
	if(addr == 0x40BB1B) return "test esi, esi";
	if(addr == 0x40BB36) return "test edi, edi";
	if(addr == 0x40B35C) return "mov eax, ebx";
	if(addr == 0x40B37D) return "mov eax, ecx";
	if(addr == 0x40B39E) return "mov eax, edx";
	if(addr == 0x40B3BF) return "mov eax, ebp";
	if(addr == 0x40B3E0) return "mov eax, esp";
	if(addr == 0x40B401) return "mov eax, esi";
	if(addr == 0x40B422) return "mov eax, edi";
	if(addr == 0x40B443) return "mov ebx, eax";
	if(addr == 0x40B464) return "mov ebx, ecx";
	if(addr == 0x40B485) return "mov ebx, edx";
	if(addr == 0x40B4A6) return "mov ebx, ebp";
	if(addr == 0x40B4C7) return "mov ebx, esp";
	if(addr == 0x40B4E8) return "mov ebx, esi";
	if(addr == 0x40B509) return "mov ebx, edi";
	if(addr == 0x40B52A) return "mov ecx, eax";
	if(addr == 0x40B54B) return "mov ecx, ebx";
	if(addr == 0x40B56C) return "mov ecx, edx";
	if(addr == 0x40B58D) return "mov ecx, ebp";
	if(addr == 0x40B5AE) return "mov ecx, esp";
	if(addr == 0x40B5CF) return "mov ecx, esi";
	if(addr == 0x40B5F0) return "mov ecx, edi";
	if(addr == 0x40B611) return "mov edx, eax";
	if(addr == 0x40B632) return "mov edx, ebx";
	if(addr == 0x40B653) return "mov edx, ecx";
	if(addr == 0x40B674) return "mov edx, ebp";
	if(addr == 0x40B695) return "mov edx, esp";
	if(addr == 0x40B6B6) return "mov edx, esi";
	if(addr == 0x40B6D7) return "mov edx, edi";
	if(addr == 0x40B6F8) return "mov ebp, eax";
	if(addr == 0x40B719) return "mov ebp, ebx";
	if(addr == 0x40B73A) return "mov ebp, ecx";
	if(addr == 0x40B75B) return "mov ebp, edx";
	if(addr == 0x40B77C) return "mov ebp, esp";
	if(addr == 0x40B79D) return "mov ebp, esi";
	if(addr == 0x40B7BE) return "mov ebp, edi";
	if(addr == 0x40B7DF) return "mov esp, eax";
	if(addr == 0x40B800) return "mov esp, ebx";
	if(addr == 0x40B821) return "mov esp, ecx";
	if(addr == 0x40B842) return "mov esp, edx";
	if(addr == 0x40B863) return "mov esp, ebp";
	if(addr == 0x40B884) return "mov esp, esi";
	if(addr == 0x40B8A5) return "mov esp, edi";
	if(addr == 0x40B8C6) return "mov esi, eax";
	if(addr == 0x40B8E7) return "mov esi, ebx";
	if(addr == 0x40B908) return "mov esi, ecx";
	if(addr == 0x40B929) return "mov esi, edx";
	if(addr == 0x40B94A) return "mov esi, ebp";
	if(addr == 0x40B96B) return "mov esi, esp";
	if(addr == 0x40B98C) return "mov esi, edi";
	if(addr == 0x40B9AD) return "mov edi, eax";
	if(addr == 0x40B9CE) return "mov edi, ebx";
	if(addr == 0x40B9EF) return "mov edi, ecx";
	if(addr == 0x40BA10) return "mov edi, edx";
	if(addr == 0x40BA31) return "mov edi, ebp";
	if(addr == 0x40BA52) return "mov edi, esp";
	if(addr == 0x40BA73) return "mov edi, esi";
	if(addr == 0x40AB40) return "mov byte al, bl";
	if(addr == 0x40AB40) return "mov byte al, cl";
	if(addr == 0x40AB82) return "mov byte al, dl";
	if(addr == 0x40ABA3) return "mov byte bl, al";
	if(addr == 0x40ABC4) return "mov byte bl, cl";
	if(addr == 0x40ABE5) return "mov byte bl, dl";
	if(addr == 0x40AC06) return "mov byte cl, al";
	if(addr == 0x40AC27) return "mov byte cl, bl";
	if(addr == 0x40AC48) return "mov byte cl, dl";
	if(addr == 0x40AC69) return "mov byte dl, al";
	if(addr == 0x40AC8A) return "mov byte dl, bl";
	if(addr == 0x40ACAB) return "mov byte dl, cl";
	if(addr == 0x40ACCC) return "cmp eax, ebx";
	if(addr == 0x40ACEA) return "cmp eax, ecx";
	if(addr == 0x40AD08) return "cmp eax, edx";
	if(addr == 0x40AD26) return "cmp eax, ebp";
	if(addr == 0x40AD44) return "cmp eax, esp";
	if(addr == 0x40AD62) return "cmp eax, esi";
	if(addr == 0x40AD80) return "cmp eax, edi";
	if(addr == 0x40AD9E) return "cmp ebx, eax";
	if(addr == 0x40ADBC) return "cmp ebx, ecx";
	if(addr == 0x40ADDA) return "cmp ebx, edx";
	if(addr == 0x40ADF8) return "cmp ebx, ebp";
	if(addr == 0x40AE16) return "cmp ebx, esp";
	if(addr == 0x40AE34) return "cmp ebx, esi";
	if(addr == 0x40AE52) return "cmp ebx, edi";
	if(addr == 0x40AE70) return "cmp ecx, eax";
	if(addr == 0x40AE8E) return "cmp ecx, ebx";
	if(addr == 0x40AEAC) return "cmp ecx, edx";
	if(addr == 0x40AECA) return "cmp ecx, ebp";
	if(addr == 0x40AEE8) return "cmp ecx, esp";
	if(addr == 0x40AF06) return "cmp ecx, esi";
	if(addr == 0x40AF24) return "cmp ecx, edi";
	if(addr == 0x40AF42) return "cmp edx, eax";
	if(addr == 0x40AF60) return "cmp edx, ebx";
	if(addr == 0x40AF7E) return "cmp edx, ecx";
	if(addr == 0x40AF9C) return "cmp edx, ebp";
	if(addr == 0x40AFBA) return "cmp edx, esp";
	if(addr == 0x40AFD8) return "cmp edx, esi";
	if(addr == 0x40AFF6) return "cmp edx, edi";
	if(addr == 0x40B014) return "cmp ebp, eax";
	if(addr == 0x40B032) return "cmp ebp, ebx";
	if(addr == 0x40B050) return "cmp ebp, ecx";
	if(addr == 0x40B06E) return "cmp ebp, edx";
	if(addr == 0x40B08C) return "cmp ebp, esp";
	if(addr == 0x40B0AA) return "cmp ebp, esi";
	if(addr == 0x40B0C8) return "cmp ebp, edi";
	if(addr == 0x40B0E6) return "cmp esp, eax";
	if(addr == 0x40B104) return "cmp esp, ebx";
	if(addr == 0x40B122) return "cmp esp, ecx";
	if(addr == 0x40B140) return "cmp esp, edx";
	if(addr == 0x40B15E) return "cmp esp, ebp";
	if(addr == 0x40B17C) return "cmp esp, esi";
	if(addr == 0x40B19A) return "cmp esp, edi";
	if(addr == 0x40B1B8) return "cmp esi, eax";
	if(addr == 0x40B1D6) return "cmp esi, ebx";
	if(addr == 0x40B1F4) return "cmp esi, ecx";
	if(addr == 0x40B212) return "cmp esi, edx";
	if(addr == 0x40B230) return "cmp esi, ebp";
	if(addr == 0x40B24E) return "cmp esi, esp";
	if(addr == 0x40B26C) return "cmp esi, edi";
	if(addr == 0x40B28A) return "cmp edi, eax";
	if(addr == 0x40B2A8) return "cmp edi, ebx";
	if(addr == 0x40B2C6) return "cmp edi, ecx";
	if(addr == 0x40B2E4) return "cmp edi, edx";
	if(addr == 0x40B302) return "cmp edi, ebp";
	if(addr == 0x40B320) return "cmp edi, esp";
	if(addr == 0x40B33E) return "cmp edi, esi";
	if(addr == 0x40A9D8) return "cmp byte al, bl";
	if(addr == 0x40A9F6) return "cmp byte al, cl";
	if(addr == 0x40AA14) return "cmp byte al, dl";
	if(addr == 0x40AA32) return "cmp byte bl, al";
	if(addr == 0x40AA50) return "cmp byte bl, cl";
	if(addr == 0x40AA6E) return "cmp byte bl, dl";
	if(addr == 0x40AA8C) return "cmp byte cl, al";
	if(addr == 0x40AAAA) return "cmp byte cl, bl";
	if(addr == 0x40AAC8) return "cmp byte cl, dl";
	if(addr == 0x40AAE6) return "cmp byte dl, al";
	if(addr == 0x40AB04) return "cmp byte dl, bl";
	if(addr == 0x40AB22) return "cmp byte dl, cl";

	return GetDisasm(addr+vm_offst+0x03); 
}
// ------------------------------------------------------------------------------------------------
static main()
{
	auto instraddr, n_instr, opcds, mnem, clock, n;
	
	
	n_instr = 80;										// how many instruction to execute
	clock   = 0;										// if you run the script many times during
														// a single execution, change clock ticks to
														// avoid negative time intervals
	
	/* when we start the script, program must be at line 0x4077DF */
	Message( "Debug Emulator started...\n" );

	// this doesn't work:
	// RunTo( 0x004077DF );								// run the beginning of the decoder
	// GetDebuggerEvent(WFNE_SUSP, -1);					// allow execution to continue	
	
	for( n=0; n<n_instr; n++ )							// execute n_instr instructions
	{				
		RunTo( 0x0040A0CD + vm_offst );					// run the end of the command
		GetDebuggerEvent(WFNE_SUSP, -1);				// allow execution to continue	

		/* there must be a BP on every instruction. */
		
		instraddr = GetEventEa();						// get instruction address				
		opcds 	  = Dword(GetRegValue("edi"));			// get opcodes
		
		PatchDword(EDI-0x4, Dword(EDI-0x4)&0xFFFFFEFF);	// clear TF (signle step execution) 
		
			 if( instraddr > 0x427730 ) vm_offst = 0x20000;
		else if( instraddr > 0x41F730 ) vm_offst = 0x18000;
		else if( instraddr > 0x417730 ) vm_offst = 0x10000;
		else if( instraddr > 0x40F730 ) vm_offst = 0x8000;
		else vm_offst = 0x0;
		
		mnem = disasm(instraddr-vm_offst, opcds);		// disassembly
		Message( "%3d:\t%x\t%x\t%s\n", n, instraddr, opcds, mnem );
		
		/*
		** Below are some tricks to bypass the anti-debugging tricks :)
		** You can comment out and use any of them...
		*/
		
		/* Skip calls (do not change emulator) */
		if( instraddr == 0x40c969 ) {					// reach call virtual instruction?
			EIP = 0x40C9FD;								// skip the code	
			PatchDword(EDI, Dword(EDI)+5);				// update virtual eip
		}
			
		/* Make time moving slower :P */
		if( strstr(mnem, "int     2Ah") != -1 )			// call to KiGetTickCount() ?
		{
			StepOver();									// go the interrupt
			GetDebuggerEvent(WFNE_SUSP, -1);

			StepOver();									// execute it
			GetDebuggerEvent(WFNE_SUSP, -1);
			
			/* now, eax contains the clock ticks */
			
			EAX   = 0x300 + clock;						// set clock ticks to our own value
			clock = clock + 1;							// clock increases very slowly
			
			Message( "\t\teax = 0x%08x\n", EAX );		// print value
		}	
		
		
		/* escape from loop instructions */
		if( strstr(mnem, "loop") != -1 ) {
			PatchDword(EDI-0x1C, 1);					// just set ecx to 1
		}
		
		/*
		** tricks end here.
		*/
			
		RunTo( 0x004077DF + vm_offst );			// run to the beginning of the next instruction
		GetDebuggerEvent(WFNE_SUSP, -1);		// allow execution to continue	
	}
}
// ------------------------------------------------------------------------------------------------
