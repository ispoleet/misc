// ------------------------------------------------------------------------------------------------
/*  Algorithms and Data Structures - Project part #1
**  Computer Engineering and Informatics Department, University of Patras
**  Spring 2011 - Kyriakos Ispoglou
**
**
**  main.cpp:
**
**  Benchmark for heap implementationm
*/
// ------------------------------------------------------------------------------------------------
#pragma hdrstop
#include <iostream>
#include <stdlib.h>
#include <time.h>

#include "myheap.h"

using namespace std;

long int a[ 10      ],na = 10;
long int b[ 100     ],nb = 100;
long int c[ 1000    ],nc = 1000;
long int d[ 10000   ],nd = 10000;
long int e[ 100000  ],ne = 100000;
long int f[ 1000000 ],nf = 1000000;
// ------------------------------------------------------------------------------------------------
int main( void )
{
    time_t start, stop;
    long int i;

    randomize();
    for( i=0; i<10;      i++ ) a[ i ] = rand() % 10000;
    for( i=0; i<100;     i++ ) b[ i ] = rand() % 10000;
    for( i=0; i<1000;    i++ ) c[ i ] = rand() % 10000;
    for( i=0; i<10000;   i++ ) d[ i ] = rand() % 10000;
    for( i=0; i<100000;  i++ ) e[ i ] = rand() % 10000;
    for( i=0; i<1000000; i++ ) f[ i ] = rand() % 10000;

    Heap <long int> H( 1000000 );


    H.Sort(INCREASE_ORDER, a, na);
    H.Sort(DECREASE_ORDER, a, na);
    start = clock();
    for( i=0; i<1000000; i++ ) H.Sort(0, a, na);
    stop = clock();
    cout << "10      elements: Time = " << ((stop-start)/CLK_TCK)/1000000
         << " sec" << endl;

    start = clock();
/*    for( i=0; i<100000; i++ ) H.Sort(b, nb);
    stop = clock();
    cout << "100     elements: Time = " << ((stop-start)/CLK_TCK)/100000
         << " sec" << endl;

    start = clock();
    for( i=0; i<10000; i++ ) H.Sort(c, nc);
    stop = clock();
    cout << "1000    elements: Time = " << ((stop-start)/CLK_TCK)/10000
         << " sec" << endl;

    start = clock();
    for( i=0; i<1000; i++ ) H.Sort(d, nd);
    stop = clock();
    cout << "10000   elements: Time = " << ((stop-start)/CLK_TCK)/1000
         << " sec" << endl;

    start = clock();
    for( i=0; i<100; i++ ) H.Sort(e, ne);
    stop = clock();
    cout << "100000  elements: Time = " << ((stop-start)/CLK_TCK)/100
         << " sec" << endl;

    start = clock();
    H.Sort(f, nf);
    stop = clock();
    cout << "1000000 elements: Time = " << ((stop-start)/CLK_TCK)
         << " sec" << endl;
         */
    system( "pause" );

    return 0;
}
// ------------------------------------------------------------------------------------------------


