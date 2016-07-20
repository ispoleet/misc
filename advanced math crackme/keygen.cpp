//-------------------------------------------------------------------------------------------------
/*
**  mopy's advanced math crackme
**  October 2013
**
**  This crackme uses MIRACL library in order to implement an Elliptic Curve Digital Signature
**  Algorithm (ECDSA).
*/
//-------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <ctime>

#include "ecn.h"
#include "big.h"

#define LEN 128

char v_40F030[ 256 ] =
{
    0x2D, 0x00, 0x00, 0x00, 0x6D, 0x6F, 0x70, 0x79, 0x00, 0x00, 0x00, 0x00, 0x53, 0x6F, 0x72, 0x72,
    0x79, 0x2C, 0x79, 0x6F, 0x75, 0x72, 0x20, 0x6B, 0x65, 0x79, 0x20, 0x69, 0x73, 0x20, 0x69, 0x6E,
    0x76, 0x61, 0x6C, 0x69, 0x64, 0x21, 0x00, 0x00, 0x54, 0x68, 0x61, 0x6E, 0x6B, 0x20, 0x79, 0x6F,
    0x75, 0x20, 0x66, 0x6F, 0x72, 0x20, 0x79, 0x6F, 0x75, 0x72, 0x20, 0x72, 0x65, 0x67, 0x69, 0x73,
    0x74, 0x72, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x21, 0x00, 0x00, 0x00, 0x00, 0x31, 0x39, 0x30, 0x44,
    0x30, 0x30, 0x39, 0x38, 0x34, 0x41, 0x34, 0x35, 0x32, 0x30, 0x36, 0x30, 0x43, 0x34, 0x32, 0x44,
    0x41, 0x39, 0x41, 0x30, 0x46, 0x00, 0x00, 0x00, 0x45, 0x38, 0x32, 0x32, 0x45, 0x43, 0x37, 0x36,
    0x43, 0x36, 0x42, 0x34, 0x32, 0x43, 0x33, 0x46, 0x46, 0x43, 0x37, 0x37, 0x45, 0x45, 0x32, 0x36,
    0x00, 0x00, 0x00, 0x00, 0x33, 0x37, 0x37, 0x36, 0x37, 0x34, 0x31, 0x43, 0x39, 0x35, 0x30, 0x45,
    0x43, 0x32, 0x39, 0x42, 0x43, 0x30, 0x41, 0x30, 0x43, 0x42, 0x37, 0x41, 0x38, 0x00, 0x00, 0x00,
    0x33, 0x37, 0x37, 0x36, 0x37, 0x34, 0x31, 0x43, 0x39, 0x35, 0x30, 0x45, 0x43, 0x36, 0x44, 0x46,
    0x39, 0x30, 0x39, 0x41, 0x41, 0x46, 0x38, 0x41, 0x37, 0x00, 0x00, 0x00, 0x32, 0x33, 0x31, 0x41,
    0x44, 0x33, 0x30, 0x32, 0x31, 0x37, 0x35, 0x44, 0x38, 0x45, 0x34, 0x32, 0x37, 0x37, 0x41, 0x44,
    0x38, 0x30, 0x42, 0x30, 0x33, 0x00, 0x00, 0x00, 0x32, 0x34, 0x39, 0x36, 0x31, 0x43, 0x43, 0x39,
    0x32, 0x45, 0x41, 0x39, 0x41, 0x42, 0x45, 0x36, 0x45, 0x31, 0x31, 0x37, 0x35, 0x43, 0x38, 0x38,
    0x46, 0x00, 0x00, 0x00, 0x33, 0x37, 0x37, 0x36, 0x37, 0x34, 0x31, 0x43, 0x39, 0x35, 0x30, 0x45
};

char v_40F160[ 256 ] =
{
    0x29, 0x2E, 0x43, 0xC9, 0xA2, 0xD8, 0x7C, 0x01, 0x3D, 0x36, 0x54, 0xA1, 0xEC, 0xF0, 0x06, 0x13,
    0x62, 0xA7, 0x05, 0xF3, 0xC0, 0xC7, 0x73, 0x8C, 0x98, 0x93, 0x2B, 0xD9, 0xBC, 0x4C, 0x82, 0xCA,
    0x1E, 0x9B, 0x57, 0x3C, 0xFD, 0xD4, 0xE0, 0x16, 0x67, 0x42, 0x6F, 0x18, 0x8A, 0x17, 0xE5, 0x12,
    0xBE, 0x4E, 0xC4, 0xD6, 0xDA, 0x9E, 0xDE, 0x49, 0xA0, 0xFB, 0xF5, 0x8E, 0xBB, 0x2F, 0xEE, 0x7A,
    0xA9, 0x68, 0x79, 0x91, 0x15, 0xB2, 0x07, 0x3F, 0x94, 0xC2, 0x10, 0x89, 0x0B, 0x22, 0x5F, 0x21,
    0x80, 0x7F, 0x5D, 0x9A, 0x5A, 0x90, 0x32, 0x27, 0x35, 0x3E, 0xCC, 0xE7, 0xBF, 0xF7, 0x97, 0x03,
    0xFF, 0x19, 0x30, 0xB3, 0x48, 0xA5, 0xB5, 0xD1, 0xD7, 0x5E, 0x92, 0x2A, 0xAC, 0x56, 0xAA, 0xC6,
    0x4F, 0xB8, 0x38, 0xD2, 0x96, 0xA4, 0x7D, 0xB6, 0x76, 0xFC, 0x6B, 0xE2, 0x9C, 0x74, 0x04, 0xF1,
    0x45, 0x9D, 0x70, 0x59, 0x64, 0x71, 0x87, 0x20, 0x86, 0x5B, 0xCF, 0x65, 0xE6, 0x2D, 0xA8, 0x02,
    0x1B, 0x60, 0x25, 0xAD, 0xAE, 0xB0, 0xB9, 0xF6, 0x1C, 0x46, 0x61, 0x69, 0x34, 0x40, 0x7E, 0x0F,
    0x55, 0x47, 0xA3, 0x23, 0xDD, 0x51, 0xAF, 0x3A, 0xC3, 0x5C, 0xF9, 0xCE, 0xBA, 0xC5, 0xEA, 0x26,
    0x2C, 0x53, 0x0D, 0x6E, 0x85, 0x28, 0x84, 0x09, 0xD3, 0xDF, 0xCD, 0xF4, 0x41, 0x81, 0x4D, 0x52,
    0x6A, 0xDC, 0x37, 0xC8, 0x6C, 0xC1, 0xAB, 0xFA, 0x24, 0xE1, 0x7B, 0x08, 0x0C, 0xBD, 0xB1, 0x4A,
    0x78, 0x88, 0x95, 0x8B, 0xE3, 0x63, 0xE8, 0x6D, 0xE9, 0xCB, 0xD5, 0xFE, 0x3B, 0x00, 0x1D, 0x39,
    0xF2, 0xEF, 0xB7, 0x0E, 0x66, 0x58, 0xD0, 0xE4, 0xA6, 0x77, 0x72, 0xF8, 0xEB, 0x75, 0x4B, 0x0A,
    0x31, 0x44, 0x50, 0xB4, 0x8F, 0xED, 0x1F, 0x1A, 0xDB, 0x99, 0x8D, 0x33, 0x9F, 0x11, 0x83, 0x14
};

char h[LEN];        // hash table
//-------------------------------------------------------------------------------------------------
/*
**  shuffle(): This is the core of hashing algorithm. It makes some XORs with elements of hash
**      table.
**
**  Return Value: A pointer to hash table.
*/
void shuffle( void )
{
#define I(a) (*(int*) a)

    int i, j, eax, edx;

	I(&h[32]) = I(&h[16]) ^ I(&h[0]);
	I(&h[36]) = I(&h[20]) ^ I(&h[4]);
	I(&h[40]) = I(&h[24]) ^ I(&h[8]);
	I(&h[44]) = I(&h[28]) ^ I(&h[12]);

	edx = h[0x3f] & 0xff;

    for( i=-0x18; i!=0; i++ )
	{
		edx ^= h[0x28 + i];

		h[0x48 + i] ^= v_40F160[ edx & 0xff ];

		edx = h[0x48 + i] & 0xff;
	}

	for( eax=0, j=0; j<18; j++, eax+=j-1 )
	for( i=-0x30; i!=0; i++ )
	{
		h[0x30 + i] ^= v_40F160[ eax & 0xff ];
		eax = h[0x30 + i];
	}
}
//-------------------------------------------------------------------------------------------------
/*
**  copy_name(): This function copies the user name to a hash table, and performs (if need) the
**      hash function
**
**  Return Value: None.
*/
void copy_name( const char *name, const int len )
{
	int ebx, edx;

	for( ebx=len; ebx!=0;  ebx -= edx)
	{
		edx = (ebx > 16) ? 16 : ebx;
		memcpy(&h[16],name,edx);

		if( edx == 16 ) { shuffle(); I(&h[0x44]) = 0; }
		else I(&h[0x44]) = edx;

		name += 16;
	}
}
//-------------------------------------------------------------------------------------------------
/*
**  pre_shuffle(): This function prepares the main shuffle.
**
**  Return Value: A pointer to hash table.
*/
char *pre_shuffle( void )
{
	memset( &h[16 + I(&h[0x44])], (16 - I(&h[0x44])) & 0xff, (16 - I(&h[0x44])) & 0xff );
	shuffle();
	memcpy(&h[16],&h[48],16);
	shuffle();

	return h;
}
//-------------------------------------------------------------------------------------------------
/*
**  hex_to_ascii(): This function is too simple. It just converts a hex string to ASCII.
**
**  Return Value: None.
*/
void hex_to_ascii(char* in, int length, char* out)
{
#define CHR(a) ( (a) < 10 ? '0' + (a) : 'A' + ((a)-10) )

    for( int i=0; i<length; i++ )
    {
        out[2*i]   = CHR((in[i] >> 4) & 0xf);
        out[2*i+1] = CHR( in[i]       & 0xf);
    }
}
//-------------------------------------------------------------------------------------------------
/*
**  sig_get(): This function generates a digital signature of a message hash.
**
**  Return Value: None.
*/
void sig_gen
(
    epoint *const   P,              // INP: A point on curve
    big             d,              // INP: private key
    big             e,              // INP: hashed message
    big             n,              // INP: modulo n
    big            &r,              // OUT: r
    big            &s               // OUT: s
)
{
    epoint* kP = epoint_init();     // initialize curve point
    big     k  = mirvar( 0 );       // initialize big numbers
    big     x1 = mirvar( 0 );
    big     y1 = mirvar( 0 );


    // We can pick a new, random k each time. To keep code simple, we use the same k
    cinstr(k, "123456789CAFEBABE"); // pick a random k

    ecurve_mult(k, P, kP);          // kP = k*P
    epoint_get (kP, x1, y1);        // k*P = (x1,y1)

    power(x1, 1, n, r);             // r = x1 mod n
    // FUN FACT: At first, I used powmod, which is indentical with power, except that
    // the exponent is big num and not integer as power. When powmod() was called the
    // value of epoint P changed!

    xgcd(k, n, k, k, k);            // k = k^-1 mod n

    multiply(r, d, s);              // s = d * r
    add(s, e, s);                   // s = e + d*r
    multiply(s, k, s);              // s = k^-1*(e + d*r)
    power(s, 1, n, s);              // s = k^-1*(e + d*r) mod n
}
//-------------------------------------------------------------------------------------------------
/*
**  sig_ver(): This function performs the verification of digital signature.
**
**  Return Value: If signature is correct, the return value is 0. Otherwise function returns an
**      non zero value.
*/
int sig_ver
(
    epoint *const   P,              // INP: A point on curve
    epoint *const   Q,              // INP: Public Key
    const big       n,              // INP: modulo n
    const big       e,              // INP: message hash
    const big       r,              // INP: r value
    const big       s               // INP: s value
)
{
    epoint  *X   = epoint_init();   // initialize curve points
    epoint  *u1P = epoint_init();
    big     u1   = mirvar( 0 );     // initialize big numbers
    big     u2   = mirvar( 0 );
    big     x1   = mirvar( 0 );
    big     y1   = mirvar( 0 );


    // The first step is to check is r and s are in range [1, n-1].
    // we don't need to make this check.
    xgcd(s, n, s, s, s);            // s = s^-1 mod n

    multiply(e, s, u1);             // u1 = e * w
    multiply(r, s, u2);             // u2 = r * w
    power (u1, 1, n, u1);           // u1 = u1 mod n
    power (u2, 1, n, u2);           // u2 = u2 mod n

    ecurve_mult(u1, P, u1P);        // u1P = u1 * P
    ecurve_mult(u2, Q, X);          // X = u2 * Q
    ecurve_add (u1P, X);            // X = u1*P + u2*Q
    epoint_get(X, x1, y1);          // x1 = (x1, y1) => r = x1
    power(x1, 1, n, x1);            // x1 = x1 mod n

    return 0;
//    return compare(x1, r);          // return true if signature verification is successfull
}
//-------------------------------------------------------------------------------------------------
int main( void )
{
    char name[120] = { '\0' },      // user name
         tmp [128],                 // temporary buffer
         hash[128],                 // name hash
         ch;                        // store user input


    miracl *mip = mirsys(128,0);    // initialize miracl library
    epoint* P   = epoint_init();    // initialize curve points
    epoint* Q   = epoint_init();
    big     b1  = mirvar(1);        // initialize big nums
    big     A   = mirvar(0);
    big     B   = mirvar(0);
    big     p   = mirvar(0);
    big     n   = mirvar(0);
    big     e   = mirvar(0);
    big     d   = mirvar(0);        // private key
    big     xp  = mirvar(0);        // coordinates of point P
    big     yp  = mirvar(0);
    big     xq  = mirvar(0);        // coordinates of point Q
    big     yq  = mirvar(0);
    big     r   = mirvar(0);
    big     s   = mirvar(0);

     mip->IOBASE=16;                // set base of input numbers to hex
    // ------------------------------------------------------------------------
    cinstr(xp, "24961CC92EA9ABE6E1175C88F");        // x coordinate of point P
    cinstr(yp, "E822EC76C6B42C3FFC77EE26");         // y coordinate of point P
    cinstr(A,  "3776741C950EC29BC0A0CB7A4");        // parameter A of elliptic curve
    cinstr(B,  "231AD302175D8E4277AD80B03");        // parameter B of elliptic curve
    cinstr(n,  "3776741C950EC6DF909AAF8A7");        // modulo n
    cinstr(p,  "3776741C950EC29BC0A0CB7A8");        // modulo p
    cinstr(d,  "1337b4dc0d3");                      // initial private key

    // ------------------------------------------------------------------------
    printf( "+--------------------------------------------------+\n" );
    printf( "|      mopy's advanced math crackme (level 3)      |\n" );
    printf( "|                 keygen by ispo                   |\n" );
    printf( "+--------------------------------------------------+\n" );
    printf( "Use the private key of patched crackme version [Y/n]? " );
    scanf ( "%c", &ch );

    if( ch == 'y' || ch == 'Y' )
    {
        printf( "Setting private key for patched version... d = " );
        memset(tmp,0,LEN); cotstr(d, tmp); printf( "%s\n", tmp );
    }
    else
    {
        printf( "Generating a random private key... d = " );

        bigrand(n, d);                   // generate a random private key
        memset(tmp,0,LEN); cotstr(d, tmp); printf( "%s\n", tmp );
    }

    // ------------------------------------------------------------------------
    subtract(p, b1, p);                 // p = p - 1
    ecurve_init(A, B, p, 0);            // y^2 = x^3 + Ax + B (mod p)
    epoint_set(xp, yp, 0, P);           // P = (xp, yp)

    ecurve_mult(d, P, Q);               // Q = d * P
    epoint_get(Q, xq, yq);              // Q = (xq, yq)

    printf( "Calculating public key.... Q = d*P\n" );
    memset(tmp,0,LEN); cotstr(xq, tmp); printf("Qx = %s \t(fix 0x40F07C with it)\n", tmp);
    memset(tmp,0,LEN); cotstr(yq, tmp); printf("Qy = %s \t(fix 0x401F40 with it)\n", tmp);

    // ------------------------------------------------------------------------
    printf( "\n\nEnter your name: " );
    scanf ( "%s", name );               // read name

    copy_name(name, strlen(name));      // copy name
    pre_shuffle();                      // hash name
    hex_to_ascii(h, 16, hash);          // convert to ASCII

    printf( "Hashing name... H(name) = %s\n", hash );

    // ------------------------------------------------------------------------
    printf( "Generate Digital Signature...\n" );

    cinstr(e, hash);                    // e = H(name)
    sig_gen( P, d, e, n, r, s );        // generate signature

    printf( "Serial is: " );
    memset(tmp,0,LEN); cotstr(r, tmp); printf( "%s-", tmp );
    memset(tmp,0,LEN); cotstr(s, tmp); printf( "%s\n", tmp );

    // ------------------------------------------------------------------------
/*
    printf( "Verifying Serial (optional)...\n" );

    if( sig_ver( P, Q, n, e, r, s ) == 0 )
         printf( "Verification Successfull.\n" );
    else printf( "Verification Failed.\n" );
*/

    return 0;
}
//-------------------------------------------------------------------------------------------------
