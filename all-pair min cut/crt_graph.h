//-------------------------------------------------------------------------------------------------
/*                                                                                               **
**          Computer Engineering & Informatics Department, University of Patras, Greece          **
**                                                                                               **
**                             Algorithm Engineering - 8th semester                              **
**                                  Final Project - June 2013                                    **
**                                                                                               **
**                                     Kyriakos Ispoglou                                         **
**  crt_graph.h: Version 1.0                                                                     **
**  This library contains functions for create graphs.                                           **
**                                                                                               */
//-------------------------------------------------------------------------------------------------
#ifndef __CRT_GRAPH__
#define __CRT_GRAPH__

#include <LEDA/graph/graph.h>
#include <cstdlib>

using namespace leda;


// -------------------------------------------------------------------------------------------------
/*
**  create_complete_graph(): This function creates a complete graph, with random capacities
**
**  Return Value: None.
*/
void create_complete_graph
(
    graph               &G,                     // our graph
    edge_array<int>     &capacity,              // capacity of graph edges
    const int           n                       // number of nodes in G
)
{

    edge                e, et;                  // auxilary edges
    node                nt;                     // auxilary node
    int                 j = 0;                  // auxilary iterator

    complete_graph(G, n);                       // create a complete graph

    node_map<int>       N(G, 0);                // node map N for G. assign 0 to all nodes
    forall_nodes(nt, G) { N[nt] = j++; }        // assign an unique integer to each node

    capacity.init(G);                           // initialize capacity vector

    /*
    ** If edge (a,b) has capacity C, then edge (b,a) must have the same
    ** capacity, if we want to have an undirected graph. Thus we assign
    ** a random number in edge (a,b) and for edge (b,a) we lookup in graph
    ** for edge's (a,b) capacity.
    */
    forall_edges(e, G)                              // for each edge
    {
        if( N[G.source(e)] > N[G.target(e)] )       // b > a ?
        {
            forall_adj_edges(et, G.target(e) )      // we found (b,a). Search for (a,b)
            {
                if( G.target(et) == G.source(e) )   // (a, b) found. Get its capacity.
                    capacity[e] = capacity[ et ];
            }
        }
        else if( N[G.source(e)] == N[G.target(e)] ) // loop edge? Assign 0 in its capacity
            capacity[e] = 0;
        else capacity[e] = rand() % 100000 + 1;     // (a,b) edge. Assing a random numbe
    }
}

// -------------------------------------------------------------------------------------------------
/*
**  create_complete_graph(): This function creates a synthetic graph with parameter n
**
**  Return Value: None.
*/
void create_synthetic_graph
(
    graph               &G,                     // our graph
    edge_array<int>     &capacity,              // capacity of graph edges
    const int           num_n                   // number of nodes in G
)
{
#define MAX_E 10000
    graph               Gs;                     // a small complete graph
    edge_array<int>     cs;                     // capacity of graph edges
    node                n, n1, n2, n3;          // auxilary nodes
    edge                e, e0, e1, e2, e3;      // auxilary edges
    edge                E[MAX_E];               // edge repository
    int                 C[MAX_E],               // capacity repository
                        c = 0;                  // counter

    n1 = G.new_node();                          // create a new node in G

    for( int i=0; i<num_n; i++ )                // for each subgraph
    {

        Gs.clear();                             // clear complete graph
        create_complete_graph(Gs, cs, num_n);   // create a new one

        node_map<node> map(Gs);                 // map nodes of Gs with them of G

        forall_nodes(n, Gs)
            map[n] = G.new_node();              // create new node in G

        n3 = G.new_node();                      // create anothe new node


        forall_nodes(n, Gs)                     // for each node in Gs
        {
            n2 = map[n];                        // get equivalent node in G

            e0 = G.new_edge(n1, n2);            // connect n1 & n3 with all nodes
            e1 = G.new_edge(n2, n1);            // in complete graph Gs
            e2 = G.new_edge(n2, n3);
            e3 = G.new_edge(n3, n2);

            E[ c ] = e0; C[ c++ ] = num_n*num_n;// store temporary edges & capacities
            E[ c ] = e1; C[ c++ ] = num_n*num_n;
            E[ c ] = e2; C[ c++ ] = num_n*num_n;
            E[ c ] = e3; C[ c++ ] = num_n*num_n;
        }


        forall_edges(e, Gs)                     // copy edges from Gs to G
        {
            e0 = G.new_edge(map[Gs.source(e)], map[Gs.target(e)]);
            E[ c ] = e0; C[c++] = cs[e];
        }

        n1 = n3;                                // last node, is the first in next iteration
    }


    capacity.init(G);                           // initialize edge capacity vector
    for( int i=0; i<c; i++ )
        capacity[ E[i] ] = C[i];                // set capacity

#undef MAX_E
}

// -------------------------------------------------------------------------------------------------
/*
**  create_random_graph(): This function creates a random graph.
**
**  Return Value: None.
*/
void create_random_graph
(
    graph               &G,                     // our graph
    edge_array<int>     &capacity,              // capacity of graph edges
    const int           n,                      // number of nodes in G
    const int           m                       // number of edges in G
)
{
    edge                e, e0, e1;              // auxilary edges
    int                 i, j, cap;              // auxilary edges

    // random_graph(G, n, m, false, true, true);
    node *N = new node[ n   ];                  // allocate memory for nodes
    int  *C = new int [ 2*m ];                  // allocate memory for capacities

    G.init(n,m);                                // allocate memory for nodes & edges in G

    for( i=0; i<n; i++ ) N[i] = G.new_node();   // create nodes
    for( j=0, i=0; i<m; i++ )                   // create edges
    {
        int a=0, b=0;

        do {                                    // generate 2 different random numbers
            a = rand() % n;
            b = rand() % n;
        } while( a == b );

        e0 = G.new_edge( N[a], N[b] );          // create edges
        e1 = G.new_edge( N[b], N[a] );

        cap = rand() % 10000 + 1;               // generate capacity in range [1, 10000]

        C[ j++ ] = cap;                         // set capacity for e0
        C[ j++ ] = cap;                         // set capacity for e1
    }

    j = 0;                                      //c lear j
    capacity.init(G);                           // allocate memory for edge array

    forall_edges(e, G) capacity[e] = C[j++];    // assign capacities to G

    delete [] N;                                // release memory
    delete [] C;
}

// -------------------------------------------------------------------------------------------------
/*
**  create_small_graph(): This function creates a small graph.
**
**  Return Value: None.
*/
void create_small_graph

(
    graph               &G,                 // our graph
    edge_array<int>     &capacity           // capacity of graph edges
)
{
    /* create nodes */
    node n0 = G.new_node();
    node n1 = G.new_node();
    node n2 = G.new_node();
    node n3 = G.new_node();
    node n4 = G.new_node();
    node n5 = G.new_node();
    node n6 = G.new_node();

    /* create edges */
    edge e0  = G.new_edge(n0, n1);
    edge e1  = G.new_edge(n1, n0);
    edge e2  = G.new_edge(n0, n2);
    edge e3  = G.new_edge(n2, n0);
    edge e4  = G.new_edge(n0, n6);
    edge e5  = G.new_edge(n6, n0);
    edge e6  = G.new_edge(n1, n2);
    edge e7  = G.new_edge(n2, n1);
    edge e8  = G.new_edge(n3, n5);
    edge e9  = G.new_edge(n5, n3);
    edge e10 = G.new_edge(n4, n5);
    edge e11 = G.new_edge(n5, n4);
    edge e12 = G.new_edge(n2, n6);
    edge e13 = G.new_edge(n6, n2);
    edge e14 = G.new_edge(n0, n4);
    edge e15 = G.new_edge(n4, n0);
    edge e16 = G.new_edge(n5, n6);
    edge e17 = G.new_edge(n6, n5);

/*
    edge e0=G.new_edge(n0,n1);
    edge e1=G.new_edge(n0,n2);
    edge e2=G.new_edge(n0,n3);
    edge e3=G.new_edge(n1,n3);
    edge e4=G.new_edge(n1,n4);
    edge e5=G.new_edge(n2,n5);
    edge e6=G.new_edge(n3,n2);
    edge e7=G.new_edge(n3,n4);
    edge e8=G.new_edge(n3,n5);
    edge e9=G.new_edge(n4,n1);
    edge e10=G.new_edge(n4,n5);
    edge e11=G.new_edge(n4,n6);
    edge e12=G.new_edge(n5,n4);
    edge e13=G.new_edge(n5,n6);
    edge e14=G.new_edge(n6,n5);
*/

    /* create capacities */
    capacity.init(G);

    capacity[ e0  ] = 5;
    capacity[ e1  ] = 5;
    capacity[ e2  ] = 8;
    capacity[ e3  ] = 8;
    capacity[ e4  ] = 3;
    capacity[ e5  ] = 3;
    capacity[ e6  ] = 2;
    capacity[ e7  ] = 2;
    capacity[ e8  ] = 6;
    capacity[ e9  ] = 6;
    capacity[ e10 ] = 10;
    capacity[ e11 ] = 10;
    capacity[ e12 ] = 4;
    capacity[ e13 ] = 4;
    capacity[ e14 ] = 7;
    capacity[ e15 ] = 7;
    capacity[ e16 ] = 2;
    capacity[ e17 ] = 2;

/*
    capacity[ e0  ] = 5;
    capacity[ e1  ] = 4;
    capacity[ e2  ] = 8;
    capacity[ e3  ] = 1;
    capacity[ e4  ] = 3;
    capacity[ e5  ] = 5;
    capacity[ e6  ] = 3;
    capacity[ e7  ] = 4;
    capacity[ e8  ] = 6;
    capacity[ e9  ] = 2;
    capacity[ e10 ] = 1;
    capacity[ e11 ] = 10;
    capacity[ e12 ] = 1;
    capacity[ e13 ] = 6;
    capacity[ e14 ] = 1;
*/

}
// -------------------------------------------------------------------------------------------------
#endif
