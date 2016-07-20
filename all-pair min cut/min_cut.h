//-------------------------------------------------------------------------------------------------
/*                                                                                               **
**          Computer Engineering & Informatics Department, University of Patras, Greece          **
**                                                                                               **
**                             Algorithm Engineering - 8th semester                              **
**                                  Final Project - June 2013                                    **
**                                                                                               **
**                                     Kyriakos Ispoglou                                         **
**                                                                                               **
**  min_cut.h: Version 1.0                                                                       **
**  This library contains some core functions for finding minimum cuts.                          **
**                                                                                               */
//-------------------------------------------------------------------------------------------------
#ifndef __MIN_CUT_H__
#define __MIN_CUT_H__

#include <LEDA/graph/graph.h>
#include <LEDA/graph/max_flow.h>
#include <LEDA/graph/min_cut.h>
#include <LEDA/core/list.h>
#include <LEDA/core/queue.h>


#define INF 99999999

using namespace std;
using namespace leda;

// -------------------------------------------------------------------------------------------------
/*
**  minimum_cut(): This function calculates the minimum cut between 2 nodes  in a given graph.
**      The minimum cut, splits nodes, in 2 sets. Source node s belongs to first set, and sink
**      node t belogns to second set.  The union of these sets is  the graph. In order to find
**      these sets, we work as follows:  First we  find the  maximum flow between these nodes.
**      However, maximum flow  is equal  with the minimum cut.  We can find the residual graph
**      from initial graph and edges flow  vector. Then we make a siple BFS in residual graph,
**      starting from node s. All marked nodes from s  belogs to the first set.  minimum_cut()
**      also verifies these sets as follows: Each edge in minimum cut, has its source in first
**      set, and its sink in the second. If we add the flows of theses edges, the summary must
**      be equal with the maximum flow.
**
**  Return Value: If verification process is successfull, function returns the minimum cut
**      value. Otherwise, function returns -1.
*/
int minimum_cut
(
    const graph           &G,               // our graph
    const node            s,                // source node
    const node            t,                // sink node
    const edge_array<int> capacity,         // capacity of graph edges
//  list<node>            &Sa,              // first  set of nodes
//  list<node>            &Sb,              // second set of nodes
    node_array<bool>      &node_set         // array for lookup node's set
)
{
    graph               Gr;                 // residual graph of G
    edge_array<int>     flow(G);            // edge array to store max-flows
    queue <node>        Q;                  // queue to store marked nodes
    node_array <node>   map(G);             // a map between nodes of G and Gr
    node                curr,               // current node in search
                        n;                  // auxilary node for 'forall' loops
    edge                e;                  // auxilary edge for 'forall' loops
    int                 min_cut=0,          // minimum cut value
                        ver_cut=0;          // cut value after verificaton


    if( s == t ) return 0;                  // source == sink ???

    // Use LEDA's MAX_FLOW(), in order to find maximum flow between nodes s and t.
    // On return, flow contains edges max-flows.
    min_cut = MAX_FLOW(G, s, t, capacity, flow);


    /*
    **  Crete the residual graph
    */

    // create a new node and map with equivalent node in initial graph.
    forall_nodes(n, G) map[n] = Gr.new_node();

    forall_edges(e, G)                      // create edges for residual graph
    {
        if( flow[e] > 0 )                   // if edge has flow, create reverse edge
            Gr.new_edge( map[G.target(e)], map[G.source(e)] );

        if( capacity[e] - flow[e] > 0 )     // if edge has residual, just copy edge
            Gr.new_edge( map[G.source(e)], map[G.target(e)] );
    }


    /*
    **  Make a BFS in order ot find minimum cut sets
    */
    node_array <bool>   mark(Gr);           // array to store which marked nodes

    forall_nodes(n, Gr) mark[n] = false;    // unmark all nodes

    mark[ map[s] ] = true;                  // mark initial (mapped) node
    Q.append( map[s] );                     // append initial node to queue


    while( !Q.empty() )                     // while there are elements in queue...
    {
        curr = Q.top();                     // get topmost node from queue
        Q.pop();                            // remove topmost node from queue

        forall_adj_edges(e, curr)           // for all adjacent nodes of current node
            if( !mark[ Gr.target(e) ] )     // is adjacent node marked ?
            {
                mark[Gr.target(e)] = true;  // node is not marked. Mark it

               Q.append( Gr.target(e) );    // append node into queue
            }
    }

//    Sa.clear();                             // clear 1st set in O(1)
//    Sb.clear();                             // clear 2nd set in O(1)
    forall_nodes(n, G)                        // Determine node sets from mark array
    {
        if(mark[ map[n] ] == true)  { /*Sa.append(n);*/ node_set[n] = true;  }
        else                        { /*Sb.append(n);*/ node_set[n] = false; }
    }

    /*
    **  verify that these sets are correct
    */
    forall_edges(e, G)                      // for all edges
    {
        if( mark[ map[G.source(e)]] == true &&
            mark[ map[G.target(e)]] == false )
        {
            // edge has its source in 1st set, and it's sink in 2nd set.This
            // means that is an edges of the minimum cut. So, add its flow.
            ver_cut += flow[e];
        }
    }

    if( ver_cut != min_cut )                // minimum cut values match?
    {
        cerr << "Min-Cut values don't match! MAX_FLOW() value: "
             << min_cut << ", My value: " << ver_cut << endl;

        return -1;                          // function failure
    }


    return min_cut;                         // function success!
}

// -------------------------------------------------------------------------------------------------
/*
**  locate(): This function determines which node in seperator tree T, will be connected with the
**      next node p. locate() finds the edges with min cost in T, and then split the tree in 2
**      sets, depending min cut set of this edge. From these sets, locate() rejects the set which
**      not contain new node p in its mimium cut. This process continues recursively, until only
**      1 node exists in T. This node will be the requested node k,
**
**  Return Value: None.
*/
void locate
(
    graph                   &T,                 // our seperator tree
    const edge_map<int>     min_cut,            // edge's min cut costs
    const edge_map<int>     N,                  // map to "map" edges with integers
    const node              p,                  // source node
    int                     &k,                 // sink node ID
    const int               active_nodes        // number of active nodes in T
)
{
    edge_array<bool>    mark_e(T);              // array to store marked edges
    node_array<bool>    mark_n(T);              // array to store marked nodes
    queue <node>        Q;                      // queue to store marked nodes
    node                curr,                   // current node in search
                        a, b, n;                // auxilary nodes
    edge                e, min_edge;            // edges
    int                 j=0, min_edge_c;        // auxilary iterators

#ifdef _DEBUG_
    cout << "------------------------- edges in T: " << T.number_of_edges() << "-------------------------\n";
#endif

    T.make_undirected();                        // first make T undirected to ensure functions's correctness

    if( T.number_of_edges() == 0 )              // if T has no edges, return the first node (0)
    {
        k = 0;                                  // k is the first node
        return;                                 // return from function
    }

    forall_nodes(n, T) mark_n[n] = false;       // unmark all nodes
    forall_edges(e, T) mark_e[e] = false;       // unmark all edges

    k = 0;                                      // clear k
    for( int c=0; c<active_nodes-1; )           // while T is not singleton
    {
        /*
        **  find edge with minimum cost, and nodes a & b
        */
        min_edge_c = INF;                       // assing a very high value
        a = nil; b = nil; min_edge = nil;       // initialize these variables, to prevent compiler warnings

        forall_edges( e, T )                    // for all edges
        {
            if( min_edge_c > min_cut[e] &&      // smaller cost than minimum &
                mark_e[e] == false )            // not marked edge
            {
                min_edge_c = min_cut[e];        // store min cost
                a = T.source(e);                // store source of edge
                b = T.target(e);                // store target of edge
                min_edge = e;                   // store min edge
            }
        }
        mark_e[min_edge] = true;                // mark minimum edge


        if( cut_set[ N[min_edge] ][p] == 0 ) {  // node p is in set with b.
            // So toggle nodes a and b.
            node tmp = a; a = b; b = tmp;
        }
        // else, node p is in set with a. Do nothing :P

#ifdef _DEBUG_
        cout << "min edge: ";   T.print_edge(min_edge);
        cout << "\t: a = ";     T.print_node(a);
        cout << ",  b = ";      T.print_node(b); cout << endl;
#endif

        /*
        **  Make a BFS in order ot find minimum cut sets
        */
        mark_n[ b ] = true;                     // mark initial (mapped) node
        Q.append( b );                          // append initial node to queue
        c++;                                    // increase marked nodes

        while( !Q.empty() )                     // while there are elements in queue...
        {
            curr = Q.top();                     // get topmost node from queue
            Q.pop();                            // remove topmost node from queue

            forall_adj_edges(e, curr)           // for all adjacent nodes of current node
            {
                node src = T.source(e),         // get source node of e
                     trg = T.target(e);         // get target node of e

                if( src != curr ) {             // source node is not target?
                    // Toggle source and target nodes.
                    node tmp = src; src = trg; trg = tmp;
                }

#ifdef _DEBUG_
                cout << "curr: ";
                T.print_node(curr); cout << "\t";
                T.print_edge(e);    cout << "\t";
                T.print_node(trg);  cout << "\t";
                T.print_node(a);    cout << "\n";
#endif

                if( !mark_n[trg] && trg != a )  // is adjacent node marked  & adjacent node different to a?
                {
                    mark_e[e]   = true;         // edge is not marked. Mark it.
                    mark_n[trg] = true;         // node is not marked. Mark it.
                    Q.append( trg );            // append node into queue
                    c++;                        // increase marked nodes
                }
            }
        }

#ifdef _DEBUG_
        forall_nodes(n, T)                      // print mark state of nodes
        {
            if( mark_n[n] == false ) cout << "0  ";
            else cout << "1  ";
        }
#endif
    }

    /*
    ** T is singleton. Find the last non-marked node.
    */
    j = 0;
    forall_nodes(n, T)
    {
        if( mark_n[n] == false ) {
            k = j;
            break;
        }
        j++;
    }

    // function returns
}
// -------------------------------------------------------------------------------------------------
#undef INF
#endif
