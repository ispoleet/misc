//-----------------------------------------------------------------------------------------
/*
**  Copyright (c) 2011
**  My Open Source Project - KiFS - Kyriakos Ispoglou
**
**  fileSRV.h:  Version 1.2
**
**  This library is at the highest level of file and directory management. It offers a
**  set of shell commands, that manage files and directories
**
**
**  IMPORTANT WARNING: This is the first complete version of file & directory service
**      Some properties will be implemented better in future version.
*/
//-----------------------------------------------------------------------------------------
#ifndef FILESRV_H_INCLUDED
#define FILESRV_H_INCLUDED

#include "blockSRV.h" // include block service API

#include <stack>

hardDisk *virtualDisk  = NULL;
blockSRV *blockService = NULL;

#define MAX_CMD_LEN 256

char Path[ 256 ] = { "/" };              // string conatining the current path

char currPath[ MAX_CMD_LEN ][ 21 ];      // current path (1 directory in each row
int currCount;                           // current deep in directories

char argv[ MAX_CMD_LEN ][ MAX_CMD_LEN ]; // tokens of cmd
int nargs;                               // number of arguments

//-----------------------------------------------------------------------------------------
/*
**  explode:
**      Spreads a string into tokens. Delimiter is the character to delimiter tokens
**
**  Return value: the number of tokens
*/
int explode( char delimiter, char str[] )
{
    int i, argc=0, len=strlen(str); // local variables

    bzero( argv[0], MAX_CMD_LEN ); // clear next buffer

    for( int l=0; l<len; ) // scan the whole string
    {
        for( i=l; str[i]!=delimiter&&i<len; i++ )
            argv[argc][i-l] = str[i]; // copy characters

        if( i == l ) l++; // go to next character
        else
        {
            l = i; // go to the next token

            bzero( argv[++argc], MAX_CMD_LEN ); // clear next buffer
        }
    }

    return argc; // return the number of tokens
}

//-----------------------------------------------------------------------------------------



/******************************************************************************************
**                                                                                       **
**                                                                                       **
**                         SYSTEM INITIALIZATION FUNCTIONS                               **
**                                                                                       **
**                                                                                       **
*******************************************************************************************/



//-----------------------------------------------------------------------------------------
/*
**  mkfs:
**      Creates a new file system, and loads tables in memory
**
**  Return value: None.
*/
void mkfs( void ) /* function has no arguments */
{
    /* attempt to create a new virtual hard disk */
    if( (virtualDisk=new hardDisk(CREATE_NEW)) == NULL )
    {
        cout << "mkfs: cannot create virtual disk." << endl;
        return; // function failure
    }

    /* attemp to mount file system tables */
    if( (blockService=new blockSRV(virtualDisk)) == NULL )
    {
        cout << "mkfs: cannot mount tables." << endl;
        return; // function failure
    }

    cout << "File system created and mounted." << endl; // success!
}

//-----------------------------------------------------------------------------------------
/*
**  mount:
**      Open the virtual disk, and loads the basic tables in memory.
**
**  Return value: None.
*/
void mount( void ) /* function has no arguments */
{
    /* attempt to open the virtual disk */
    if( virtualDisk == NULL &&
        (virtualDisk=new hardDisk(OPEN_EXISTING)) == NULL )
    {
        cout << "mount: cannot locate virtual disk." << endl;
        return; // function failure
    }

    /* attemp to mount file system tables */
    if( blockService == NULL &&
        (blockService=new blockSRV(virtualDisk)) == NULL )
    {
        cout << "mount: cannot mount tables." << endl;
        return; // function failure
    }

    cout << "File system mounted." << endl; // success!
}

//-----------------------------------------------------------------------------------------
/*
**  unmount:
**      Stores the basic tables back to disk, and closes the virtual disk.
**
**  return value: None.
*/
void unmount( void ) /* function has no arguments */
{
    if( blockService == NULL )
    {
        cout << "unmount: Cannot find block service." << endl;
        return; // function failure
    }

    blockService->~blockSRV(); // destruct block service object

    if( virtualDisk == NULL )
    {
        cout << "unmount: Cannot find virtual disk." << endl;
        return; // function failure
    }

    virtualDisk->~hardDisk(); // destruct virtual disk

    virtualDisk  = NULL; // clear pointers
    blockService = NULL;

    cout << "File system unmounted." << endl; // success!
}
//-----------------------------------------------------------------------------------------



/******************************************************************************************
**                                                                                       **
**                                                                                       **
**                           DIRECTORY SERVICE FUNCTIONS                                 **
**                                                                                       **
**                                                                                       **
*******************************************************************************************/



//-----------------------------------------------------------------------------------------
/*
**  gotoPath:
**      This function travels to the path specified by fullpath. If the last element of
**      the path is file gotopath ignores last file. gotoPath does not upadate current
**      path string.
**
**  Return Value: True on success. False otherwise.
*/
bool gotoPath
(
    char *fullpath, // path to travel
    bool uselast,   // flag to use last element of the path as directory
    char *last      // name of the last element in the path
)
{
    char path[ MAX_CMD_LEN ]; // temporary buffer for the original path

    strncpy( path, fullpath, MAX_CMD_LEN ); // copy original path

    nargs = explode( '/', path ); // spread directories

    if( path[0] == '/' ) /* start from root directory ? */
    {
        if( !blockService->chdir(TXT("/")) ) // go to the root directory
        {
            cout << "cd: Cannot travel to '/' directory" << endl;
            return false; // function failure
        }
    }

    if( last != NULL )  // copy last file
        strncpy( last, argv[nargs-1], 20 );

    for( int i=0; i<(uselast ? nargs : nargs-1); i++ ) // for each directory in path
    {
        if( !blockService->chdir( argv[i] ) ) // change directory
        {
            cout << "cd: " << argv[i] << ": No such file or directory" << endl;
            return false; // function failure
        }
    }

    return true; // function success
}

//-----------------------------------------------------------------------------------------
/*
**  chdir:
**      Change directory. chdir travels the path and go to last directory (of the path).
**
**  Return value: None.
*/
void chdir
(
    char *fullpath   // path is path to the directory to change
)
{
    char path[ MAX_CMD_LEN ];

    if( blockService == NULL ) // check if block service is uninitialized
    {
        cout << "mount/mkfs required first!" << endl;
        return;
    }

    strncpy( path, fullpath, MAX_CMD_LEN ); // make a copy of fullpath

    if( !gotoPath( path, true, NULL )  )   // attempt to change path
    {
        gotoPath( Path, true, NULL );      // path is invalid, restore previous path
    }
    else
    {
        /*
        ** New path setup success. Upadate current path name
        */
        if( path[0] == '/' || path[0] == '~' ) // clear path ?
            currCount = 0;

        for( int i=0; i<nargs; i++ )
        {
            if( !strcmp( argv[i], ".." ) )       // go to parent directory ?
                currCount--;
            else if( !strcmp( argv[i], "~" ) )   // go to root directory ?
                currCount = 0;
            else strncpy( currPath[currCount++], argv[i], 20 );
        }

        bzero( Path, MAX_CMD_LEN ); // clear total path
        strcpy( Path, "/" );

        for( int i=0; i<currCount; i++ ) // set new path
        {
            strcat( Path, currPath[i] );
            if( i < currCount-1 ) strcat( Path, "/" );
        }
    }
}

//-----------------------------------------------------------------------------------------
/*
**  mkdir:
**      Make directory. mkdir travels the path, go to semifinal directory (of the path),
**      and creates a directory with name the final directory of the path.
**
**  Return value: None.
*/
void mkdir
(
    char *path   // path is path to the directory to make
)
{
    char newdir[ 21 ];                  // name of the new directory

    if( blockService == NULL ) // check if block service is uninitialized
    {
        cout << "mount/mkfs required first!" << endl;
        return;
    }

    gotoPath( path, false, newdir );    // change path

    if( !blockService->mkdir(newdir) )  // attempt to create directory
    {
        switch( blockService->error_code ) // error verbose
        {
            case DIR_TABLE_FULL:
                cout << "mkdir: There is no left space on disk" << endl;
                break;

            case FILE_ALREADY_EXISTS:
                cout << "mkdir: File already exists" << endl;
        }
    }

    gotoPath( Path, true, NULL );    // restore previous path
}

//-----------------------------------------------------------------------------------------
/*
**  rmdir:
**      Remove directory. rmdir travels the path, go to semifinal directory (of the path),
**      and deletes a directory with name the final directory of the path.
**
**  Return value: None.
*/
void rmdir
(
    char *path    // file is name of the file to delete
)
{
   char olddir[ 21 ];                  // name of the old directory

    if( blockService == NULL ) // check if block service is uninitialized
    {
        cout << "mount/mkfs required first!" << endl;
        return;
    }

    gotoPath( path, false, olddir );   // change directory

    if( !blockService->rmdir(olddir) ) // remove directory
    {
        switch( blockService->error_code ) // error verbose
        {
            case DROP_SPECIAL_DIR:
                cout << "rmdir: Attempt to delete protected file" << endl;
                break;

            case DIR_NOT_EMPTY:
                cout << "rmdir: Directory is not empty" << endl;
                break;

            case FILE_NOT_EXISTS:
                cout << "rmdir: No such file or directory" << endl;
        }
    }

    gotoPath( Path, true, NULL );    // restore previous path
}

//-----------------------------------------------------------------------------------------
/*
**  SearchTree:
**      Search the whole file system. DFS algorithm is used.
**
**  Return value: None.
*/
char Stack[ MAX_CMD_LEN ][ 21 ];  // directory stack
int SP;                           // directory stack pointer
char *req_name;                   // requested name to search for
int files_found;                  // total numbers of files found

void SearchTree
(
    char *dir, // current directory to search
    int deep,  // current deep in file system tree
    int mode   // search mode
)
{
#define MAXFILESPERDIR 64

    char file[ MAXFILESPERDIR ][ 20 ];  // array to store file names
    char type[ MAXFILESPERDIR ];        // type of file (directory or not)
    int nfiles;                         // number of files
    int i, j;                           // loop counters

    strncpy( Stack[SP++], dir, 20 );    // push directory to stack

    if( !blockService->chdir( dir ) )
        return; // change directory failure

    /* read files */
    nfiles = blockService->files( file, type );

    for( i=0; i<nfiles; i++ )     // for each file
    if( strcmp( file[i], ".." ) ) // ignore special directories
    {
        if( !mode ) /* MODE = 0. search for files */
        {
            if( !strcmp( file[i], req_name ) ) // file found ?
            {
                /*
                **  print the file with its full path
                */
                cout << ++files_found << ": ";   // set index

                for( j=1; j<SP; j++ )
                    cout << "/" << Stack[j];     // set path

                cout << "/" << req_name << endl; // set name
            }
        }
        else /* MODE = 1. print all files */
        {
            char msg[MAXFILESPERDIR],          // string containing message to display
                 pad[MAXFILESPERDIR] = {'\0'}; // string containing spaces

            for( j=0; j<deep*2; j++ ) pad[j] = ' '; // create pad

            if( type[i] == 'F' ) /* file contains data */
            {
                int ufid,   // file descriptor
                    size;   // file size

                ufid = blockService->open( file[i] ); // open file
                size = blockService->file_size(ufid); // get file size
                blockService->close( ufid );          // close file

                cout << pad << " " << file[i] << " (" << size << " Bytes)" << endl;
            }
            else /* file is directory */
            {
                /*
                ** Display directory with different color
                */
                strcpy( msg, "echo -ne '" );       // echo command
                strcat( msg, pad );                // pad with spaces
                strcat( msg, " \033[1m\033[36m" ); // bold and cyan text
                strcat( msg, file[i] );            // directory name
                strcat( msg, "\033[0m'" );         // restore color
                system( msg );                     // display command
                cout << endl;                      // new line
            }

            files_found++;  // increase total files found
        }

        if( type[i] == 'D' ) /* file is directory. continue searching */
        {
            SearchTree( file[i], deep+1, mode ); // recursion
        }
    }

    blockService->chdir( TXT("..") ); // go to parent directory
    SP--;                             // pop directory from stack

#undef MAXFILESPERDIR
}

//-----------------------------------------------------------------------------------------
/*
**  whereis:
**      Search the whole file system for a specified name
**
**  Return value: None.
*/
void whereis
(
    char *name // file name to search
)
{
    if( blockService == NULL ) // check if block service is uninitialized
    {
        cout << "mount/mkfs required first!" << endl;
        return;
    }

    req_name = name;  // set requested name value

    cout << "Searching for '" << name << "'..." << endl;

    files_found = 0;              // clear files found

    SearchTree( TXT("/"), 0, 0 ); // search tree in mode 0

    cout << endl << files_found << " file(s) total found." << endl;
}

//-----------------------------------------------------------------------------------------
/*
**  tree:
**      Display the whole file system.
**
**  Return value: None.
*/
void tree( void ) /* function has no arguments. */
{
    if( blockService == NULL ) // check if block service is uninitialized
    {
        cout << "mount/mkfs required first!" << endl;
        return;
    }

    cout << "Directory listing of *.*:" << endl;

    files_found = 0;              // clear files found

    SearchTree( TXT("/"), 0, 1 ); // search tree in mode 1

    cout << endl << files_found << " file(s) total found." << endl;

    return;
}
//-----------------------------------------------------------------------------------------



/******************************************************************************************
**                                                                                       **
**                                                                                       **
**                                FILE SERVICE FUNCTIONS                                 **
**                                                                                       **
**                                                                                       **
*******************************************************************************************/



//-----------------------------------------------------------------------------------------
/*
**  ls:
**      Listing all files in current directory
**
**  return value: None.
*/
void ls( char *path ) /* function has no arguments */
{
#define MAXFILESIZE 16384

    if( blockService == NULL )    // check if block service is uninitialized
    {
        cout << "mount/mkfs required first!" << endl;
        return;
    }

    if( path != NULL ) gotoPath( path, true, NULL ); // change path

    char file[ MAXFILESIZE ][ 20 ];  // array to store file names
    char type[ MAXFILESIZE ];
    int nfiles;                      // number of files

    nfiles = blockService->files( file, type ); // get files

    if( nfiles < 0 ) cout << "ls: cannot list files." << endl;
    else
    {
        //cout << "Directory listing of *.*:" << endl;

        for( int i=0; i<nfiles; i++ )
        {
            if( type[i] == 'F' )  cout << file[i] << endl; /* display file */
            else                                           /* display directory */
            {
                char temp[ 64 ]; // temporary string buffer

                strcpy( temp, "echo -ne \033[1m\033[36m" ); // change color: CYAN
                strcat( temp, file[i] );                    // set name
                strcat( temp, "\033[0m " );                 // restore color: WHITE
                system( temp );                             // send message
                cout << endl;                               // change line
            }
        }
    }

    if( path != NULL ) gotoPath( Path, true, NULL );    // restore path

#undef MAXFILESIZE
}

//-----------------------------------------------------------------------------------------
/*
**  cp:
**      Copies the contents of a file to another.
**
**  return value: None.
*/
void cp
(
    char *sourcepath, // source file to copy
    char *destpath    // destination file to paste
)
{
    if( blockService == NULL ) // check if block service is uninitialized
    {
        cout << "mount/mkfs required first!" << endl;
        gotoPath( Path, true, NULL );    // restore initial path
        return;
    }

    char source[21],  // source file name
         dest[21];    // destination file name

    int ufid1, ufid2; // the file descriptors
    int size;         // the size of the file

    gotoPath( sourcepath, false, source );    // change path

    if( (ufid1=blockService->open(source)) == -1 )  // open source file
    {
        cout << "cp: Cannot find " << source << endl;
        gotoPath( Path, true, NULL );    // restore initial path
        return; // function failure
    }

    gotoPath( destpath, false, dest );    // change path

    /* create and open the new file */
    if( blockService->create(dest)       == -1 ||
        (ufid2=blockService->open(dest)) == -1 )
    {
        cout << "cp: Cannot create " << dest << endl;

        return; // function failure
    }

    gotoPath( sourcepath, false, source );    // change path

    if( (size=blockService->file_size(ufid1)) < 0 ) // get file size
    {
        cout << "cp: Cannot get file size of " << source << endl;
        gotoPath( Path, true, NULL );    // restore initial path
        return; // function failure
    }

    char *buf = new char[ size ]; // allocate memory for buffer

    if( blockService->read( ufid1, buf, size, 0 ) < 0 )  // read from source
    {
        cout << "cp: Cannot read from " << source << endl;
        gotoPath( Path, true, NULL );    // restore initial path
        return; // function failure
    }

    gotoPath( destpath, false, dest );    // change path

    if( blockService->write( ufid2, buf, size, 0 ) < 0 ) // write to destination
    {
        cout << "cp: Cannot write to " << dest << endl;
        gotoPath( Path, true, NULL );    // restore initial path
        return; // function failure
    }

    delete [] buf; // release memory allocated for buffer

    gotoPath( sourcepath, false, source );  // change path
    blockService->close(ufid1);             // close file 1

    gotoPath( destpath, false, dest );      // change path
    blockService->close(ufid2);             // close file 2

    gotoPath( Path, true, NULL );           // restore initial path

    /* function success */
}

//-----------------------------------------------------------------------------------------
/*
**  rm:
**      Deletes a file.
**
**  return value: None.
*/
void rm
(
    char *filepath // file is name of the file to delete
)
{
    if( blockService == NULL )  // check if block service is uninitialized
    {
        cout << "mount/mkfs required first!" << endl;
        return;
    }

    char file[ 21 ]; // file name to delete

    gotoPath( filepath, false, file );    // change path

    if( blockService->_delete(file) < 0 )  // delete file
    {
        switch( blockService->error_code ) // verbose error
        {
            case FILE_NOT_EXISTS:
                cout << "rm: No such file '" << file << "'\n";
                break;
            case FILE_IS_DIR:
                cout << "rm: File '" << file << "' is directory" << endl;
        }
    }

    gotoPath( Path, true, NULL );    // restore initial path

    /* function success */
}

//-----------------------------------------------------------------------------------------
/*
**  cat:
**      Displays the contents of a file
**
**  return value: None.
*/
void cat
(
    char *filepath // file is name of the file to display
)
{
    int ufid; // the file descriptor

    if( blockService == NULL ) // check if block service is uninitialized
    {
        cout << "mount/mkfs required first!" << endl;
        return;
    }

    char file[ 21 ]; // file name to delete

    gotoPath( filepath, false, file );    // change path

    if( (ufid=blockService->open(file)) == -1 )
    {
        gotoPath( Path, true, NULL );    // restore initial path

        switch( blockService->error_code )
        {
            case FILE_NOT_EXISTS:
                cout << "cat: No such file '" << file << "'\n";
                return;
            case FILE_IS_DIR:
                cout << "cat: File '" << file << "' is directory" << endl;
                return;
            default:
                cout << "cat: display fialure\n";
        }
    }
    else
    {
        int size; // the file size.

        if( (size=blockService->file_size(ufid)) < 0 )
        {
            cout << "cat: Cannot get file size." << endl;
            gotoPath( Path, true, NULL );    // restore initial path
            return; // function failure
        }

        char *buf = new char[ size ];             // allocate memory for buffer

        blockService->read( ufid, buf, size, 0 ); // read file

        /* file may contain NULL bytes, so cout << buf, will fail. */
        for( int i=0; i<size; i++ )
            cout << buf[i]; // display file

        delete [] buf; // release memory allocated for buffer

        if( blockService->close( ufid ) < 0 )
        {
            cout << "cat: Cannot close file." << endl;
        }

        cout << endl << endl;        // newline padding
    }

    gotoPath( Path, true, NULL );    // restore initial path

    /* function success */
}

//-----------------------------------------------------------------------------------------
/*
**  echo:
**      Writes data to file. If file does not exist, echo creates the file. Otherwise,
**      echo appends data tp file
**
**  return value: None.
*/
void echo
(
    char *filepath, // the file name to write
    char *buf,      // the data to add
    int len         // the length of the data
)
{
    int ufid;        // the file descriptor
    int st_wr=0;     // the position in the file to start writing

    char file[ 21 ]; // file name to delete

    if( blockService == NULL )  // check if block service is uninitialized
    {
        cout << "mount/mkfs required first!" << endl;
        return;
    }

    gotoPath( filepath, false, file );    // change path

    if( (ufid=blockService->open(file)) < 0 )  // file does not exist. Create it.
    {
        if( blockService->create(file) == -1 ) // create a new file
        {
            gotoPath( Path, true, NULL );    // restore initial path

            switch( blockService->error_code ) // verbose error
            {
                case INODES_TABLE_FULL:
                case DIR_TABLE_FULL:
                    cout << "echo: Not enough space on left on disk" << endl;
                    return;

                case FILE_IS_DIR:
                    cout << "echo: file '" << file << "' is a directory\n";
                    return;
            }
        }

        ufid = blockService->open( file );   // open the file
    }

    st_wr = blockService->file_size( ufid ); // get the file size

    /* write data to file. If file already contains data, append data to the end */
    if( blockService->write( ufid, buf, len, st_wr ) < 0 ||
        blockService->close( ufid ) < 0 )
    {
        cout << "echo: Cannot write to file '" << file << "'\n";
        gotoPath( Path, true, NULL );    // restore initial path
        return; // function failure
    }

    gotoPath( Path, true, NULL );    // restore initial path

    /* function success */
}


#undef MAX_CMD_LEN

#endif // FILESRV_H_INCLUDED
//-----------------------------------------------------------------------------------------
/*
**
**  END OF fileSRV.h
**
**  Kyriakos Ispoglou - Computer Engineering and Informatics Department
**                                   University of Patras
**
**  31/12/2011
**
*/
//-----------------------------------------------------------------------------------------

