// ------------------------------------------------------------------------------------------------
/*
**	Athcon 2013 reverse engineering challenge
**
** 	make_comm.c: Copy comments and breakpoints from one VM to another.
*/
// ------------------------------------------------------------------------------------------------
#include <idc.idc>

static main()
{
	auto addr, comm, vm_offst, vm_name;

	vm_offst = 0x18000;							// set VM offset
	vm_name  = "VM2";							// set VM name

	Message( "\nCopy Command script started ...\n" );	
		
	/* parse the whole VM */
	for( addr=0x407708; addr<0x40CAD9; addr=FindCode(addr, SEARCH_DOWN|SEARCH_NEXT) )
	{
		MakeCode(addr+vm_offst);				// make it code
		
		comm = CommentEx(addr, 1);				// get comment
		if( strlen(comm) > 0 )					// if comment exist
		{
			MakeComm(addr+vm_offst, "");		// clear previous comments
			
			MakeRptCmt(addr+vm_offst, sprintf("%s (%s)", comm, vm_name) );		
		}
		
		if( CheckBpt(addr) == BPTCK_ACT )		// copy breakpoint
		{
			AddBptEx(addr+vm_offst, 0, BPT_SOFT);
			
			// if you want to revert any changes
			// DelBpt(addr + vm_offst);
		}
	}
}
// ------------------------------------------------------------------------------------------------
