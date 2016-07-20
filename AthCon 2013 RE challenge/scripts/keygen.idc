// ------------------------------------------------------------------------------------------------
/*
**	Athcon 2013 reverse engineering challenge
**
** 	keygen.idc: Generate valid key files.
*/
// ------------------------------------------------------------------------------------------------
#include <idc.idc>

// ------------------------------------------------------------------------------------------------
static main()
{
	auto k1, k2, k3, k4, vector, fp;
	
	
	Message( "Keygen Started ...\n" );

	
	k1 = 0x12345678;
	k2 = 0x9abcdef0;
	k3 = 0xdeadbeef;
	vector = 0xc3ec8a62 ^ 0x4292f007 ^ 0xe9e6474e ^ 0x55ca2c39;
	
	k4 = vector ^ k1 ^ k2 ^ k3 ^ (0x40d44e+0x4e1A9001);

	fp = fopen( "g", "wb" );
	
	writelong(fp, k1, 0);
	writelong(fp, k2, 0);
	writelong(fp, k3, 0);
	writelong(fp, k4, 0);

	fclose(fp);
}
// ------------------------------------------------------------------------------------------------
