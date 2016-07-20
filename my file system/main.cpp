//-----------------------------------------------------------------------------------------
/*
**  Copyright (c) 2011
**  My Open Source Project - KiFS - Kyriakos Ispoglou
**
**  main.h:  Version 1.2
**
**  Main program of file system.
*/
//-----------------------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <iostream>
#include <signal.h>
#include <map>

using namespace std;



#include "fileSRV.h"     // include file service API

#define MAX_CMD_LEN 256

char inp[ MAX_CMD_LEN ]; // buffer to store commands from cin
char msg[ MAX_CMD_LEN ];



int INT_recv = 1;        // auxilary variable for INT signal handling


map <string, int> cmd;   // declare a hash table to map command with ids

//-----------------------------------------------------------------------------------------
/*
**  SIGNAL HANDLER (to catch SINGINT signals)
*/
void catch_int( int sign_num )
{
    signal( SIGINT, catch_int );  // enable SIGINT signal handler again

    if( INT_recv == 1 ) exit( EXIT_SUCCESS ); // terminate program

    INT_recv = 1; // stop echo..
    sign_num = 0; // clear argunment

    cout << "\nOk writing stopped... Press any key to continue." << endl; // display message
}
//-----------------------------------------------------------------------------------------
int main()
{
    /* clear screen and display the initial messages */
    system( "clear" );

    /* Display initiallization data */
    ifstream in( "init.ini" );

    if( !in ) cout << "Cannot find 'init.ini'.\n\n";
    else
    {
        char ch;     // auzilary variable

        while( in )  /* in will be false when eof is reached */
        {
            in.get( ch );
            if( in ) cout << ch;
        }
    }

    /* map commands with integers */
    cmd.insert( std::make_pair("mkfs",    1) );
    cmd.insert( std::make_pair("mount",   2) );
    cmd.insert( std::make_pair("unmount", 3) );
    cmd.insert( std::make_pair("quit",    4) );
    cmd.insert( std::make_pair("ls",      5) );
    cmd.insert( std::make_pair("cp",      6) );
    cmd.insert( std::make_pair("rm",      7) );
    cmd.insert( std::make_pair("cd",      8) );
    cmd.insert( std::make_pair("mkdir",   9) );
    cmd.insert( std::make_pair("rmdir",  10) );
    cmd.insert( std::make_pair("cat",    11) );
    cmd.insert( std::make_pair("echo",   12) );
    cmd.insert( std::make_pair("whereis",13) );
    cmd.insert( std::make_pair("tree",   14) );

    signal( SIGINT, catch_int ); // set signal handler


    while( 1 ) // infinity loop
    {
        /* generate pre-command */
        strcpy( msg, "echo -ne \033[1m\033[31m$USER\033[0m:\033[1m\033[36m" );
        strcat( msg, Path );
        strcat( msg, "\033[0m# " );

        system( msg ); // display pre-command
        cout << " ";

        bzero( inp, MAX_CMD_LEN );       // clear command buffer
        cin.getline( inp, MAX_CMD_LEN ); // read  command

        nargs = explode( ' ', inp );     // spread command tokens

        if( cmd.find(argv[0]) == cmd.end() )
        {
            cout << argv[0] << ": Command not found" << endl;
            continue;
        }

        switch( (cmd.find(argv[0]))->second )
        {
            case 1: /* mkfs */
                if(nargs == 1) mkfs();
                else cout << "Systax error. Usage: mkfs (no parameters)" << endl;
                break;

            case 2: /* mount */
                if(nargs == 1) mount();
                else cout << "Systax error. Usage: mount (no parameters)" << endl;
                break;

            case 3: /* unmount */
                if(nargs == 1) unmount();
                else cout << "Systax error. Usage: unmount (no parameters)" << endl;
                break;

            case 4: /* quit */
                cout << "Bye bye :)" << endl; return 0;
                break;

            case 5: /* ls */
                if(nargs == 1) ls( NULL );
                else if(nargs == 2) ls( argv[1] );
                else cout << "Systax error. Usage: ls (no parameters)" << endl;
                break;

            case 6: /* cp */
                if(nargs == 3) cp(argv[1], argv[2]);
                else cout << "Systax error. Usage: cp <SOURCE> <DESTINATION>" << endl;
                break;

            case 7: /* rm */
                if(nargs == 2) rm(argv[1]);
                else cout << "Systax error. Usage: rm <FILENAME>" << endl;
                break;

            case 8: /* cd */
                if(nargs == 2) chdir(argv[1]);
                else cout << "Systax error. Usage: cd <DIRNAME>" << endl;
                break;

            case 9: /* mkdir */
                if(nargs == 2) mkdir(argv[1]);
                else cout << "Systax error. Usage: mkdir <DIRNAME>" << endl;
                break;

            case 10: /* rmdir */
                if(nargs == 2) rmdir(argv[1]);
                else cout << "Systax error. Usage: rmdir <DIRNAME>" << endl;
                break;

            case 11: /* cat */
                if(nargs == 2) cat(argv[1]);
                else cout << "Systax error. Usage: cat <FILE>" << endl;
                break;

            case 12: /* echo */
                if(nargs != 2) cout << "Systax error. Usage: echo <FILE>" << endl;
                else
                {
                #define MAXFILESIZE 15*1024
                    char *temp = new char[ MAXFILESIZE ]; // allocate memory for data
                    int len  = 0; // lengrh of data
                    INT_recv = 0; // clear interrupt signal

                    cout << "IMPORTANT WARNING: Terminate always your text with a newline." << endl;
                    cout << "Write your data below. (Ctrl+C to quit):" << endl;

                    // read from stdin until an INT signal received
                    while( !INT_recv ) temp[len++] = cin.get();

                    //strcpy( temp, "kyriakos ispgolou\n\n\n" );
                    //len = strlen( temp );

                    echo( argv[1], temp, len ); // write buf to file

                    INT_recv = 1; // set interrupt signal again
                #undef MAXFILESIZE
                }
                break;

            case 13: /* whereis */
                if(nargs == 2) whereis(argv[1]);
                else cout << "Systax error. Usage: whereis <NAME>" << endl;
                break;

            case 14: /* tree */
                if(nargs == 1) tree();
                else cout << "Systax error. Usage: tree (no parameters)" << endl;
        }
    }

    return 0;
}

#undef MAX_CMD_LEN

//-----------------------------------------------------------------------------------------
/*
**
**  END OF main.cpp
**
**  Kyriakos Ispoglou - Computer Engineering and Informatics Department
**                                   University of Patras
**
**  31/12/2011
**
*/
//-----------------------------------------------------------------------------------------
