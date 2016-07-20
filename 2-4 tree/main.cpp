// ------------------------------------------------------------------------------------------------
/*  Algorithms and Data Structures - Project part #2
**  Computer Engineering and Informatics Department, University of Patras
**  Spring 2011 - Kyriakos Ispoglou
**
**
**  main.cpp
**
**  The testbench of the tree.
*/
// ------------------------------------------------------------------------------------------------
#pragma hdrstop
#include "my24tree.h"

#include <iostream>
#include <fstream>
#include <cstdlib>

#define MAX_N 32767

using namespace std;

mytree *Tree;                               /* our tree */

int array[ MAX_N ];                         /* an auxilary array */
// ------------------------------------------------------------------------------------------------
/*
**  EXERCISE 1:
**
**  This function calculates the height of a 2-4 tree with N objects. Function inserts the objects, 
**  calculates the height and then, deletes the objects.
**
**  Return Value: The height of the tree. -1 if an error occurs.
*/
int CalcHeight( int N )
{
    int i,height;

    Tree = new mytree();                    /* construct tree */

    if( N > MAX_N || N < 1 ) {
        /* to N einai ektos oriwn */
        return( -1 );
    }

    /* create and insert random objects */
    for( i=0; i<N; i++ ) {
        array[ i ] = rand() % 32768;
        Tree->Insert( array[i] );
    }

    height = Tree->GetHeight();             /* calc height */

    /* delete objects */
    for( i=0; i<N; i++ ) Tree->Delete( array[i] );

    Tree->~mytree();                        /* destruct tree */

    return height;
}

// ------------------------------------------------------------------------------------------------
/*
**  EXERCISE 2:
**
**  This function calculates the total number of splits, shares and fuses, of M random objects 
**  that randomly insert or delete in a non-empty tree.
**
**  Return Value: The total number of splits, shares and fuses.
*/
int TotalActs( int M )
{
    /* create random objects */
    for( int i=0; i<M; i++ ) array[ i ] = rand() % 32768;

    Tree->TotalActs = 0;                    /* clear total acts */
    for( int i=0; i<M; i++ )
    {
        if( rand() % 2  == 0 ) {
            /* insert an object */
            Tree->Insert( rand() % 32768 );
        } else {
            /* delete an object */
            Tree->Delete( rand() % 32768 );
        }
    }

    return Tree->TotalActs;                 /* return */
}

// ------------------------------------------------------------------------------------------------
/*
**  EXERCISE 3:
**
**  This function searches for N random objects in the tree.
**
**  Return Value: the number of objects found.
*/
int Search( int L )
{
    int found = 0;

    for( int l=0; l<L; l++ )
    if( Tree->Search( rand() % 32768 ) ) {
        /* object found */
        found++;
    }

    return found;
}

// ------------------------------------------------------------------------------------------------
#pragma argused                             /* main args won't use */
int main( int argc, char* argv[])
{
    int i;                                  /* iterator */

    randomize();                            /* initialize rand function */

    ofstream out1 ( "results2.txt" );       /* output file */

    /*
    **  Exercise 1
    */
    out1 << endl << endl << "INSERT:" << endl << endl;

    for( i=10; i<MAX_N; )
    {
        out1 << "Insert " << i << " objects. Height is "
             << CalcHeight( i ) << endl;

        i += (i < 500) ? 100 : 1000;
    }

    /*
    **  Exercise 2
    */
    out1 << endl << endl << "INS DEL:" << endl << endl;

    Tree = new mytree();
    for( i=0; i<8192; i++ ) Tree->Insert( rand() % 32768 );

    for( i=10; i<MAX_N; i+=1000 )
        out1 << i << " objects. Total acts: " << TotalActs( i ) << endl;

    /*
    **  Exercise 3
    */
    out1 << endl << endl << "SEARCH:" << endl << endl;

    for( i=10; i<MAX_N; i+=1000 )
        out1 << i << " objects. Objects found: " << Search( i ) << endl;

    out1.close();   

    system( "pause" );

    return 0;
}
// ------------------------------------------------------------------------------------------------
