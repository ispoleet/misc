// ------------------------------------------------------------------------------------------------
/*
**	Athcon 2013 reverse engineering challenge
**
** 	disasm.c: Disassembly the emulated instructions.
**
**
**	IMPORTANT NOTE: This script must be run while the program in running!
*/
// ------------------------------------------------------------------------------------------------
#include <idc.idc>


extern real_opcd, decd, is_real, vm_offst;		// global variables

// ------------------------------------------------------------------------------------------------
static jmpoff(offst, addr)						/* convert 1 byte offset to a signed offset */
{
	if( offst > 0x80 ) 
		return sprintf( "-%02Xh (target: %x)", (0xfe - offst), addr-(0xfe - offst));

	return sprintf( "%02Xh (target: %x)", offst, addr+offst+2);
}
// ------------------------------------------------------------------------------------------------
static real_disasm(opcd1, opcd2, addr, size)	/* disassembly a real x86 */
{
	auto instr, j;
	
	for( j=0; j<15; j++ )							// copy 15 bytes
		PatchByte(decd+j, Byte(addr+j));

	for( j=0; j<256; j++ ) {						// execute instruction in real environment
		if( Byte(real_opcd + j) == (opcd1^0x8a) )
			break;
	}
	
	MakeUnknown(decd, 15, DOUNK_SIMPLE);			// undefine all bytes
	PatchByte(decd, j);						   	 	// patch byte
		
	size = MakeCode( decd );						// make them code
	instr = GetDisasm( decd );						// disassemble!
	
	return instr;									// return instruction
}
// ------------------------------------------------------------------------------------------------
static my_disasm(opcd1, opcd2, addr, size)		/* do the disassembly */
{
	auto instr;
	
	
		 if( opcd1 == 0x1F && opcd2 == 0xE0 ) { size = 2; return "jmp eax"; }
	else if( opcd1 == 0x6D && opcd2 == 0x18 ) { size = 3; return sprintf("shr eax, %02Xh", Byte(addr+2)); }
	else if( opcd1 == 0x42 )
	{
			 if(opcd2 == 0x04) { size = 2; return "sysenter"; }
        else if(opcd2 == 0x24) instr = "jo" ;
        else if(opcd2 == 0x25) instr = "jno";
        else if(opcd2 == 0x26) instr = "jb" ;
        else if(opcd2 == 0x27) instr = "jnb";
        else if(opcd2 == 0x36) instr = "jz" ;
        else if(opcd2 == 0x39) instr = "jnz";
        else if(opcd2 == 0x3B) instr = "jbe";
        else if(opcd2 == 0x3C) instr = "ja" ;
        else if(opcd2 == 0x3D) instr = "js" ;
        else if(opcd2 == 0x3E) instr = "jns";
        else if(opcd2 == 0x3F) instr = "jp" ;
        else if(opcd2 == 0x7A) instr = "jnp";
        else if(opcd2 == 0x7B) instr = "jge";
        else if(opcd2 == 0xA6) instr = "jl" ;
        else if(opcd2 == 0xA7) instr = "jle";
        else if(opcd2 == 0xFF) instr = "jg" ;
        else return real_disasm( opcd1, opcd2, addr, &size );

		size = 6;
        return sprintf( "%s %08Xh (target: %x)", instr, Dword(addr+2), addr+Dword(addr+2)+6);
    }
    else if(opcd1 == 0xA5) {size = 2; return sprintf("mov byte al, %02Xh", opcd2); }
    else if(opcd1 == 0xB5) {size = 2; return sprintf("mov byte cl, %02Xh", opcd2); }
    else if(opcd1 == 0x95) {size = 2; return sprintf("mov byte dl, %02Xh", opcd2); }
    else if(opcd1 == 0x85) {size = 2; return sprintf("mov byte bl, %02Xh", opcd2); }
    
    else if(opcd1 == 0xE5) {size = 5; return sprintf("mov eax, %08Xh", Dword(addr+1) ); }
    else if(opcd1 == 0xF5) {size = 5; return sprintf("mov ecx, %08Xh", Dword(addr+1) ); }
    else if(opcd1 == 0xD5) {size = 5; return sprintf("mov edx, %08Xh", Dword(addr+1) ); }
    else if(opcd1 == 0x75) {size = 5; return sprintf("mov ebx, %08Xh", Dword(addr+1) ); }
    else if(opcd1 == 0x65) {size = 5; return sprintf("mov esp, %08Xh", Dword(addr+1) ); }
    else if(opcd1 == 0x35) {size = 5; return sprintf("mov ebp, %08Xh", Dword(addr+1) ); }
    else if(opcd1 == 0x55) {size = 5; return sprintf("mov esi, %08Xh", Dword(addr+1) ); }
    else if(opcd1 == 0x45) {size = 5; return sprintf("mov edi, %08Xh", Dword(addr+1) ); }
		
    else if(opcd1 == 0xAB) {size = 2; return sprintf("jo %s",  jmpoff(opcd2,addr) ); }
    else if(opcd1 == 0xBB) {size = 2; return sprintf("jno %s", jmpoff(opcd2,addr) ); }
    else if(opcd1 == 0x9B) {size = 2; return sprintf("jb %s",  jmpoff(opcd2,addr) ); }
    else if(opcd1 == 0x8B) {size = 2; return sprintf("jnb %s", jmpoff(opcd2,addr) ); }
    else if(opcd1 == 0xFB) {size = 2; return sprintf("jz %s",  jmpoff(opcd2,addr) ); }
    else if(opcd1 == 0xEB) {size = 2; return sprintf("jnz %s", jmpoff(opcd2,addr) ); }
    else if(opcd1 == 0x7B) {size = 2; return sprintf("jbe %s", jmpoff(opcd2,addr) ); }
    else if(opcd1 == 0xDB) {size = 2; return sprintf("ja %s",  jmpoff(opcd2,addr) ); }
    else if(opcd1 == 0x6B) {size = 2; return sprintf("js %s",  jmpoff(opcd2,addr) ); }
    else if(opcd1 == 0xCB) {size = 2; return sprintf("jns %s", jmpoff(opcd2,addr) ); }
    else if(opcd1 == 0x5B) {size = 2; return sprintf("jp %s",  jmpoff(opcd2,addr) ); }
    else if(opcd1 == 0x4B) {size = 2; return sprintf("jnp %s", jmpoff(opcd2,addr) ); }
    else if(opcd1 == 0x0B) {size = 2; return sprintf("jl %s",  jmpoff(opcd2,addr) ); }
    else if(opcd1 == 0x1B) {size = 2; return sprintf("jge %s", jmpoff(opcd2,addr) ); }
    else if(opcd1 == 0x2B) {size = 2; return sprintf("jle %s", jmpoff(opcd2,addr) ); }
    else if(opcd1 == 0x3B) {size = 2; return sprintf("jg %s",  jmpoff(opcd2,addr) ); }
         
    else if(opcd1 == 0x7C) {size = 2; return sprintf("loopne %s", jmpoff(opcd2,addr) ); }
    else if(opcd1 == 0x6C) {size = 2; return sprintf("loope %s",  jmpoff(opcd2,addr) ); }
    else if(opcd1 == 0x1C) {size = 2; return sprintf("loop %s",   jmpoff(opcd2,addr) ); }
    else if(opcd1 == 0xBC) {
		auto vm = addr + Dword(addr+1) + 5 - Dword(0x407604+vm_offst); 

		size = 5; 

		return sprintf("call VM (%Xh)\t (%s)", vm, GetDisasm(vm) ); 
	}
    else if(opcd1 == 0xAC) {
		size = 5; 
		return sprintf("jmp %08Xh (target: %x)", Dword(addr+1), addr+Dword(addr+1)+5); 
	}
    else if(opcd1 == 0x5C) {size = 2; return sprintf("jmp %s", jmpoff(opcd2,addr) ); }
    
    else if(opcd1 == 0x28) {size = 1; return "push eax";}
    else if(opcd1 == 0x38) {size = 1; return "push ecx";}
    else if(opcd1 == 0x48) {size = 1; return "push edx";}
    else if(opcd1 == 0xC8) {size = 1; return "push ebx";}
    else if(opcd1 == 0x18) {size = 1; return "push esp";}
    else if(opcd1 == 0x58) {size = 1; return "push ebp";}
    else if(opcd1 == 0x98) {size = 1; return "push esi";}
    else if(opcd1 == 0x68) {size = 1; return "push edi";}
    
    else if(opcd1 == 0xA8) {size = 1; return "pop eax";}
    else if(opcd1 == 0xB8) {size = 1; return "pop ecx";}
    else if(opcd1 == 0x78) {size = 1; return "pop edx";}
    else if(opcd1 == 0x88) {size = 1; return "pop ebx";}
    else if(opcd1 == 0x08) {size = 1; return "pop esp";}
    else if(opcd1 == 0xE8) {size = 1; return "pop ebp";}
    else if(opcd1 == 0xF8) {size = 1; return "pop esi";}
    else if(opcd1 == 0xD8) {size = 1; return "pop edi";}
    
    else if(opcd1 == 0x60) {size = 1; return "inc eax";}
    else if(opcd1 == 0x70) {size = 1; return "inc ecx";}
    else if(opcd1 == 0x50) {size = 1; return "inc edx";}
    else if(opcd1 == 0x20) {size = 1; return "inc ebx";}
    else if(opcd1 == 0x10) {size = 1; return "inc esp";}
    else if(opcd1 == 0x30) {size = 1; return "inc ebp";}
    else if(opcd1 == 0x90) {size = 1; return "inc esi";}
    else if(opcd1 == 0x80) {size = 1; return "inc edi";}
       
    else if(opcd1 == 0x40) {size = 1; return "dec eax";}
    else if(opcd1 == 0x00) {size = 1; return "dec ecx";}
    else if(opcd1 == 0xA0) {size = 1; return "dec edx";}
    else if(opcd1 == 0xE0) {size = 1; return "dec ebx";}
    else if(opcd1 == 0xF0) {size = 1; return "dec esp";}
    else if(opcd1 == 0xB0) {size = 1; return "dec ebp";}
    else if(opcd1 == 0xD0) {size = 1; return "dec esi";}
    else if(opcd1 == 0xC0) {size = 1; return "dec edi";}
       
    else if(opcd1 == 0xD4) {size = 5; return sprintf("push %08Xh", Dword(addr+1));}
    else if(opcd1 == 0xC4) {size = 2; return sprintf("push %02Xh", opcd2);}
     
    else if(Byte(addr + 0x1200) != 0x01) 
	{
		return real_disasm( opcd1, opcd2, addr, &size );	
	}
	
    else if(opcd1 == 0x8A) 
	{
             if(opcd2 == 0x13) instr ="mov byte al, 0";
        else if(opcd2 == 0x14) instr ="mov byte bl, 0";
        else if(opcd2 == 0x15) instr ="mov byte cl, 0";
        else if(opcd2 == 0x16) instr ="mov byte dl, 0";
        else return real_disasm( opcd1, opcd2, addr, &size );

        size = 2;
		return instr;
    }
    else if(opcd1 == 0xCA) 
	{
			 if(opcd2 == 0x01) instr = "mov eax, 0";  
        else if(opcd2 == 0x02) instr = "mov ebx, 0";  
        else if(opcd2 == 0x03) instr = "mov ecx, 0";  
        else if(opcd2 == 0x04) instr = "mov edx, 0";  
        else if(opcd2 == 0x05) instr = "mov ebp, 0";  
        else if(opcd2 == 0x06) instr = "mov esi, 0";  
        else if(opcd2 == 0x07) instr = "mov edi, 0";  
        else if(opcd2 == 0x08) instr = "xor eax, ebx";
        else if(opcd2 == 0x09) instr = "xor eax, ecx";
        else if(opcd2 == 0x0A) instr = "xor eax, edx";
        else return real_disasm( opcd1, opcd2, addr, &size );
        
        size = 2;
		return instr;
	}
    else if(opcd1 == 0x97)
	{
			 if(opcd2 == 0x55) instr = "test byte al, al";
        else if(opcd2 == 0x56) instr = "test byte bl, bl";
        else if(opcd2 == 0x57) instr = "test byte cl, cl";
        else if(opcd2 == 0x58) instr = "test byte dl, dl";
        else return real_disasm( opcd1, opcd2, addr, &size );
       
		size = 2;
        return instr;
	}
    else if(opcd1 == 0x37) 
	{
			 if(opcd2 == 0x72) instr = "test eax, eax";
        else if(opcd2 == 0x73) instr = "test ebx, ebx";
        else if(opcd2 == 0x74) instr = "test ecx, ecx";
        else if(opcd2 == 0x75) instr = "test edx, edx";
        else if(opcd2 == 0x76) instr = "test ebp, ebp";
        else if(opcd2 == 0x77) instr = "test esi, esi";
        else if(opcd2 == 0x78) instr = "test edi, edi";
        else return real_disasm( opcd1, opcd2, addr, &size );
       
        size = 2;
		return instr;
	}	
		
    else if(opcd1 == 0xB7) 
	{
			 if(opcd2 == 0x11) instr = "mov eax, ebx";
        else if(opcd2 == 0x12) instr = "mov eax, ecx";
        else if(opcd2 == 0x13) instr = "mov eax, edx";
        else if(opcd2 == 0x14) instr = "mov eax, ebp";
        else if(opcd2 == 0x15) instr = "mov eax, esp";
        else if(opcd2 == 0x16) instr = "mov eax, esi";
        else if(opcd2 == 0x17) instr = "mov eax, edi";
        else if(opcd2 == 0x23) instr = "mov ebx, eax";
        else if(opcd2 == 0x25) instr = "mov ebx, ecx";
        else if(opcd2 == 0x27) instr = "mov ebx, edx";
        else if(opcd2 == 0x29) instr = "mov ebx, ebp";
        else if(opcd2 == 0x2B) instr = "mov ebx, esp";
        else if(opcd2 == 0x2D) instr = "mov ebx, esi";
        else if(opcd2 == 0x2F) instr = "mov ebx, edi";
        else if(opcd2 == 0x31) instr = "mov ecx, eax";
        else if(opcd2 == 0x32) instr = "mov ecx, ebx";
        else if(opcd2 == 0x33) instr = "mov ecx, edx";
        else if(opcd2 == 0x34) instr = "mov ecx, ebp";
        else if(opcd2 == 0x35) instr = "mov ecx, esp";
        else if(opcd2 == 0x36) instr = "mov ecx, esi";
        else if(opcd2 == 0x37) instr = "mov ecx, edi";
        else if(opcd2 == 0x43) instr = "mov edx, eax";
        else if(opcd2 == 0x44) instr = "mov edx, ebx";
        else if(opcd2 == 0x45) instr = "mov edx, ecx";
        else if(opcd2 == 0x46) instr = "mov edx, ebp";
        else if(opcd2 == 0x47) instr = "mov edx, esp";
        else if(opcd2 == 0x48) instr = "mov edx, esi";
        else if(opcd2 == 0x49) instr = "mov edx, edi";
        else if(opcd2 == 0x53) instr = "mov ebp, eax";
        else if(opcd2 == 0x54) instr = "mov ebp, ebx";
        else if(opcd2 == 0x55) instr = "mov ebp, ecx";
        else if(opcd2 == 0x56) instr = "mov ebp, edx";
        else if(opcd2 == 0x57) instr = "mov ebp, esp";
        else if(opcd2 == 0x58) instr = "mov ebp, esi";
        else if(opcd2 == 0x59) instr = "mov ebp, edi";
        else if(opcd2 == 0x73) instr = "mov esp, eax";
        else if(opcd2 == 0x74) instr = "mov esp, ebx";
        else if(opcd2 == 0x75) instr = "mov esp, ecx";
        else if(opcd2 == 0x76) instr = "mov esp, edx";
        else if(opcd2 == 0x77) instr = "mov esp, ebp";
        else if(opcd2 == 0x78) instr = "mov esp, esi";
        else if(opcd2 == 0x79) instr = "mov esp, edi";
        else if(opcd2 == 0x80) instr = "mov esi, eax";
        else if(opcd2 == 0x81) instr = "mov esi, ebx";
        else if(opcd2 == 0x82) instr = "mov esi, ecx";
        else if(opcd2 == 0x83) instr = "mov esi, edx";
        else if(opcd2 == 0x84) instr = "mov esi, ebp";
        else if(opcd2 == 0x85) instr = "mov esi, esp";
        else if(opcd2 == 0x86) instr = "mov esi, edi";
        else if(opcd2 == 0x93) instr = "mov edi, eax";
        else if(opcd2 == 0x94) instr = "mov edi, ebx";
        else if(opcd2 == 0x95) instr = "mov edi, ecx";
        else if(opcd2 == 0x96) instr = "mov edi, edx";
        else if(opcd2 == 0x97) instr = "mov edi, ebp";
        else if(opcd2 == 0x98) instr = "mov edi, esp";
        else if(opcd2 == 0x99) instr = "mov edi, esi";
        else return real_disasm( opcd1, opcd2, addr, &size );
        
        size = 2;
		return instr;
	}
   
    else if(opcd1 == 0x07) 
	{
			 if(opcd2 == 0x94) instr = "mov byte al, bl";
        else if(opcd2 == 0x95) instr = "mov byte al, cl";
        else if(opcd2 == 0x96) instr = "mov byte al, dl";
        else if(opcd2 == 0x97) instr = "mov byte bl, al";
        else if(opcd2 == 0x98) instr = "mov byte bl, cl";
        else if(opcd2 == 0x99) instr = "mov byte bl, dl";
        else if(opcd2 == 0x9A) instr = "mov byte cl, al";
        else if(opcd2 == 0x9B) instr = "mov byte cl, bl";
        else if(opcd2 == 0x9C) instr = "mov byte cl, dl";
        else if(opcd2 == 0x9D) instr = "mov byte dl, al";
        else if(opcd2 == 0x9E) instr = "mov byte dl, bl";
        else if(opcd2 == 0x9F) instr = "mov byte dl, cl";
        else return real_disasm( opcd1, opcd2, addr, &size );
      
		size = 2;
		return instr;
	}
    
    else if(opcd1 == 0xDA) 
	{
			 if(opcd2 == 0x70) instr = "cmp eax, ebx";
        else if(opcd2 == 0x71) instr = "cmp eax, ecx";
        else if(opcd2 == 0x72) instr = "cmp eax, edx";
        else if(opcd2 == 0x73) instr = "cmp eax, ebp";
        else if(opcd2 == 0x74) instr = "cmp eax, esp";
        else if(opcd2 == 0x75) instr = "cmp eax, esi";
        else if(opcd2 == 0x76) instr = "cmp eax, edi";
        else if(opcd2 == 0x77) instr = "cmp ebx, eax";
        else if(opcd2 == 0x78) instr = "cmp ebx, ecx";
        else if(opcd2 == 0x79) instr = "cmp ebx, edx";
        else if(opcd2 == 0x80) instr = "cmp ebx, ebp";
        else if(opcd2 == 0x81) instr = "cmp ebx, esp";
        else if(opcd2 == 0x82) instr = "cmp ebx, esi";
        else if(opcd2 == 0x83) instr = "cmp ebx, edi";
        else if(opcd2 == 0x84) instr = "cmp ecx, eax";
        else if(opcd2 == 0x85) instr = "cmp ecx, ebx";
        else if(opcd2 == 0x86) instr = "cmp ecx, edx";
        else if(opcd2 == 0x87) instr = "cmp ecx, ebp";
        else if(opcd2 == 0x88) instr = "cmp ecx, esp";
        else if(opcd2 == 0x89) instr = "cmp ecx, esi";
        else if(opcd2 == 0x90) instr = "cmp ecx, edi";
        else if(opcd2 == 0x91) instr = "cmp edx, eax";
        else if(opcd2 == 0x92) instr = "cmp edx, ebx";
        else if(opcd2 == 0x93) instr = "cmp edx, ecx";
        else if(opcd2 == 0x94) instr = "cmp edx, ebp";
        else if(opcd2 == 0x95) instr = "cmp edx, esp";
        else if(opcd2 == 0x96) instr = "cmp edx, esi";
        else if(opcd2 == 0x97) instr = "cmp edx, edi";
        else if(opcd2 == 0x98) instr = "cmp ebp, eax";
        else if(opcd2 == 0x90) instr = "cmp ebp, ebx";
        else if(opcd2 == 0x20) instr = "cmp ebp, ecx";
        else if(opcd2 == 0x21) instr = "cmp ebp, edx";
        else if(opcd2 == 0x22) instr = "cmp ebp, esp";
        else if(opcd2 == 0x23) instr = "cmp ebp, esi";
        else if(opcd2 == 0x24) instr = "cmp ebp, edi";
        else if(opcd2 == 0x25) instr = "cmp esp, eax";
        else if(opcd2 == 0x26) instr = "cmp esp, ebx";
        else if(opcd2 == 0x27) instr = "cmp esp, ecx";
        else if(opcd2 == 0x28) instr = "cmp esp, edx";
        else if(opcd2 == 0x29) instr = "cmp esp, ebp";
        else if(opcd2 == 0x30) instr = "cmp esp, esi";
        else if(opcd2 == 0x31) instr = "cmp esp, edi";
        else if(opcd2 == 0x32) instr = "cmp esi, eax";
        else if(opcd2 == 0x33) instr = "cmp esi, ebx";
        else if(opcd2 == 0x34) instr = "cmp esi, ecx";
        else if(opcd2 == 0x35) instr = "cmp esi, edx";
        else if(opcd2 == 0x36) instr = "cmp esi, ebp";
        else if(opcd2 == 0x37) instr = "cmp esi, esp";
        else if(opcd2 == 0x38) instr = "cmp esi, edi";
        else if(opcd2 == 0x39) instr = "cmp edi, eax";
        else if(opcd2 == 0x40) instr = "cmp edi, ebx";
        else if(opcd2 == 0x41) instr = "cmp edi, ecx";
        else if(opcd2 == 0x42) instr = "cmp edi, edx";
        else if(opcd2 == 0x43) instr = "cmp edi, ebp";
        else if(opcd2 == 0x44) instr = "cmp edi, esp";
        else if(opcd2 == 0x45) instr = "cmp edi, esi";
        else return real_disasm( opcd1, opcd2, addr, &size );
		
		size = 2;
		return instr;
	}
    else if(opcd1 == 0x5A) 
	{
			 if(opcd2 == 0x00) instr = "cmp byte al, bl";
        else if(opcd2 == 0x01) instr = "cmp byte al, cl";
        else if(opcd2 == 0x02) instr = "cmp byte al, dl";
        else if(opcd2 == 0x03) instr = "cmp byte bl, al";
        else if(opcd2 == 0x04) instr = "cmp byte bl, cl";
        else if(opcd2 == 0x05) instr = "cmp byte bl, dl";
        else if(opcd2 == 0x06) instr = "cmp byte cl, al";
        else if(opcd2 == 0x07) instr = "cmp byte cl, bl";
        else if(opcd2 == 0x08) instr = "cmp byte cl, dl";
        else if(opcd2 == 0x09) instr = "cmp byte dl, al";
        else if(opcd2 == 0x11) instr = "cmp byte dl, bl";
        else if(opcd2 == 0x11) instr = "cmp byte dl, cl";
        else return real_disasm( opcd1, opcd2, addr, &size );

		size = 2;
		return instr;
	}
	
	return real_disasm( opcd1, opcd2, addr, &size );
}
// ------------------------------------------------------------------------------------------------
static main()
{
	auto i, instr, veip, sz, n_instr;
	
	
	vm_offst = 0x0000;									// set VM offset
	n_instr  = 1500;									// how many instructions to disassemble
	
	real_opcd = 0x407608 + vm_offst;					// initialize offsets
	veip      = 0x40cade + vm_offst;
	decd      = 0x40A071 + vm_offst;
	is_real   = veip + 0x1200;
	
	for( i=1; i<=n_instr; i++ )							// for each instruction
	{
		instr = my_disasm(Byte(veip) ^ 0x8a, Byte(veip+1), veip, &sz);
		
		Message( "[%03d]\t%x\t%d\t%s\n", i, veip, sz, instr );
	
		veip = veip + sz;
	}
}
// ------------------------------------------------------------------------------------------------
