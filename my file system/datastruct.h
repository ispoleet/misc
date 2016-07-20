//-----------------------------------------------------------------------------------------
/*
**  Copyright (c) 2011
**  My Open Source Project - KiFS - Kyriakos Ispoglou
**
**  datastruct.h: Version 1.1
**
**  This library contains the basic data structures for the special structures of
**  our filesystem.
*/
//-----------------------------------------------------------------------------------------
#ifndef DATASTRUCT_H_INCLUDED
#define DATASTRUCT_H_INCLUDED

#include <time.h>
#include <sys/time.h>

//-----------------------------------------------------------------------------------------
class superBlock /* super block basic structure. */
{
    public: /* ALL MEMBRES ARE PUBLIC */
        int inodes;         // i-node counter
        int block_size;     // the size of disk block
        int inodes_bitmap;  // the size of i-node bitmap (in blocks)
        int block_bitmap;   // the size of block bitmap (in blocks)
        int directory_table;// ths size of directory table (in blocks)
};

//-----------------------------------------------------------------------------------------
enum BIT_VALUE { ZERO=0x0,ONE =0x1 }; /* the values of a single bit. */

class bitwise // a class for bit manipulation
{
#define _INT_ (sizeof(int) << 3)
    public: /* PUBLIC MEMBER DECLARATIONS */
        int *bit; // an array of bits
        int size; // the size of bits / sizeof(int)

        /* contructor: allocates memory for bits. Size specifies the size of bits. */
        bitwise( int size, void *buffer=NULL )
        {
            this->size = (size / _INT_) + 1; // size integer size
            bit = new int[ this->size ];      // allocate memory

            if( buffer == NULL ) bzero(bit, this->size); // clear memory
            else // initialize bits.
            {
                for( int i=0; i<this->size; i++ )
                    bit[i] = *((int*)buffer + i); // copy buffer into bit map
            }
        };

        /* destructor: releases allocated memory.*/
        ~bitwise( void )
        {
            delete [] bit; // release memory
        };

        /* get the value of a bit. index shows the position */
        BIT_VALUE get( int index )
        {
            /*
            ** divide by sizeof(int)*8 to get the bit block. Modulo by sizeof(int)*8 to find the
            ** reletive distance in the integer. Shift an 1 to this position and make logic AND.
            ** Thus we return the bit. If total value is zero, then the bit is 0. otherwise the bit is set.
            */
            return( bit[index / _INT_] & (ONE << (index % _INT_)) ? ONE : ZERO);
        };

        /* set the value of a bit. index shows the position */
        void set( int index, BIT_VALUE value )
        {
            /*
            ** if the value is ONE, set the bit in the correct location and make logic OR, with
            ** bit block. If value is ZERO, clear the bit in the correct location, set all other
            ** bits and make logic AND.
            */
            bit[index / _INT_] = (value == ONE) ?
                                  bit[ index / _INT_ ] |  (ONE << (index % _INT_)) :
                                  bit[ index / _INT_ ] & ~(ONE << (index % _INT_)) ;
        };
#undef _INT_
};

//-----------------------------------------------------------------------------------------
class iNode /* i-node basic structure. */
{
    public: /* ALL MEMBERS ARE PUBLIC */
        int file_size; // the size of the file
        int block[15]; // an array of indices, shows to the original blocks on disk
};

//-----------------------------------------------------------------------------------------
class dirTable /* directory table basic sdata tructure. size: 32 bytes */
{
   // private:
    public:
        /*
        **  PRIVATE MEMBER DECLARATIONS
        */

        int next; // pointer to the next record
        int prev; // pointer to the previous record

    public:
        /*
        **  PUBLIC MEMBER DECLARATIONS
        */


        char filename[20]; // the name of the file

        /*
        **  If record is a directory, MSB of index is set. Use 31 LSBits to get the
        **  record in directory table to get the first subfile of this directory.If
        **  MSB is clear, use index to get the i-node record in i-nodes table
        */
        int index;
};

//-----------------------------------------------------------------------------------------
class activeDirInfo /* Information about current directory */
{
    public:
        /*
        ** File Descriptor Table (FDT). FDT is a hash table to map the file name with
        ** the index in Directory table.
        */
        map <string, int> FDT;

        int lastfile;      // last subfile in current directory
        int dirIndex;      // record of current directory in dirtable

        timeval timestamp; // timestamp, showing the last time referencing this directory
};

#endif // DATASTRUCT_H_INCLUDED
//-----------------------------------------------------------------------------------------
/*
**
**  END OF datastruct.h
**
**  Kyriakos Ispoglou - Computer Engineering and Informatics Department
**                                   University of Patras
**
**  13/12/2011
**
*/
//-----------------------------------------------------------------------------------------s
