//-----------------------------------------------------------------------------------------
/*
**  Copyright (c) 2011
**  My Open Source Project - KiFS - Kyriakos Ispoglou
**
**  virtualDisk.h: Version 1.2
**
**  This library creates and manages a virtual hard disk with default size.This is
**  the lowest level of abstraction.
*/
//-----------------------------------------------------------------------------------------
#ifndef VIRTUALDISK_H_INCLUDED // include once
#define VIRTUALDISK_H_INCLUDED

/* required includes */
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define BLOCKSIZE_BYTES   1024
#define SUPERBLOCK_SZ     1
#define INODEBITMAP_SZ    1
#define BLOCKBITMAP_SZ    15
#define INODES_SZ         512
#define DIRECTORYTABLE_SZ 256
#define FILEBLOCKS_SZ     122880 //122880

/* the hard disk size (in blocks) */
#define DISK_BLOCKS \
(SUPERBLOCK_SZ + INODEBITMAP_SZ    + BLOCKBITMAP_SZ +\
 INODES_SZ     + DIRECTORYTABLE_SZ + FILEBLOCKS_SZ )

enum diskMode /* the disk mode */
{
    CREATE_NEW   = 0, // create a new hard disk
    OPEN_EXISTING= 1  // open an existing hard disk
};
//-----------------------------------------------------------------------------------------
class hardDisk /* main class for hard disk with basic operations */
{
    private:
        /*
        **  PRIVATE MEMBER DECLARATIONS.
        */
        int fileDesc; // our file descriptor

    public:
        /*
        **  PUBLIC MEMBER DECLARATIONS.
        */
        char blockBuffer[ BLOCKSIZE_BYTES ]; // the temporary block buffer
        //bool validData; // if this is set, blockBuffer contains valid data.

        /*
        ** constructor: creates/opens a virtual hard disk.
        */
        hardDisk( diskMode mode )
        {
            /* attemp to open/create file */
            if( (fileDesc=open( "fileSystem.vhd", mode==OPEN_EXISTING ?
                                O_RDWR : O_RDWR|O_CREAT, S_IRWXU)) == -1 )
            {
                perror( "hardDisk open failure" );
                exit( EXIT_FAILURE ); /* function failure */
            }

            bzero( blockBuffer, BLOCKSIZE_BYTES ); // clear buffer

            if( mode == CREATE_NEW ) // format disk
            {
                const int boot[] = {
                    INODES_SZ, BLOCKSIZE_BYTES, INODEBITMAP_SZ, BLOCKBITMAP_SZ, DIRECTORYTABLE_SZ
                };

                /* write values to superblock */
                if( write(fileDesc, boot, sizeof(boot)) < 0 )
                {
                    perror( "hardDisk superblock initialization failure" );
                    exit( EXIT_FAILURE ); /* function failure */
                }

                bzero( blockBuffer, BLOCKSIZE_BYTES ); // clear buffer

                for( int i=1; i<DISK_BLOCKS; i++ ) writeBlock( i );
            }

            //validData = false; // blockBuffer contains garbage
        };

        /*
        ** desctructor: closes the hard disk file descriptor.
        */
        ~hardDisk( void )
        {
            close( fileDesc ); /* close file descriptor */
        }

        /*
        ** readBlock: reads a block from disk. offset is the relative offset of the block
        **            in the disk. block is stored in blockBuffer.
        **
        ** return value: None.
        */
        bool readBlock( int offset )
        {
            if( offset < 0 ) return false; // check if offset is positive

            if( lseek(fileDesc, offset*BLOCKSIZE_BYTES, SEEK_SET) == (off_t)(-1) || // seek
                read (fileDesc, blockBuffer, BLOCKSIZE_BYTES )    == -1 )           // read
            {
                perror( "readBlock failure" );
               return false; /* read failure. */
            }
            return true; /* read success. */
        }

        /*
        ** writeBlock: writes a block to disk. The block is stored in blockBuffer.
        **
        ** return value: True on success. Othrewise function returns false.
        */
        bool writeBlock( int offset )
        {
            if( offset < 0 ) return false; // check if offset is positive

            if( lseek(fileDesc, offset*BLOCKSIZE_BYTES, SEEK_SET) == (off_t)(-1) || // seek
                write(fileDesc, blockBuffer, BLOCKSIZE_BYTES )    == -1 )           // write
            {
                perror( "writeBlock failure" );
                return false; // function failure.
            }
            return true; // function success.
        }
};
//-----------------------------------------------------------------------------------------
#undef DISK_BLOCKS
#undef FILEBLOCKS_SZ
#undef DIRECTORYTABLE_SZ
#undef INODES_SZ
#undef BLOCKBITMAP_SZ
#undef INODEBITMAP_SZ
#undef SUPERBLOCK_SZ
#undef BLOCKSIZE_BYTES
#endif // VIRTUALDISK_H_INCLUDED
//-----------------------------------------------------------------------------------------
/*
**
**  END OF virtualDisk.h
**
**  Kyriakos Ispoglou - Computer Engineering and Informatics Department
**                                   University of Patras
**
**  13/12/2011
**
*/
//-----------------------------------------------------------------------------------------
