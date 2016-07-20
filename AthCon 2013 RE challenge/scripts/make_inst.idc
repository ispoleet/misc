// ------------------------------------------------------------------------------------------------
/*
**	Athcon 2013 reverse engineering challenge
**
** 	make_inst.c: Create comments and add breakpoints on "mov", "cmp", "mov byte" and "cmp byte"
**		instructions.
*/
// ------------------------------------------------------------------------------------------------
#include <idc.idc>

static main()
{
	auto addr_mov, addr_cmp;
	auto reg1, reg2;
	auto i, j;
	auto comm;
	auto hdl;
	
	
	Message( "\nMake Instruction script started ...\n" );
	
	/*
	** Do the mov and cmp instructions first.
	** Register order: eax, ebx, ecx, edx, ebp, esp, esi, edi 
	*/
	addr_mov = 0x0040B35C;								// initialize addresses
	addr_cmp = 0x0040ACCC;
	
	hdl = CreateArray( "regs" );						// initialize array
	SetArrayString(hdl, 1, "eax" );
	SetArrayString(hdl, 2, "ebx" );
	SetArrayString(hdl, 3, "ecx" );
	SetArrayString(hdl, 4, "edx" );
	SetArrayString(hdl, 5, "ebp" );
	SetArrayString(hdl, 6, "esp" );
	SetArrayString(hdl, 7, "esi" );
	SetArrayString(hdl, 8, "edi" );

	for( i=1; i<=8; i++ )								// for each pair
	for( j=1; j<=8; j++ )
	{
		if( i == j ) continue;							// regs must be differ
		
		reg1 = GetArrayElement(AR_STR, hdl, i);			// get regs name
		reg2 = GetArrayElement(AR_STR, hdl, j);
				
		MakeRptCmt(addr_mov, sprintf( "mov %s, %s", reg1, reg2) );
		AddBptEx(addr_mov, 0, BPT_SOFT);
		
		MakeRptCmt(addr_cmp, sprintf( "cmp %s, %s", reg1, reg2) );
		AddBptEx(addr_cmp, 0, BPT_SOFT);

//  In case of a mistake, delete comments and breakpoints:
//		MakeRptCmt(addr_mov, "" );
//		DelBpt(addr_mov);

		addr_mov = addr_mov + 0x21;						// go to the next instruction
		addr_cmp = addr_cmp + 0x1E;
	}
	
	DeleteArray( hdl );									// delete array (recycle it)

	
	/*
	** Now "mov byte" and "cmp byte" instructions.
	** Register order: al, bl, cl, dl
	*/
	addr_cmp = 0x0040A9D8;								// initialize addresses
	addr_mov = 0x0040AB40;
	
	hdl = CreateArray( "regs" );						// initialize array
	SetArrayString(hdl, 1, "al" );
	SetArrayString(hdl, 2, "bl" );
	SetArrayString(hdl, 3, "cl" );
	SetArrayString(hdl, 4, "dl" );
	
	for( i=1; i<=4; i++ )								// for each pair
	for( j=1; j<=4; j++ )
	{
		if( i == j ) continue;							// regs must be differ
		
		reg1 = GetArrayElement(AR_STR, hdl, i);
		reg2 = GetArrayElement(AR_STR, hdl, j);

		MakeRptCmt(addr_mov, sprintf( "mov byte %s, %s", reg1, reg2) );
		AddBptEx(addr_mov, 0, BPT_SOFT);
		
		MakeRptCmt(addr_cmp, sprintf( "cmp byte %s, %s", reg1, reg2) );
		AddBptEx(addr_cmp, 0, BPT_SOFT);
			
		addr_mov = addr_mov + 0x21;						// go to the next instruction
		addr_cmp = addr_cmp + 0x1E;
	}
		
	DeleteArray( hdl );

}
// ------------------------------------------------------------------------------------------------
