// ------------------------------------------------------------------------------------------------
/*  Algorithms and Data Structures - Project part #2
**  Computer Engineering and Informatics Department, University of Patras
**  Spring 2011 - Kyriakos Ispoglou
**
**
**  my24tree.h: 
**
**  This library creates and manages an 2-4 (also called 2-3-4) tree. The tree can search data 
**  in O( logn ), inserts data in O( logn ) and deletes data in O( logn ). This data stucture is 
**  based on Pointer Machine, which means the class uses memory dynamic, which means data are 
**  stored into HEAP SEGMENT.
*/
// ------------------------------------------------------------------------------------------------
#ifndef MY24TREE_H
#define MY24TREE_H

#include <iostream.h>

using namespace std;

#ifndef NULL
    #define NULL 0
#endif

#define _A 0                                // 1 child    -> 0 keys .down child bound
#define _B 4                                // 5 children -> 4 keys .upper child bound
// ------------------------------------------------------------------------------------------------

/* contains all information for a node */
class __Node {
    public:
        bool leaf;                          /* leaf=1 -> node if leaf   */
        int nkey;                           /* number of keys           */
        int key[5];                         /* the keys                 */
        __Node *child[5];                   /* the children             */
        __Node *parent;                     /* the parent               */
};

/* main class of 2-4 tree */
class mytree {
    private:
        __Node *Root;                       /* the root of our tree */

        /*
        ** Auxilary functions
        */

        /* search for a value in the tree and return it's parent */
        __Node *internal_search( int, bool& );

        /* insert root in the tree */
        void root_insert( __Node*&, __Node*&, int );

        void split( __Node* );              /* split node into 2 nodes */
        void share( __Node* );              /* share a child for brother's node */
        void fuse ( __Node* );              /* fuse 2 nodes */
    public:
        mytree() { Root = NULL; };          /* constructor */
        ~mytree() { ; }                     /* destructor  */

        bool Search( int );                 /* Search for a value into tree */
        bool Insert( int );                 /* Insert a value into tree */
        bool Delete( int );                 /* Delete a value into tree */

        int GetHeight( void );              /* returns tree height */

        int TotalActs;        
};

// ------------------------------------------------------------------------------------------------
/*
**  Search: search for a leaf in the tree.
**
**  Return Value: True if the leaf is found; Otherwise, return false.
*/
bool mytree::Search( int Value )
{
    bool found;

    internal_search( Value, found );

    return found;
}

// ------------------------------------------------------------------------------------------------
/*
**  internal_search: this function search for a key. if key does not exist function returns the 
**  parent node which should was the supposed key, and clears the found flag. Otherwise, function
**  returns the parent node of the leaf with value equal to key, and returns the parent node.
*/
__Node *mytree::internal_search( int Key, bool &found )
{
    __Node *curr = Root;                    /* start from root */

    if( !curr )
    {
        /* there are no nodes! Nothing eas found! */
        found = false;
        return NULL;
    }

    while( !curr->leaf )
    {
        bool getout = false;                /* auxilary flag */
        int i;                              /* auxilary variable */

        if( curr->nkey <= _A || curr->nkey >= _B )
        {
            /*
            **  If program goes here, something was going wrong...
            */
            cerr << "ERROR! INVALID NUMBER OF CHILDREN!" << endl;
            system( "pause" );
        }

        for( i=0; i<curr->nkey; i++ )
        if( curr->key[ i ] >= Key )
        {
            curr = curr->child[ i ];        /* go 1 level deeper */
            getout = true;
            break;
        }

        if( !getout ) {
            /* get rightmost child */
            curr = curr->child[ i ];
        }
    }

    if( curr->key[0] == Key ) found = true;
    else found = false;

    return( curr->parent == NULL ? curr : curr->parent );
}

// ------------------------------------------------------------------------------------------------
/*
**  split: this function takes an overflowed node and split it into 2 nodes. The parent node, will
**   have 1 more child. If the parent node overflows, function recursive split parent node.
**
**  Return Value: None.
*/
void mytree::split( __Node *curr )
{
    int i, j, Value;
    __Node *parent;

    /*
    ** Create a new node a place the 2 rightmost children of overflowed node.
    */
    __Node *newnode = new __Node;           /* allocate memory      */
    newnode->leaf = false;                  /* node isn't a leaf    */
    newnode->nkey = 1;                      /* node has 1 key       */

    /* node's key is the lastone key from overflowed node */
    newnode->key[0]=curr->key[3];

    newnode->child[0]=curr->child[3];       /* set node's children */
    newnode->child[1]=curr->child[4];
    newnode->child[0]->parent = newnode;    /* set children's parent */
    newnode->child[1]->parent = newnode;

    Value = curr->key[ 2 ];                 /* get key for parent */

    /* remove 2 rightmost children of node curr */
    curr->nkey = 2;
    curr->key[2] = 0; curr->child[3] = NULL;
    curr->key[3] = 0; curr->child[4] = NULL;

    if( curr->parent == NULL )
    {
        /* there is no parent. create one */
        root_insert( curr, newnode, Value );

        parent = Root;                      /* get parent */
    }
    else
    {
        parent = curr->parent;              /* get curr's parent */

        /* find child index in parent node */
        for( j=0; parent->child[j]!=curr; j++ );

        /* shift children to add the new */
        for( i=parent->nkey; i>j; i-- )
            parent->child[i + 1] = parent->child[i];

        curr->parent->child[ j + 1 ] = newnode;     /* insert new node  */
        newnode->parent = parent;                   /* set parent       */

        /* shift keys to add the new */
        for( i=parent->nkey; i>j; i-- )
            parent->key[i] = parent->key[i-1];

        parent->key[ j ] = Value;           /* insert new key */
        parent->nkey++;                     /* increase index counter */

        if( parent->nkey == _B )
        {
            /*
            **  Parent node has more than _B children. Overflow. Split parent recursive...
            */
            split( parent );
        }
    }

    TotalActs++;
}

// ------------------------------------------------------------------------------------------------
/*
**  root_insert: this function takes 2 nodes and a key, and creates a root node with 2 children: 
**  a and b. The key of the new node is equal to key parameters.
**
**  Return Value: None.
*/
void mytree::root_insert( __Node* &a, __Node* &b, int key )
{
    __Node *root = new __Node;              /* allocate memory for root */
    root->leaf = false;                     /* root is not leaf         */
    root->nkey = 1;                         /* root has 1 key           */
    root->parent = NULL;                    /* root has no parent       */
    root->key[ 0 ] = key;                   /* set key value            */
    root->child[ 0 ] = a;                   /* left  child is a         */
    root->child[ 1 ] = b;                   /* right child is b         */

    a->parent = root;                       /* a's parent is root       */
    b->parent = root;                       /* b's parent is root       */
    Root      = root;                       /* Parent, node is root     */
}

// ------------------------------------------------------------------------------------------------
/*
**  Insert: this function adds a value into tree. insert, finds and places the value in the correct
**  position. If a node overflow occurs, insert makes editing moves to restore balance into tree.
**
**  Return Value: If value already exist, function returns false; Otherwise return true.
*/
bool mytree::Insert( int Value )
{
    __Node *curr, *newnode;

    /* Create the new leaf */
    newnode = new __Node;                   /* Allocate Memory              */
    newnode->leaf  = true;                  /* newnode is leaf              */
    newnode->key[0]= Value;                 /* store Value                  */
    newnode->nkey  = 0;                     /* unused variable              */
    newnode->parent= NULL;                  /* newnode hasn't parent yet    */

    if( Root == NULL )
    {
        /* There is not parent. Root is Leaf! */
        Root = newnode;

        return true;                        /* insertion successful! */
    }
    else if( Root->leaf )
    {
        /* Root is leaf. Split root */

        if( Root->key[0] < newnode->key[0] )
        {
            /* Root is left, newnode is right */
            root_insert( Root, newnode, Root->key[0] );
        }
        else if( Root->key[0] > newnode->key[0] )
        {
            /* newnode is left, Root is right */
            root_insert( newnode, Root, newnode->key[0] );
        }
        else return false; /* insertion fail! */
    }
    else
    {
        /* Our tree isn't empty. Find correct node for our leaf. */
        bool found;                         /* auxilary variable */
        int i;                              /* auxilary variable */

        curr = internal_search( Value, found ); /* search for value */
        if( found == true )
        {
            /* Value already exist */
            return false;
        }
        newnode->parent = curr;

        /* place child in correct position */
        for( i=curr->nkey++;
             (i>=0)&&(curr->child[i]->key[0] > Value); i-- )
        {
            curr->child[i + 1] = curr->child[i];
        }

        curr->child[i + 1] = newnode;       /* insert child */

        /* update keys */
        for( i=0; i<curr->nkey; i++ )
            curr->key[i] = curr->child[i]->key[0];

        if( curr->nkey == _B )
        {
            /* node overflow. split children */
            split( curr );
        }
    }

    return true; /* insertion success! */
}

// ------------------------------------------------------------------------------------------------
/*
**  Delete: this function deletes a leaf. If an underflowes node occurs, function repairs tree 
**  balancing.
**
**  Return Value: True on success; False if key does not exist.
*/
// ------------------------------------------------------------------------------------------------
bool mytree::Delete( int Value )
{
    __Node *curr;
    bool found;                             /* auxilary flag */
    int i, j;                               /* auxilary counters */

    curr = internal_search( Value, found ); /* search for Value */
    if( found == false )
    {
        /* Value does not exist */
        return false;
    }

    /* find leaf's order */
    for( j=0; j<curr->nkey; j++ )
        if( curr->child[j]->key[0] == Value ) break;

    delete curr->child[j];                  /* release memory */
    curr->child[j] = NULL;                  /* clear pointer  */

    /* shift children to fill the gap */
    for( i=j; i<curr->nkey; i++ )
        curr->child[i] = curr->child[i+1];

    curr->child[i] = NULL;                  /* clear last child */
    curr->nkey--;                           /* decrease key counter */

    /* update keys */
    for( i=0; i<curr->nkey; i++ )
        curr->key[i] = curr->child[i]->key[0];
    curr->key[ curr->nkey ] = 0;            /* clear last key */

    if( curr->nkey == _A )
    {
        /*
        **  Underflowed node. Try to share.
        */

        if( curr->parent != NULL ) share( curr->parent );
        else
        {
            if( curr->leaf )
            {
                delete Root;                        /* release memory */
                Root = NULL;                        /* clear root */
            }
            else {
                delete Root;                        /* release memory */
                Root = NULL;                        /* clear root */

                Root = curr->child[ 0 ];            /* set new root */
                curr->child[ 0 ]->parent = NULL;    /* set root's parent */
            }
        }
    }

    return true;                            /* deletion success! */
}

// ------------------------------------------------------------------------------------------------
/*
**  share: this function tries to share a child for brother's underflowed node. If both of node's 
**  brothers have a (=2) children, share fails.
**
**  Return Value: None.
*/
void mytree::share( __Node *curr )
{
    int i, j;                               /* auxilary variables */

    /* find underflowed child's index in parent node */
    for( j=0; j<=curr->nkey; j++ )
        if( curr->child[j]->nkey == _A )
            break;

    if( j>0 && curr->child[j - 1]->nkey > 1 )
    {
        /*
        **  share a child from left node
        */

        /* get righmost key index from left brother */
        int lchd = curr->child[j-1]->nkey--;

        /* shift right the unique child for underflowed node */
        curr->child[j]->child[1] = curr->child[j]->child[0];

        /* share the rightmost child of left node*/
        curr->child[j]->child[0] = curr->child[j - 1]->child[lchd];
        curr->child[j - 1]->child[lchd] = NULL; /* clear key */

        /* set parent's shared child */
        curr->child[j]->child[0]->parent = curr->child[j];

        /* update keys */
        curr->child[j]->nkey = 1;
        curr->child[j]->key[0] = curr->key[j - 1]; /* get key from parent */
        curr->key[j - 1] =
                curr->child[j - 1]->key[lchd - 1]; /* send key to parent */
        curr->child[j - 1]->key[lchd - 1] = 0; /* clear key */
    }
    else if( j<curr->nkey && curr->child[j + 1]->nkey > 1 )
    {
        /*
        **  share a child from right node
        */

        /* get the leftmost child of right brother and set it's parent */
        curr->child[j]->child[1] = curr->child[j + 1]->child[0];
        curr->child[j]->child[1]->parent = curr->child[j];

        /* update keys */
        curr->child[j + 1]->nkey--;
        curr->child[j]->nkey = 1;
        curr->child[j]->key[0] = curr->key[j]; /* get key from parent */
        curr->key[j] =
                 curr->child[j + 1]->key[ 0 ]; /* send key to parent */

        /* shift right brother's children to fill the gap */
        for( i=0; i<=curr->child[j + 1]->nkey; i++ )
            curr->child[j + 1]->child[i] =
                    curr->child[j + 1]->child[i + 1];

            curr->child[j + 1]->child[i] = NULL; /* clear key */

        /* update right brother's keys */
        for( i=0; i<curr->child[j + 1]->nkey; i++ )
            curr->child[j + 1]->key[i] =
                    curr->child[j + 1]->key[i + 1];

        curr->child[j + 1]->key[i] = 0; /* clear key */
    }
    else fuse( curr );

    TotalActs++;
}

// ------------------------------------------------------------------------------------------------
/*
**  fuse: this function fuses the underflowed node with one of it's brothers with A nodes. If the 
**  parent node is underflowed after fusion, function makes fusion to parent node.
**
**  Return Value: None.
*/
void mytree::fuse( __Node* curr )
{
    int i, j;                               /* auxilary variables */

    /* find underflowed child's index in parent node */
    for( j=0; j<=curr->nkey; j++ )
        if( curr->child[j]->nkey == _A )
            break;

    if( j > 0 )                             /* left brother exists */
    {
        /*get last child of left brother */
        int lchd = ++curr->child[j - 1]->nkey;

        /* move node's one child into last index */
        curr->child[j - 1]->child[lchd] = curr->child[j]->child[0];
        curr->child[j - 1]->key[lchd-1] = curr->key[j - 1]; /* get key */

        curr->child[j - 1]->child[lchd]->parent = curr->child[j - 1];

        delete curr->child[ j ];            /* release memory */
        curr->child[ j ] = NULL;

        for( i=j; i<curr->nkey; i++ )       /* shift children */
            curr->child[i] = curr->child[i+1];
        curr->child[ curr->nkey-- ] = NULL;  /* clear child */

        for( i=j; i<curr->nkey; i++ )       /* shift keys */
            curr->key[i] = curr->key[i + 1];
        curr->key[curr->nkey] = 0;          /* clear key */
    }
    else if( j < curr->nkey )               /* right brother exists */
    {
        for( i=curr->child[j + 1]->nkey; i>=0; i-- ) /* shift children */
            curr->child[j + 1]->child[i + 1] =
                curr->child[j + 1]->child[i];

        /* move the lastone child from underflowed node */
        curr->child[j + 1]->child[0] = curr->child[j]->child[0];

        curr->child[j + 1]->child[0]->parent = curr->child[j + 1];

        delete curr->child[ j ];            /* release memory */

        /* shift keys */
        for( i=curr->child[j + 1]->nkey++-1; i>=0; i-- )
            curr->child[j + 1]->key[i + 1] =
                curr->child[j + 1]->key[i];
        curr->child[j + 1]->key[ 0 ] = curr->key[ j ]; /* set key */

        for( i=0; i<curr->nkey; i++ )       /* shift keys */
            curr->child[i] = curr->child[i + 1];
        curr->child[i] = NULL;              /* clear child */

        curr->nkey--;                       /* update parent keys */
        for( i=0; i<curr->nkey; i++ )
            curr->key[i] = curr->key[i + 1]; // shift

        curr->key[i] = 0;                   /* clear key */
    }

    if( curr->nkey == _A )
    {
        /*
        **  Parent node, is underflowed bacause of fusion. Fuse again.
        */
        if( curr == Root )
        {
            delete Root;                    /* release memory */
            Root = NULL;                    /* clear root */

            Root = curr->child[ 0 ];        /* replace root, from unique child */
            curr->child[ 0 ]->parent = NULL;/* set root's parent */
        }
        else share( curr->parent );
    }

    TotalActs++;
}

// ------------------------------------------------------------------------------------------------
/*
**  GetHeight: finds the height of the tree. All the leafs have the same length. Start from root.
**  If you find a leaf stop.
**
**  Return Value: The height of the tree.
*/
int mytree::GetHeight( void )
{
    __Node *curr = Root;                    /* start from root */
    int height = 1;                         /* clear counter */

    while( !curr->leaf )
    {
        /* current node is not a leaf */
        curr = curr->child[ 0 ];            /* get 1 level deeper */

        height++;                           /* increase height */
    }

    return height;
}

#endif
// ------------------------------------------------------------------------------------------------

