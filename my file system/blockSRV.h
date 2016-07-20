//-----------------------------------------------------------------------------------------
/*
**  Copyright (c) 2011
**  My Open Source Project - KiFS - Kyriakos Ispoglou
**
**  blockSRV.h:  Version 1.3
**
**  This library offers an API for file and directory management, hiding the block level
**  implementation. i-node size and directory table sizes are configurable. 
**
**  File size:
**      Files are implemented in an i-node struct. Each i-node struct contains the size
**      of the file and 15 pointers to disk blocks. The first 13 pointers are direct
**      to blocks, while the last 2 pointers are indirect pointers that point to block
**      of pointers. In our example, Block Size=1k, so we can have 13 + 1024/4 + 1024/4 
**      = 525 blocks. This means that the maximum file size is 525*1024 = 537,600 bytes,
**      or 525KB.
**
**  Directory Implementation:
**      The big advantage of directory implementation is that the maximum number of files per
**      directory is variable. Also searching for a file within a directory takes O(1) time.
**
**      There's a special directory '..', that moves to the parent directory. It could exist an
**      extra special directory '.', that moves to the current directory, but there's no reason
**      for that in this system.
**
**      We assume that in the first position of directory table there's root directory (/).
**      For there, there's a pointer to the first file of the subdirectory. This file belongs
**      to a double-linked list. Traversing that list we get all files of that directory, that
**      we store in a hash table. Thus after loading a directory in memory, file management 
**      becomes extremely fast. But when we change a directory, we have to do this time consuming
**      operation. Here's where time locality takes place: The maximum number of "open directories"
**      that can be at memory at any time is 32 (in our example). When this table becoms full, we
**      remove the least recently used open directory. A recently used directory has higher 
**      probability to be already in that table, so the time complexity is significantly reduced. 
**
**      Each record in directory table, contains the file name, the list pointers and an index.
**      If that recored in the table is file, MSB of index is 0 and the 31 LSBitss show the 
**      location of i-node in i-node table. If MSB is set, then the 31 LSBits indicate the
**      position of the first subdirectory (which is usually '..') inside directory table. From
**      there, we can read all the contents by following the pointers.
**
**
**  IMPORANT WARNING: To avoid problems, all the sizes must be powers of 2.
**
*/
//-----------------------------------------------------------------------------------------
#ifndef BLOCKSRV_H_INCLUDED
#define BLOCKSRV_H_INCLUDED

/* include files */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <map>   // required for hashing

#include "datastruct.h"  // include basic data structures
#include "virtualDisk.h" // include virtual disk class

/* define macros */
#define TXT(str) (char*)(str)
#define MAX_FDTS 32

//-----------------------------------------------------------------------------------------
enum BLOCK_SRV_ERROR_CODES /* error codes in block service */
{
    SUCCESS,             /* success. noerrors          */
    DIR_TABLE_FULL,      /* directory table is full    */
    FILE_NOT_EXISTS,     /* file does not exists       */
    FILE_ALREADY_EXISTS, /* file already exists        */
    FILE_IS_NOT_DIR,     /* file is not a directory    */
    FILE_IS_DIR,         /* file is a directory        */
    INODE_LOAD_FAIL,     /* inode loading failure      */
    INODE_STORE_FAIL,    /* inode storing failure      */
    BAD_FILE_DESCRIPTOR, /* bad file descriptor        */
    DROP_SPECIAL_DIR,    /* drop a protected directory */
    DIR_NOT_EMPTY,       /* directory is not empty     */
    INODE_CLEAR_FAIL,    /* clear i-node blocks        */
    INODES_TABLE_FULL,   /* i-nodes table is full      */
    READ_BLOCK_FAIL,     /* cannot read file block     */
    WRITE_BLOCK_FAIL,    /* cannot write file block    */
    READ_INVALID_ARG,    /* read() invalid arguments   */
    WRITE_INVALID_ARG    /* write() invalid arguments  */
};

//-----------------------------------------------------------------------------------------
class blockSRV /* main class for block service API */
{
    private:
        /*
        ** PRIVATE VARIABLE DECLARATIONS
        */
        hardDisk   *vhd;          // pointer to a virtual hard disk
        superBlock superblock;    // the superblock record
        dirTable   *dirtable;     // the directory table
        bitwise    *inodesbitmap, // a binary table of i-nodes bit map
                   *blockbitmap;  // a binary table of blocks bit map

        /*
        ** DECLARE AUXILARY VARIABLES TO PREVENT WRITING LONG NAMES
        */
        int INODEBITS;  // number of bits in i-node bit map
        int BLOCKBITS;  // number of bits in block bit map
        int N_DIRS;     // number of directory table records
        int N_INODES;   // number of i-node records
        int BLOCKNODES; // number of block records per block
        int BLOCKDIRS;  // number of directory table records per block
        int STARTFILE;  // initial position of blocks-for-files section in hard disk

        /*
        ** active directory table. This table contains the last MAX_FDTS active
        ** directories basic information.
        */
        activeDirInfo activeDir[ MAX_FDTS ];

        /*
        ** currDir: the current directory index in activeDir table.
        */
        int currDir;

        /*
        ** flag that shows if is the first time of the changing directory.
        */
        int first_time;

        /*
        ** openFile is a hash table to map the open file descriptor with a pointer
        ** to an i-node record.
        */
        map <int, iNode*> openFile;

        /*
        ** PRIVATE FUNCTION DECLARATIONS
        */
        bool loadBitMap( bitwise **bitmap, int blocklen, int start ); // load bit-map from disk
        bool storeBitMap( bitwise *bitmap, int blocklen, int start ); // store bit-map to disk

        bool loadINode( iNode* &inode,  int ufid ); // load i-node from disk
        bool storeINode( iNode *inode,  int ufid ); // store i-node to disk

        void clearINode( iNode* );                  // clear an i-node record and it's blocks

        int allocBlock( void );                     // allocate a block for a file

        bool readFileBlock( iNode*, int );                   // read the i-th block of a file
        bool writeFileBlock( iNode* &, int, char*, size_t ); // write the i-th block of file to disk

        /*
        ** LIST MANIPULATION FUNCTIONS
        */
        int  list_pushback( char*, int ); // insert a record to current's directory list
        void list_drop( int );            // drop a record from current's directory list

    public:
        /*
        ** PUBLIC FUNCTION DECLARATIONS
        */
        blockSRV( hardDisk* ); // constructor
        ~blockSRV();           // destructor

        /*
        ** FILE MANIPULATION FUNCTIONS
        */
        int files( char file[][20], char type[] ); // gets the file names of the system

        int create ( const char *filename ); // creates a new file
        int _delete( const char *filename ); // deletes an existing file

        int open( const char *filename ); // opens a file
        int close( int ufid );            // closes a file

        int file_size( int ufid );        // returns the file size

        int read ( int ufid, char* &buf, int num, int pos ); // read  bytes from an open file
        int write( int ufid, char* buf,  int num, int pos ); // write bytes to an open file

        /*
        ** DIRECTORY MANIPULATION FUNCTIONS
        */
        bool chdir( char* dirname ); // change directory
        bool mkdir( char* dirname ); // create a new directory
        bool rmdir( char* dirname ); // remove a directory

        /*
        ** ERROR REPORTING MEMBERS
        */
        int error_code;
};

//-----------------------------------------------------------------------------------------
/*
**  consturctor:
**      Loads the basic tables from hard disk (mount).
*/
blockSRV::blockSRV
(
    hardDisk *vhd // a pointer to virtual hard disk
)
{
    int blockcursor = 0; // a cursor for block indexing
    int i, j;            // auxilary counters

    this->vhd = vhd;     // store virtual disk pointer

    /*
    **  SUPERBLOCK LOADING
    */
    if( !vhd->readBlock(blockcursor++) ) exit( EXIT_FAILURE );
    else /* initialize superblock */
    {
        int *p = (int*)vhd->blockBuffer;        // auxilary pointer
        superblock.inodes         = *(p + 0);
        superblock.block_size     = *(p + 1);
        superblock.inodes_bitmap  = *(p + 2);
        superblock.block_bitmap   = *(p + 3);
        superblock.directory_table= *(p + 4);
    }

    /*
    ** SET BASIC VARIABLES, TO PREVENT WRITING LONG NAMES
    */
    INODEBITS = superblock.inodes_bitmap  * superblock.block_size * 8;
    BLOCKBITS = superblock.block_bitmap   * superblock.block_size * 8;
    N_DIRS    = superblock.directory_table* superblock.block_size / sizeof(dirTable);
    N_INODES  = superblock.inodes         * superblock.block_size / sizeof(iNode);
    BLOCKNODES= superblock.block_size / sizeof(iNode);
    BLOCKDIRS = superblock.block_size / sizeof(dirTable);
    STARTFILE = 1 + superblock.inodes_bitmap   + superblock.block_bitmap
                  + superblock.directory_table + superblock.inodes;

    /*
    ** I-NODES BIT-MAP LOADING
    */
    if( !loadBitMap(&inodesbitmap, superblock.inodes_bitmap, blockcursor) )
        exit( EXIT_FAILURE ); // function failure

    blockcursor += superblock.inodes_bitmap; // increase block cursor

    /*
    ** BLOCK BIT-MAP LOADING
    */
    if( !loadBitMap(&blockbitmap, superblock.block_bitmap, blockcursor) )
        exit( EXIT_FAILURE ); // function failure

    blockcursor += superblock.block_bitmap; // increase block cursor

    /*
    ** DIRECTORY TABLE LOADING
    */
    dirtable = new dirTable[ N_DIRS ]; // allocate memory for tables

    blockcursor += superblock.inodes;  // go to directory table

    for( i=0; i<superblock.directory_table; i++ ) // for each block
    {
        if( !vhd->readBlock(blockcursor + i) ) exit( EXIT_FAILURE ); // try to read a block

        /*
        ** store the directory table
        */
        for( j=0; j<BLOCKDIRS; j++ )
        {
            dirtable[i*BLOCKDIRS + j] = *((dirTable*)(vhd->blockBuffer) + j);
        }
    }

    openFile.clear(); /* drop all records from open-files hash */

    /*
    ** CREATE AND LOAD ROOT DIRECTORY
    */
    for( i=0; i<MAX_FDTS; i++ ) // clear active directory table
    {
        activeDir[i].dirIndex = 0;
        activeDir[i].lastfile = 0;

        activeDir[i].FDT.clear();
    }

    currDir = 0; // clear current directory index

    /*
    ** File system created or mounted ?
    */
    if( !strcmp( dirtable[0].filename, "/" ) ) first_time = 1;
    else first_time = 0;

    if( !first_time ) // create root directory
        if( !mkdir( TXT("/") ) ) exit( EXIT_FAILURE ); // abort

    // go to  root directory
    if( !chdir( TXT("/") ) ) exit( EXIT_FAILURE ); // abort

    /* function success. */
}

//-----------------------------------------------------------------------------------------
/*
**  destructor:
**      Stores basic tables back to disk (unmount).
*/
blockSRV::~blockSRV( void ) /* destructor has no arguments */
{
    int blockcursor = 0; // a cursor for block indexing

    /*
    ** SUPERBLOCK STORING
    */
    bzero( vhd->blockBuffer, superblock.block_size ); // clear block buffer

    int *p = (int*)vhd->blockBuffer;                  // auxilary pointer

    *(p + 0) = superblock.inodes;
    *(p + 1) = superblock.block_size;
    *(p + 2) = superblock.inodes_bitmap;
    *(p + 3) = superblock.block_bitmap;
    *(p + 4) = superblock.directory_table;

    if( !vhd->writeBlock(blockcursor++) ) exit( EXIT_FAILURE ); // unmount failure.

    /*
    ** I-NODES BIT MAP STORING
    */
    if( !storeBitMap(inodesbitmap, superblock.inodes_bitmap, blockcursor) )
        exit( EXIT_FAILURE ); // function failure

    blockcursor += superblock.inodes_bitmap; // increase block counter

    /*
    ** BLOCK BIT MAP STORING
    */
    if( !storeBitMap(blockbitmap, superblock.block_bitmap, blockcursor) )
        exit( EXIT_FAILURE ); // function failure

    blockcursor += superblock.block_bitmap; // increase block counter

    /*
    ** DIRECTORY TABLE STORING
    */
    blockcursor += superblock.inodes;       // go to directory table

    for( int i=0; i<superblock.directory_table; i++ )
    {
        for( int j=0; j<BLOCKDIRS; j++ )
            *((dirTable*)(vhd->blockBuffer) + j) = dirtable[i*BLOCKDIRS + j];

        if( !vhd->writeBlock(blockcursor + i) ) //attempt to write to disk
            exit( EXIT_FAILURE );
    }

    delete [] dirtable;  // release directory table memory
    delete blockbitmap;  // release block bit map memory
    delete inodesbitmap; // release i-nodes bit map memory
}
//-----------------------------------------------------------------------------------------



/******************************************************************************************
**                                                                                       **
**                                                                                       **
**                              TABLE MANAGEMENT FUNCTIONS                               **
**                                                                                       **
**                                                                                       **
*******************************************************************************************/



//-----------------------------------------------------------------------------------------
/*
**  WARNING: PRIVATE FUNCTION
**
**  loadBitMap:
**      Loads a bitmap from hard disk. Function reading blocks from the disk
**      (starting at position specified by start) and stores them in a bitwise object.
**
**  Return value: If function succeed the return value is true. Otherwise function
**                returns false.
*/
bool blockSRV::loadBitMap
(
    bitwise **bitmap,   // a pointer to a bitwise pointer object (double pointer)
    int blocklen,       // the size of blocks to allocate for bitmap
    int start           // the start position in the hard disk to allocate the blocks
)
{
    int bsize  = superblock.block_size;       // get block size

    char *temp = new char[bsize * blocklen];  // allocate memory for  blocks

    for( int i=0; i<blocklen; i++ )           // scan all blocks
    {
        if( vhd->readBlock(start + i) )       // read block successfull
        {
            memcpy( temp+i*bsize, vhd->blockBuffer, bsize ); // copy block
        }

        else return false;                    // read block failure.
    }

    *bitmap = new bitwise(bsize*blocklen*8, temp); // create bitmap

    delete [] temp;                           // release memory allocated for temp

    return true;                              // function success.
}

//-----------------------------------------------------------------------------------------
/*
**  WARNING: PRIVATE FUNCTION
**
**  storeBitMap:
**      Stores a bitmap back to hard disk. Function writing blocks from an bitwise object
**      to disk (starting at position specified by start). This function makes 'pair' with
**      loadBitMap.
**
**  Return value: If function succeed the return value is true. Otherwise function
**                returns false.
*/
bool blockSRV::storeBitMap
(
    bitwise *bitmap, // a pointer to a bitwise object
    int blocklen,    // the size of blocks to write to disk
    int start        // the start position in the hard disk to write the blocks
)
{
    int bsize = superblock.block_size;   // get block size

    for( int i=0; i<blocklen; i++ )      // for each block
    {
        /* copy current block to temporary buffer */
        memcpy( vhd->blockBuffer, (bitmap->bit+i*bsize), bsize );

        if( !vhd->writeBlock(start + i) )
            return false;                // writing failure failure.
    }

    return true;                         // function success.
}

//-----------------------------------------------------------------------------------------
/*
**  WARNING: PRIVATE FUNCTION
**
**  loadINode:
**      This function loads an i-node record from hard disk, and returns a pointer to it.
**      ufid specifies the position in i-nodes table.
**
**  Return value: If function succeed the return value is true. Otherwise function
**                returns false.
*/
bool blockSRV::loadINode
(
    iNode* &inode,  // an reference to an i-node pointer
    int ufid        // the i-node position in i-nodes table
)
{
    /* find the correct block containing requested i-node in hard disk */
    int offset = 1 + superblock.inodes_bitmap // 1 for superblock plus inode bit map block
                   + superblock.block_bitmap  // plus blocks bit map
                   + ufid / BLOCKNODES;       // plus the offset for the begin of i-node table


    if( !vhd->readBlock(offset) )             // attempt to read block
        return false;                         // read block failure

    inode = new iNode;                        // allocate memory for i-node record

    memcpy                                    // copy buffer to i-node
    (
        inode, ((iNode*)(vhd->blockBuffer) + ufid % BLOCKNODES), sizeof(iNode)
    );

    return true;                              // function success
}

//-----------------------------------------------------------------------------------------
/*
**  WARNING: PRIVATE FUNCTION
**
**  storeINode:
**      This function stores an i-node record back to hard disk. ufid specifies the
**      position in i-nodes table. storeINode makes 'pair' with loadINode.
**
**  Return value: If function succeed the return value is true. Otherwise function
**                returns false.
*/
bool blockSRV::storeINode
(
    iNode *inode,   // the i-node record to store back to disk
    int ufid        // the i-node position in i-nodes table
)
{
    /* find the correct block containing requested i-node in hard disk */
    int offset = 1 + superblock.inodes_bitmap // 1 for superblock plus inode bit map block
                   + superblock.block_bitmap  // plus blocks bit map
                   + ufid / BLOCKNODES;       // plus the offset for the begin of i-node table

    if( !vhd->readBlock(offset) )             // attempt to read block from disk
        return false;                         // read block failure.

    memcpy                                    // copy i-node to buffer
    (
        ((iNode*)(vhd->blockBuffer) + ufid % BLOCKNODES), inode, sizeof(iNode)
    );

    if( !vhd->writeBlock(offset) )            // attempt to write block back to disk
        return false;                         // write block failure.

    return true;                              // function success.
}

//-----------------------------------------------------------------------------------------
/*
**  WARNING: PRIVATE FUNCTION
**
**  list_pushback:
**      This function inserts a new file in the current directory list, and upadates the
**      pointers from other records. Also list_pushback inserts the new file to current's
**      directory hash table.
**
**  Return value: If there is no space left in directory table, function returns -1.
**                Otherwise function returns the position of the new record in
**                directory table.
*/
int blockSRV::list_pushback
(
    char *filename,  // the name of the file
    int dirindex     // the dirindex member of the dirtable entry
)
{
    int index; // position for new record in directory table

    error_code = SUCCESS; // initialize error code

    /* find an empty place for the dirtable record */
    for( index=0; index<N_DIRS; index++ )
        if( dirtable[index].filename[0] == '\0' )
        {
            break; // place found!
        }

    if( index == N_DIRS )
    {
        error_code = DIR_TABLE_FULL; // set error code
        return -1;                   // directory table is full. Abort.
    }

    strncpy( dirtable[index].filename, filename, 20 ); // copy name

    dirtable[ index ].index    = dirindex;  // set dirindex
    dirtable[ index ].next     = 0;         // new record is the last in the list
    dirtable[ index ].prev     =
            activeDir[currDir].lastfile;    // set previous record of the new record

    if( activeDir[currDir].lastfile != 0 )  // new record is the first record in the list?
    {
        // set the next pointer of the previous 'last' record to this record
        dirtable[ activeDir[currDir].lastfile ].next = index;
    }

    activeDir[currDir].lastfile = index; // last record in the list, is the new record

    if( strcmp( filename, "/" ) )
        activeDir[currDir].FDT.insert( std::make_pair(filename, index) ); // update hash

    return index; // return the position of the new record in the list
}

//-----------------------------------------------------------------------------------------
/*
**  WARNING: PRIVATE FUNCTION
**
**  list_drop:
**      This function drops a new file from the current directory list, and upadates the
**      pointers from other records. Also list_drop drops the file from current's
**      directory hash table.
**
**  Return value: None.
*/
void blockSRV::list_drop
(
    int index // the name of the element to delete
)
{
    /* remove record from list */
    if( dirtable[index].prev )
        dirtable[ dirtable[index].prev ].next = dirtable[index].next;

    if( dirtable[index].next )
        dirtable[ dirtable[index].next ].prev = dirtable[index].prev;

    activeDir[currDir].FDT.erase( dirtable[index].filename ); // drop from hash

    bzero( ((dirTable*)dirtable + index), sizeof(dirTable) ); // clear record
}
//-----------------------------------------------------------------------------------------



/******************************************************************************************
**                                                                                       **
**                                                                                       **
**                            DIRECTORY MANAGEMENT FUNCTIONS                             **
**                                                                                       **
**                                                                                       **
*******************************************************************************************/



//-----------------------------------------------------------------------------------------
/*
**  chdir:
**      Change directory. chdir change the current directory with one subdirectory. Also
**      chdir creates a hash table and inserts all files of this directory, in case dir
**      does not exist in activeDir table.
**
**  Return value: If function succeed the return value is true. Otherwise function
**                returns false.
*/
bool blockSRV::chdir
(
    char* dirname   // name of the new directory
)
{
    int index;                      // an index in dirtable

    error_code = SUCCESS;           // initialize error code

    if( first_time == false )   // initialization of file system ?
    {
        if( !strcmp(dirname, "/") || !strcmp(dirname, "~") ) // root directory request
        {
            /*
            ** Index of the first file in the root directory.
            ** Root directory is always loaded in activeDir table, in place 0.
            ** Just change the currDir index to 0 (root directory)
            */
            currDir = 0;

            return true;                  // function suceess
        }

        /* lookup for file in hash table */
        if( activeDir[currDir].FDT.find(dirname) == activeDir[currDir].FDT.end() )
        {
            error_code = FILE_NOT_EXISTS; // set error code
            return false;                 // file does not exists. Abort
        }

        index = (activeDir[currDir].FDT.find(dirname))->second; // get index in dirtable

        if( dirtable[ index ].index > 0 ) // if MSB is set, index is negative
        {
            error_code = FILE_IS_NOT_DIR; // set error code
            return false;                 // record is not a directory. Abort
        }

        index = dirtable[ index ].index & 0x7FFFFFFF; // get 31 LSBits of index

        /*
        ** Find a place for the new active directory in activeDir table.
        **
        ** We have only MAX_FDTS places for active directories. Because activeDir
        ** has limited size, it is possible to full with records, so we want an
        ** efficient way to replace active directories. Also changing directory
        ** has linear complexity (scan all files in a directory). In order to
        ** prevent from this we use the time locality to re-use most recenlty used
        ** active directories. The LRU algorithm maybe it's fine.
        **
        ** We scan the whole array, and if we find the requested directory, we update
        ** it's timestamp. Otherwise we replace the active directory with the minimum
        ** timestamp, with the new one.
        */
        timeval min = { 0x7FFFFFFF, 0x7FFFFFFF };  // declare a timeval for comparisons

        /* scan the whole table */
        for( currDir=0; currDir<MAX_FDTS; currDir++ )
        {
            if( activeDir[currDir].dirIndex == index )
            {
                return true; // file already exist. Terminate function
            }

            if( currDir > 0  &&     // root directory is always loaded
                (min.tv_sec >  activeDir[currDir].timestamp.tv_sec ||
                (min.tv_sec == activeDir[currDir].timestamp.tv_sec &&
                min.tv_usec > activeDir[currDir].timestamp.tv_usec)) )
            {
                min = activeDir[currDir].timestamp; // get minimum timestamp

                if( min.tv_sec == 0 && min.tv_usec == 0 )
                    break; // if current place is empty, stop
            }
        }

        gettimeofday(&activeDir[currDir].timestamp, NULL); // update timestamp

        /*
        ** Initialize activeDir members
        */
        activeDir[currDir].lastfile = 0;     // set last file of the new active directory
        activeDir[currDir].dirIndex = index; // set curr dir record

        /*
        ** ACTIVE DIRECTORY HASHING
        */
        if( dirtable[index].filename[0] == '\0' ) // directory is empty ?
        {
            /*
            ** if directory is empty, there is no need to scan list for files.
            */
            return true;
        }
    }
    else index = 1;     // first file in root directory

    first_time = false; // now, it is not the first time

    activeDir[currDir].FDT.clear();        // clear all previous records

    for( dirTable *curr=&dirtable[index]; ; curr=&dirtable[curr->next] ) // for each file
    {
        int pos = curr - dirtable;         // find relative position in dirtable

        activeDir[currDir].FDT.insert( std::make_pair(curr->filename, pos) );

        activeDir[currDir].lastfile = pos; // possible the last file of directory

        if( curr->next == 0 ) break;       // end of files
    }

    return true;                           // function success. currDir is updated.
}

//-----------------------------------------------------------------------------------------
/*
**  mkdir:
**      Creates a new directory in the current's directory space. Also mkdir, creates a
**      new special sub-directory ( '..' ) in the new directory, to help the directory
**      traversal.
**
**  Return value: If function succeed the return value is true. Otherwise function
**                returns false.
*/
bool blockSRV::mkdir
(
    char* dirname // name of the new directory
)
{
    int index, // position of the new dir in directory table
        ddot;  // position of the '..' subdirectory in directory table

    error_code = SUCCESS; // initialize error code

    if( activeDir[currDir].FDT.find(dirname) != activeDir[currDir].FDT.end() )
    {
        error_code = FILE_ALREADY_EXISTS; // set error code
        return false;                     // file does not exists. Abort
    }

    if( (index=list_pushback( dirname, -1 )) == -1 )  // add dir to current dir list
    {
        error_code = DIR_TABLE_FULL; // initialize error cod
        return false;                // function failure
    }

    /* find a place for the file name */
    for( ddot=0; ddot<N_DIRS; ddot++ )
        if( dirtable[ddot].filename[0] == '\0' )
        {
            break; // place found!
        }

    if( ddot == N_DIRS ) // directory table is full?
    {
        error_code = DIR_TABLE_FULL; // initialize error code
        return false;                // Abort
    }

    if( strcmp( dirname, "/" ) )
    {
        strncpy( dirtable[ddot].filename, "..", 20  ); // copy name

        dirtable[ddot].index =
            activeDir[currDir].dirIndex | 0x80000000; // set index of .., and enable MSB

        dirtable[index].index = ddot | 0x80000000;    // set index of new dir, and enable MSB

    }
    else dirtable[index].index = 0x80000001;          // set position of the first file of '/'

    return true;                                      // function success.
}

//-----------------------------------------------------------------------------------------
/*
**  rmdir:
**      Deletes a directory from the current's directory space. Directory must be emtpy
**      in order to remove.
**
**  Return value: If function succeed the return value is true. Otherwise function
**                returns false.
*/
bool blockSRV::rmdir
(
    char* dirname // name of directory to remove
)
{
    error_code = SUCCESS;

    if( !strcmp( dirname, ".." ) ) // attempt to delete special directory ?
    {
        error_code = DROP_SPECIAL_DIR; // set error code
        return false;                  // protected file. Delete failure.
    }

    /* lookup for file in hash table */
    if( activeDir[currDir].FDT.find(dirname) == activeDir[currDir].FDT.end() )
    {
        error_code = FILE_NOT_EXISTS; // set error code
        return false;                 // file does not exists. Abort
    }

    int index = (activeDir[currDir].FDT.find(dirname))->second; // get index in dirtable

    /*
    ** go to the current directory. Then go to the '..' directory. If the '..'
    ** has no next member, the directory is empty.
    */
    if( dirtable[ dirtable[ index ].index & 0x7FFFFFFF].next ) // directory is empty ?
    {
        error_code = DIR_NOT_EMPTY; // set error code
        return false;               // directory is not empty. delete failure
    }

    bzero( ((dirTable*)dirtable + (dirtable[index].index & 0x7FFFFFFF)),
            sizeof(dirTable) ); // clear record

    list_drop( index );         // drop directory

    return true;                // function success.
}
//-----------------------------------------------------------------------------------------



/******************************************************************************************
**                                                                                       **
**                                                                                       **
**                              FILE MANAGEMENT FUNCTIONS                                **
**                                                                                       **
**                                                                                       **
*******************************************************************************************/



//-----------------------------------------------------------------------------------------
/*
**  files:
**      This function enumerates all files from the current directory.
**
**  Return value: Function returns the total number of files.
*/
int blockSRV::files
(
    char filenames[][20], // a 2-dimensional array of the filenames
    char type[]           // type of file (file or directory)
)
{
    int total = 0; // total files

    // scan hash table
    for( map<string, int>::iterator ii =activeDir[currDir].FDT.begin();
                                    ii!=activeDir[currDir].FDT.end(); ++ii )
    {
        strncpy( filenames[total], ((*ii).first).c_str(), 20 );

        if( dirtable[ (*ii).second ].index & 0x80000000 ) // file is directory
        {
            type[ total ] = 'D';
        }
        else type[ total ] = 'F';

        total++;
    }

    return total; // return total files
}

//-----------------------------------------------------------------------------------------
/*
**  WARNING: PRIVATE FUNCTION
**
**  clearINode:
**      Releases Allocated blocks of an i-node record.
**
**  return value: None.
*/
void blockSRV::clearINode( iNode *inode )
{
#define BLOCK_INDICES (int)(superblock.block_size / sizeof(int))

    if( inode->file_size )
        for( int c=13; c<=14; c++ ) // for the in-direct indices
        if( inode->block[c] != -1 ) // pointer is not empty
        {
            if( !vhd->readBlock( STARTFILE + inode->block[c] ) )
            {
                error_code = INODE_CLEAR_FAIL; // set error code
                return;                        // reading failure. Abort
            }

            for( int i=0; i<(*((int*)vhd->blockBuffer + i)!=-1) && BLOCK_INDICES; i++ )
                blockbitmap->set( *((int*)vhd->blockBuffer + i), ZERO ); // clear i-nodes
        }

    for( int j=0; j<15; j++ ) // clear indices
        inode->block[j] = -1;

    inode->file_size = 0;     // clear file size

#undef BLOCK_INDICES
}

//-----------------------------------------------------------------------------------------
/*
**  create:
**      creates a file with name filename. create(), updates the directory, the i-node
**      bit map and i-node tables.
**
**  Return value: If function succeed the return value is 0. Otherwise function
**                returns -1.
*/
int blockSRV::create
(
    const char *filename  // name of file to create
)
{
    int inode_index; // the index in i-nodes bit map
    iNode *inode;    // pointer to current i-node

    error_code = SUCCESS; // set error code

    if( activeDir[currDir].FDT.find(filename) ==
        activeDir[currDir].FDT.end() )            // file exists ?
    {
        /* find a place for i-node block */
        for( inode_index=0; inode_index<INODEBITS; inode_index++ )
            if( inodesbitmap->get(inode_index) == ZERO )
            {
                break;
            }

        if( inode_index == INODEBITS ) // inode table is full ?
        {
            error_code = INODES_TABLE_FULL; // set error code
            return -1;                      // function failure
        }

        if( list_pushback( TXT(filename), inode_index ) < 0 ) // insert file to currdir's list
        {
            return -1;
        }

        inodesbitmap->set( inode_index, ONE ); // reserve a record in i-nodes table
    }
    else // file already exists
    {
        inode_index =                               // get i-node pointer
            dirtable[ (activeDir[currDir].FDT.find(filename))->second ].index;

        if( inode_index & 0x80000000 ) // file is directory ?
        {
            error_code = FILE_IS_DIR; // set error code
            return -1;                // function failure
        }
    }

    loadINode ( inode, inode_index ); // load i-node
    clearINode( inode );              // clear i-node
    storeINode( inode, inode_index ); // store i-node

    delete inode;                     // release memory allocated for inode

    return 0;                         // file created successfully.
}

//-----------------------------------------------------------------------------------------
/*
**  delete:
**      Delete a file with name filename. delete(), updates the directory, the i-node
**      bit map and i-node tables.
**
**  Return value: If function succeed the return value is 0. Otherwise function
**                returns -1.
*/
int blockSRV::_delete
(
    const char *filename // name of file to delete
)
{
    iNode *inode; // pointer to current i-node
    int index;    // the index in i-node table

    error_code = SUCCESS; // set error code

    if( activeDir[currDir].FDT.find(filename) ==
        activeDir[currDir].FDT.end() )            // file exists ?
    {
        error_code = FILE_NOT_EXISTS; // set error code
        return -1;                    // file does not exists. Abort
    }

    /* get i-node pointer */
    index = dirtable[ (activeDir[currDir].FDT.find(filename))->second ].index;

    if( index & 0x80000000 )      // index is directory ?
    {
        error_code = FILE_IS_DIR; // set error code
        return -1;                // attempt to delete directory. abort
    }

    inodesbitmap->set( index, ZERO );

    loadINode ( inode, index ); // load i-node
    clearINode( inode );        // clear inode record
    storeINode( inode, index ); // store i-node

    list_drop( index );

    activeDir[currDir].FDT.erase(filename); // drop file from hash

    return 0;                   // file deleted successfully.
}

//-----------------------------------------------------------------------------------------
/*
**  open:
**      Opens a file. open() loads the i-node record of the required file into memory
**
**  return value: On success function returns an open file descriptor. On failure
**                function retunrs -1.
*/
int blockSRV::open
(
    const char* filename // filename: name of the file we want to open
)
{
    iNode *inode = NULL; // a pointer to i-node record
    int ufid     = -1;   // file descriptor. it's the i-node i-node in i-node table.

    error_code = SUCCESS; // set error code

    /* check if file name is valid. */
    if( activeDir[currDir].FDT.find(filename) != activeDir[currDir].FDT.end() )
    {
        /* get the i-node index from directory table. */
        ufid = dirtable[ (activeDir[currDir].FDT.find(filename))->second ].index;

        if( ufid & 0x80000000 ) // index is directory ?
        {
            error_code = FILE_IS_DIR; // set error code
            return -1;                // attempt to delete directory. abort
        }

        /* load i-node into memory */
        if(loadINode(inode, ufid) < 0)
        {
            error_code = INODE_LOAD_FAIL; // set error code
            return -1; // loading i-node from disk failure.
        }

        /* insert i-node to open files hash table */
        openFile.insert(std::make_pair(ufid, inode) );
    }
    else /* file does not exits */
    {
        error_code = FILE_NOT_EXISTS;
    }

    return ufid; // file created successfully.
}

//-----------------------------------------------------------------------------------------
/*
**  close:
**      Closes an open file. close() stores the i-node record of the open file back to
**      disk.
**
**  return value: On success function returns 0. Otherwise function retunrs -1.
*/
int blockSRV::close( int ufid )
{
    error_code = SUCCESS; // set error code

    /* find the i-node of the open file in the openFile hash */
    if( openFile.find(ufid) == openFile.end() )
    {
        error_code = BAD_FILE_DESCRIPTOR; // set error code
        return -1;
    }

    iNode *inode = (openFile.find(ufid))->second;

    /* write i-node back to disk */
    if(storeINode(inode, ufid) < 0)
    {
        error_code = INODE_STORE_FAIL; // set error code
        return -1; // storing i-node to disk failure.
    }

    delete inode; // release memory allocated for inode

    /* drop i-node from openFile hash */
    openFile.erase( ufid );

    return 0; // file closed (stored) successfully.
}
//-----------------------------------------------------------------------------------------



/******************************************************************************************
**                                                                                       **
**                                                                                       **
**                          FILE DATA-MANAGEMENT FUNCTIONS                               **
**                                                                                       **
**                                                                                       **
*******************************************************************************************/



//-----------------------------------------------------------------------------------------
/*
**  file_size:
**      Gets the size of an open file.
**
**  return value: file_size() returns the file size.
*/
int blockSRV::file_size( int ufid )
{
    error_code = SUCCESS; // set error code

    /* find the i-node of the open file in the openFile hash */
    if( openFile.find(ufid) == openFile.end() )
    {
        error_code = BAD_FILE_DESCRIPTOR; // set error code
        return -1;
    }

    iNode *inode = (openFile.find(ufid))->second; // get i-npde

    return( inode->file_size ); // return file size;
}

//-----------------------------------------------------------------------------------------
/*
**  WARNING: PRIVATE FUNCTION
**
**  readFileBlock:
**      This function reads the i-th block of a file. If file has more than 13 blocks,
**      function uses indirect indices for block, so readFileBlock have to access disk 2
**      times. readFileBlock isolates the indirect-indices logic, so to access the n-th
**      block of a file we use readFileBlock( inode, n ), ignoring the indirect pointers.
**
**  return value: True on success, false on failure.
*/
bool blockSRV::readFileBlock
(
    iNode* inode,  // pointer to inode record
    int currblock  // the block to read
)
{
#define BLOCK_INDICES (int)(superblock.block_size / sizeof(int))

    if( currblock < 13 ) // the first 13 indeices is direct to blocks
    {
        // attempt to read file block (direct access)
        if( !vhd->readBlock(STARTFILE + inode->block[currblock]) )
            return false; // reading block failure.
    }
    else // we need indirect indices
    {
        /* choose the indirect pointer and the indirect block to read */
        int iblock = currblock < 13 + BLOCK_INDICES ? 13 : 14; // pick i-node pointer
        int offset = currblock - 13 - (currblock < 13 + BLOCK_INDICES ? 0 : BLOCK_INDICES); // pick indirect block

        if( !vhd->readBlock(STARTFILE + inode->block[iblock]) );//|| // attempt to read block with indices
        if( !vhd->readBlock(STARTFILE + *((int*)vhd->blockBuffer + offset)) ) // attempt to read file block
        {
            return false; // reading block failure
        }
    }

    return true; // readblock success

#undef BLOCK_INDICES
}

//-----------------------------------------------------------------------------------------
/*
**  WARNING: PRIVATE FUNCTION
**
**  allocBlock:
**      This function allocates a block from the disk.
**
**  return value: An integer contains an empty block from disk. If there are no empty
**                blocks, function returns -1.
*/
int blockSRV::allocBlock( void ) /* function has no arguments */
{
    int newblock; // index to the empty block

    /* allocate a new block */
    for( newblock=0; newblock<BLOCKBITS; newblock++ )
        if( blockbitmap->get( newblock ) == ZERO )
        {
            break; /* empty block found! */
        }

    if( newblock == BLOCKBITS )
    {
        error_code = INODES_TABLE_FULL; // set error code
        return -1;
    }

    blockbitmap->set( newblock, ONE ); // reserve block for file

    return newblock; // return block index
}

//-----------------------------------------------------------------------------------------
/*
**  WARNING: PRIVATE FUNCTION
**
**  writeFileBlock:
**      This function writes the i-th block of a file to disk. If file has more than 13
**      blocks, function uses indirect indices for block, so writeFileBlock have to access
**      disk 2 times. If inode record does not contains indirect pointers, function initialize
**      them. writeFileBlock isolates the indirect-indices logic, so to access the
**      n-th block of a file we use readFileBlock( inode, n ), ignoring the indirect pointers.
**
**  return value: True on success, false on failure.
*/
bool blockSRV::writeFileBlock
(
    iNode* &inode, // pointer to inode record
    int currblock, // the block to be written
    char *buffer,  // the buffer to write
    size_t size    // the size of the buffer
)
{
#define BLOCK_INDICES (int)(superblock.block_size / sizeof(int))

    int newblock; // index to new block.

    if( currblock < 13 ) // the first 13 indeices is direct to blocks
    {
        bzero( vhd->blockBuffer, superblock.block_size ); // clear disk buffer
        memcpy( vhd->blockBuffer, buffer, size );         // copy data to buffer

        if( inode->block[currblock] == -1 ) // pointer does not exist ? allocate a new block
            inode->block[currblock] = allocBlock();

        // attempt to write block back to disk
        if( !vhd->writeBlock(STARTFILE + inode->block[currblock]) )
            return false; // writing block failure.
    }
    else // we need indirect indices
    {
        if( currblock == 13 || currblock == 13 + BLOCK_INDICES ) // create block indices
        {
            newblock = allocBlock(); // allocate a block

            for( int i=0; i<BLOCK_INDICES; i++ ) // clear block buffer
                *((int*)vhd->blockBuffer + i ) = -1;

            inode->block[ (currblock==13 ? 13 : 14) ] = newblock; // update inode record

            // write new block to disk
            if( !vhd->writeBlock(STARTFILE + newblock) )
                return false; // writing block failure.
        }

        int iblock = currblock < 13 + BLOCK_INDICES ? 13 : 14; // pick i-node pointer
        int offset = currblock - 13 - (currblock < 13 + BLOCK_INDICES ? 0 : BLOCK_INDICES); // pick indirect block

        if( !vhd->readBlock(STARTFILE + inode->block[iblock]) )  // read block of indices
            return false; // reading block failure.

        if( (newblock=*((int*)vhd->blockBuffer+ offset)) == -1 ) /* block does not exists. */
        {
            newblock = allocBlock(); // allocate a block

            // upadate indices block
            *((int*)vhd->blockBuffer + offset) = newblock;

            if( !vhd->writeBlock(STARTFILE + inode->block[iblock]) ) // attempt to write block back to disk.
                return false; // writing block failure.
        }

        memcpy( vhd->blockBuffer, buffer, size ); // copy data to disk buffer

        if( !vhd->writeBlock(STARTFILE + newblock) ) // write data to disk
            return false; // writing block failure.eturn false;
    }

    return true; // function success

#undef BLOCK_INDICES
}

//-----------------------------------------------------------------------------------------
/*
**  read:
**      Reads an amount of bytes from an open file. read() start reading from position
**      pos, and stop till num bytes read or file ends.
**
**  return value: On success function returns the number of bytes successfully read.
**                If an error occurs, function returns -1..
*/
int blockSRV::read
(
    int ufid,   // an open file descriptor
    char* &buf, // an reference to buffer which stores the data
    int num,    // the number of bytes to read
    int pos     // the initial position in the file, to start reading
)
{
    int read_bytes = 0, // the number of bytes successfully read.
        currblock,      // a pointer to the current block of the file
        blockindex;     // a pointer to the current byte of the block

    error_code = SUCCESS; // set error code

    /* find i-node from open file descriptor */
    if( openFile.find(ufid) == openFile.end() )
    {
        error_code = BAD_FILE_DESCRIPTOR; // set error code
        return -1; // bad file descriptor. function failure.
    }

    iNode *inode = (openFile.find(ufid))->second; // get i-node

    /* make argument validation */
    if( num < 0  ||               // invalid number of bytes
        inode->file_size < pos )  // illegal initial file position
    {
        error_code = READ_INVALID_ARG; // set error code
        return -1;                     // bad arguments. function failure.
    }

    buf = new char[ num ]; // allocate memory from buffer

    currblock  = pos / superblock.block_size; // find start block
    blockindex = pos % superblock.block_size; // find start byte of the block

    if( !readFileBlock( inode, currblock ) )
    {
        error_code = READ_BLOCK_FAIL; // set error code
        return -1;
    }

    /* loop until read num bytes or file ends */
    for( int i=pos; i<pos+num && i<inode->file_size; i++ )
    {
        if( blockindex == superblock.block_size ) // end of block ?
        {
            blockindex = 0; // clear index

            if( !readFileBlock( inode, ++currblock ) )
            {
                error_code = READ_BLOCK_FAIL; // set error code
                return read_bytes;            // function failure. return read bytes so far.
            }
        }

        buf[ read_bytes++ ] = vhd->blockBuffer[ blockindex++ ]; // copy bytes into buffer.
    }

    return read_bytes; // success. return the number of bytes read
}

//-----------------------------------------------------------------------------------------
/*
**  write:
**      Writes an amount of bytes to an open file. write() start writing from position
**      pos, and stop till num bytes read or file ends.
**
**  return value: On success function returns the number of bytes successfully written.
**                If an error occurs, function returns -1.
*/
int blockSRV::write
(
    int ufid,  // an open file descriptor
    char* buf, // an reference to buffer which stores the data
    int num,   // the number of bytes to write
    int pos    // the initial position in the file, to start reading
)
{
#define MAX(a, b) (a > b ? a : b)

    int write_bytes = 0,   // the number of bytes successfully read.
        currblock,         // a pointer to the current block of the file
        blockindex;        // a pointer to the current byte of the block

    char *temp;            // temporary block buffer

    bool finalize = false; // if finalize is set stop writing to block buffer

    error_code = SUCCESS; // set error code

    /* find i-node from open file descriptor */
    if( openFile.find(ufid) == openFile.end() )
    {
        error_code = BAD_FILE_DESCRIPTOR; // set error code
        return -1; // bad file descriptor. function failure.
    }

    iNode *inode = (openFile.find(ufid))->second; // get i-node

    /* make argument validation */
    if( num < 0  ||               // invalid number of bytes
        inode->file_size < pos )  // illegal initial file position
    {
        error_code = WRITE_INVALID_ARG; // set error code
        return -1;                      // bad arguments. function failure.
    }

    currblock  = pos / superblock.block_size; // find start block
    blockindex = pos % superblock.block_size; // find start byte of the block

    if( blockindex == 0 ) // we need a new block
    {
        bzero( vhd->blockBuffer, superblock.block_size ); // clear block buffer
    }
    else // read last block from buffer. if block is not full, fill block, before change it.
    {
        if( !readFileBlock( inode, currblock ) )
        {
            error_code = READ_BLOCK_FAIL; // set error code
            return -1;
        }
    }

    temp = new char[ superblock.block_size ]; // allocate memory for temp

    for( int i=pos; ; i++ ) /* loop until write num bytes */
    {
        if( i >= pos + num ) finalize = true; // finish ?

        if( finalize || blockindex == superblock.block_size ) // check if current block is full
        {
            memcpy( temp, vhd->blockBuffer, superblock.block_size ); // copy block buffer

            /* attempt to write the block */
            if( !writeFileBlock(inode, currblock++, temp, blockindex)  )
            {
                error_code = WRITE_BLOCK_FAIL; // set error code
                return write_bytes; // function failure. return written bytes so far.
            }

            bzero( vhd->blockBuffer, superblock.block_size ); // clear block buffer
            blockindex = 0; // clear index

            if( finalize ) break; // terminate loop
        }

        vhd->blockBuffer[ blockindex++ ] = buf[ write_bytes++ ]; // copy bytes into buffer.
    }

    // update file size
    inode->file_size = MAX(inode->file_size, num + pos);

    delete [] temp; // release memory allocated for temp

    return write_bytes; // success. return the number of bytes writeen

#undef MAX
}

#endif // BLOCKSRV_H_INCLUDED
//-----------------------------------------------------------------------------------------
/*
**
**  END OF blockSRV.h
**
**  Kyriakos Ispoglou - Computer Engineering and Informatics Department
**                                   University of Patras
**
**  30/12/2011
**
*/
//-----------------------------------------------------------------------------------------
