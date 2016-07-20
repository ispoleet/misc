//-------------------------------------------------------------------------------------------------
/*                                                                                               **
**          Computer Engineering & Informatics Department, University of Patras, Greece          **
**                                                                                               **
**                             Algorithm Engineering - 8th semester                              **
**                                  Final Project - June 2013                                    **
**                                                                                               **
**                                     Kyriakos Ispoglou                                         **
**  main.cpp: Version 1.0                                                                        **
**                                                                                               */
//-------------------------------------------------------------------------------------------------
#include <LEDA/graph/graph.h>
#include <LEDA/graph/max_flow.h>
#include <LEDA/graph/min_cut.h>
#include <LEDA/core/list.h>
#include <LEDA/core/queue.h>
#include <LEDA/system/timer.h>

#include <iostream>
#include <cstdlib>

using namespace std;
using namespace leda;

#define MAX 1000

#define MINV(a, b) (a < b ? a : b)
#define MAXV(a, b) (a > b ? a : b)

//#define _DEBUG_

int min_cut_leda[MAX][MAX];
int min_cut_amo [MAX][MAX];
int m[MAX][MAX];

node_array<int>     cut_set[MAX];   // array of node arrays to store all N-1 cut sets


#include "min_cut.h"                // include my headers
#include "crt_graph.h"

// -------------------------------------------------------------------------------------------------
/*
**  all_min_cuts_leda(): This function find all minimum cut pairs in a graph, using n(n-1)/2
**      s-t LEDA's maximum flow calls. Complexity is O(N^3), for each MAX_FLOW. Thus the
**      total complexity of the function will be O(N^5).
**
**  Return Value: None.
*/
int all_min_cuts_leda
(
    const graph           &G,               // our graph
    const edge_array<int> capacity          // capacity of graph edges
)
{
    edge_array<int>     flow(G);            // edge array to store max-flows
    node_map<int>       N(G, 0);            // node map N for G. assign 0 to all nodes
    node                s,t,n;              // source, sink & auxilary node
    int j = 0;                              // auxilary iterator


    forall_nodes(n, G) { N[n] = j++; }      // assign an unique integer to each node

    /* for all pairs of nodes. Calculate only the upper trianglular matrix */
    for( s=G.first_node(); s!=nil; s=G.succ_node(s) )
    {
        for( t=G.succ_node(s); t!=nil; t=G.succ_node(t) )
        {
            // calculate minimum cut value for source-sink pair.
            min_cut_leda[ N[s] ][ N[t] ] = MAX_FLOW(G, s, t, capacity, flow);

#ifdef _DEBUG_
            // verify my minimum_cut.
            // list <node> A, B;
            node_array <bool> node_set(G);
            if(min_cut_leda[ N[s] ][ N[t] ] != minimum_cut(G, s, t, capacity, /*A, B,*/ node_set))
            {
                cerr << "\nminimum_cut() does not work correct... :(\n";
                return -1;
            }
#endif
        }
    }

    return 0;
}

// -------------------------------------------------------------------------------------------------
/*
**  all_min_cuts_amo(): This function find all minimum cut pairs in a graph, using only (n-1)
**       maximum flow calls. The total complexity of the function will be O(N^4).
**
**  Return Value: None.
*/
void all_min_cuts_amo
(
    const graph           &G,                   // our graph
    const edge_array<int> capacity              // capacity of graph edges
)
{
    const int num_nodes = G.number_of_nodes();  // number of nodes in G;
    node_array<bool>             node_set(G);   // node array to store  min cut sets

    node g[ num_nodes ],                        // map integers to nodes in G
         t[ num_nodes ],                        // map integers to nodes in T
          p, k, n;                              // auxilary node variables
    edge e;                                     // auxilary edge variables

    int i, j=0, l, cut_val, ec=0;               // auxilary integers, ec = edge counter

    graph T(num_nodes, num_nodes-1 );           // seperator tree with N nodes & 2*N-2 edges

    node_map<int>       Nt(T, 0);               // node map for T. assign 0 to all nodes
    edge_map<int>       N(T, 0);                // edge map N for T. assign 0 to all nodes
    edge_map<int>       min_cut(T, 0);          // edge map N for T. Map


    /*
    **  Calculate Seperator Tree T. Complexity: O(N^3) * (n-1)
    */
    forall_nodes(n, G)                          // create a new node in T and map with equivalent node in G
    {
        t[ j ] = T.new_node();                  // create a new node in T
        g[ j++ ] = n;                           // link node in G with a number
    }

    j = 0;
    forall_nodes(n, T) { Nt[n] = j++; }         // assign an unique integer to each node



    for( j=1; j<num_nodes; j++ )
    {
        locate( T, min_cut, N, g[j], l, j );    // calculate T with j+1 nodes

#ifdef _DEBUG_
        cout << "node k is: "; T.print_node(g[l]); cout << endl;     // display requested node k
#endif

        p = g[j];                               // p is the next node to add
        k = g[l];                               // k is the node in T to be connected with p

        /*
        **  we found k. Find p-k min cut and its node sets. Complexity O(N^3)
        */
        cut_val = minimum_cut(G, p, k, capacity, node_set);     // find p-k min cut.

        p = t[j];                               // get node k in seperator tree
        k = t[l];                               // get node p in seperator tree

        e = T.new_edge( p, k );                 // connect new node with other nodes in T

        cut_set[ec].init(G);                    // initialize node array for G
        forall_nodes(n, G)                      // copy node array
            cut_set[ec][n] = node_set[n];

        N[ e ] = ec++;                          // map edge with index in cut set
        min_cut[ e ] = cut_val;                 // edge weight is minimum cut
    }


    /*
    **  Seperator tree Calculated. Find all p-k min cut values by traversing from p
    **  to k in seperator tree. Min cut is the value of minimum edge in the path.
    **  Use a modified Schulze method, to calc all pair widest paths.
    **  Complexity: O(N^3)
    */
#ifdef _DEBUG_
        cout << "Finding all pair min cuts...\n\n";
#endif

    for( i=0; i<num_nodes; i++ )                // set all elements to 0
    for( j=0; j<num_nodes; j++ )
        min_cut_amo[i][j] = 0;


    forall_edges(e, T)                          // assign min cut values
    {
        min_cut_amo[ Nt[T.source(e)] ][ Nt[T.target(e)] ] = min_cut[e];
        min_cut_amo[ Nt[T.target(e)] ][ Nt[T.source(e)] ] = min_cut[e];
    }

    for( i=0; i<num_nodes; i++ )                // for all pairs of nodes in T
    for( j=0; j<num_nodes; j++ )
    {
        if( i == j ) continue;                  // capacity with source=sink not exits

        for( l=0; l<num_nodes; l++ )
        {
            if( i == l || j == l ) continue;    // the same, too

            min_cut_amo[j][l] = MAXV
                                (
                                    min_cut_amo[j][l],
                                    MINV(min_cut_amo[j][i], min_cut_amo[i][l])
                                );
        }
    }

#ifdef _DEBUG_
    forall_edges(e, T)
    {
        T.print_edge(e);
        cout << " cut: " << min_cut[e] << ". Set: " ;

        forall_nodes(n, G) cout << cut_set[ N[e] ][n] << " ";
        cout << "\n";
    }
    cout << "\n\n";
#endif
}

// -------------------------------------------------------------------------------------------------
/*
** main(): Main function...
*/
int main( int argc, char *argv[] )
{
    graph               G;                          // our graph
    edge_array<int>     capacity(G);                // edge array with caoacities

    float               tt1, tt2;                   // doubles to store times
    timer               t1, t2;                     // leda timers
    int                 gtype=0;                    // the graph type

    /*
    **  read arguments, & construct requested graph
    */
    if( argc < 2 )
    {
        cout << "\nusage: ./all_pair_min_cut -<1, 2, 3>  <number of nodes> <number of edges (optional)\n";
        return 0;
    }

         if( !strcmp( argv[1], "-1" ) ) gtype = 1;      // extract node type
    else if( !strcmp( argv[1], "-2" ) ) gtype = 2;
    else if( !strcmp( argv[1], "-3" ) ) gtype = 3;


    // create_small_graph( G, capacity );
    switch( gtype )
    {
        case 1: // type 1 graphs
            create_random_graph( G, capacity, atoi( argv[2] ), atoi( argv[3] ) );
            break;
        case 2: // type 2 graphs
            create_complete_graph( G, capacity, atoi( argv[2] ) );
            break;
        case 3: // type 3 graphs
            create_synthetic_graph( G, capacity, atoi( argv[2] ) );
            break;
        default:
            cout << "\nUnknown graph type!\n";
            return 0;
    }


    /*
    **  evaluate methods
    */
    t1.start();                                         // start t1
    all_min_cuts_leda(G, capacity);                     // execute brute force method
    t1.stop();                                          // stop t1
    tt1 = t1.elapsed_time();                            // find average time

    t2.start();                                         // start t1
    all_min_cuts_amo (G, capacity);                     // execute AMO method
    t2.stop();                                          // stop t1
    tt2 = t2.elapsed_time();                            // find average time



#ifdef _DEBUG_
    /*
    **  display min cut tables for LEDA & AMO
    */
    cout << "all min cuts for LEDA...\n";
    for( int i=0; i<G.number_of_nodes(); i++ )
    {
        for( int j=0; j<G.number_of_nodes(); j++ )
            cout << min_cut_leda[i][j] << "\t";

        cout << endl;
    }

    cout << "\nall min cuts for AMO...\n";
    for( int i=0; i<G.number_of_nodes(); i++ )
    {
        for( int j=0; j<G.number_of_nodes(); j++ )
            cout << min_cut_amo[i][j] << "\t";

        cout << endl;
    }
    cout << "\n\n";
#endif


    /*
    **  Check if these tables have the same elements.
    **  Check only the upper triangular matrix.
    */
    for( int i=0;   i<G.number_of_nodes(); i++ )
    for( int j=i+1; j<G.number_of_nodes(); j++ )
        if( min_cut_leda[i][j] != min_cut_amo[i][j] )
        {
            cout << "methods don't match :(\n";
            return -1;
            //break;
        }

    cout << "Methods match :)\n";


/*
    int u = minimum_cut(G, s, t, capacity, A, B);

    cout << "Minimum cut value for s-t="; G.print_node(s);
    cout << "-";
    G.print_node(t);
    cout << ", is " << u << endl;

    cout << "Source node set: ";
    forall(n, A) { G.print_node(n); cout << " "; }

    cout << "\nSink   node set: ";
    forall(n, B) { G.print_node(n); cout << " "; }
*/

    cout << "---------- Graph information ----------\n";
    cout << "Nodes = " << G.number_of_nodes() << "\nEdges = " << G.number_of_edges()/2 << endl;
    cout << "---------- Timing comparison ----------\n";
    cout << "LEDA: " << tt1 << endl
         << "AMO : " << tt2 << endl;

    cout << "Program exited...\n\n";

    return 0;
}
// -------------------------------------------------------------------------------------------------

