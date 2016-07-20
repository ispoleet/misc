// ------------------------------------------------------------------------------------------------
/*  Algorithms and Data Structures - Project part #1
**  Computer Engineering and Informatics Department, University of Patras
**  Spring 2011 - Kyriakos Ispoglou
**
**  NOTE: All comments below are in greeklish. Sorry about that :)
**
**
**  myheap.h:
**
**  Vivlio8hkh poy ylopoiei mia domh swroy. Periexei tis vasikes synarthseis diaxeirishs enos 
**  swroy, ka8ws epishs kai thn me8odo ta3inomhshs heapsort.
*/
// ------------------------------------------------------------------------------------------------
#ifndef MYHEAP_H
#define MYHEAP_H

#ifndef MY_DECLS

    #define MY_DECLS
    #define _INFINITY 65535

    typedef unsigned int _UINT;
#endif

const _UINT MAX_HEAP_SIZE = 65535;

enum SORT_ORDER {
    INCREASE_ORDER = 0,
    DECREASE_ORDER = 1
};
// ------------------------------------------------------------------------------------------------
/*
**  Ayth h klash dimiourgei kai diaxeirizetai enan swro
*/
template <class Type>
class Heap
{
    private:
        Type *_StoragePlace;                /* edw apo8ikeuetai o swros. */

        _UINT _MaximumSize;                 /* megisto pli8os stoixeiwn poy mporoun
                                            ** na apo8hkeytoyn ston swro. */
        _UINT _CurrentSize;                 /* trexon pli8os stoixeiwn. */

        void Heapify( Type*, _UINT, SORT_ORDER );   /* Epanafora ths idiotitas tou swrou.*/
    public:
        Heap( int max_size );               /* Constructor */
        ~Heap();                            /* Destructor  */

        /*
        **  Eyresh aristeroy paidiou se O(1).
        **  Leme _Index + 1 giati h ari8mhsh ksekinaei apo to 0.
        **
        */
        inline _UINT Left( _UINT _Index )
        {
            return( ((_Index+1) << 1) - 1 > _CurrentSize ? _INFINITY :
                    ((_Index+1) << 1) - 1 );
        }

        /* Eyresh deksiou paidiou se O(1). */
        inline _UINT Right( _UINT _Index )
        {
            return( (_Index+1) << 1 > _CurrentSize ? _INFINITY :
                    (_Index+1) << 1 );
        }

        /* Eyresh patera se O(1). */
        inline _UINT Parent( _UINT _Index )
        {
            return( ((_Index+1) >> 1) - 1 );
        }

        /* Elegxos an o swros einai adeios */
        bool Empty() { return( !_CurrentSize ); }

        /* Epistrofh toy mikroteroy stoixeioy */
        Type ReturnMin() { return _StoragePlace[ 0 ]; }

        void DeleteMin();                   /* Diagrafh  toy mikroteroy stoixeioy */
        void Insert( Type _Node );          /* Eisagwgh stoixeioy sto swro */

        void Sort(                          /* Taksinomish pinka me ton algori8mo heapsort */
            SORT_ORDER,                     // Typos ta3inomisis: (Au3ousa, F8inousa)
            Type*,                          // Pinakas pros taksinomish
            _UINT                           // Pli8os stoixeiwn
        );
};

// ------------------------------------------------------------------------------------------------
/*
**  Constructor. Katanomh mnhmhs gia tin apouhkeysh tou swrou. Se periptwsh aneparkeias mnhmhs 
**  epistrofh.
*/
template <class Type>
Heap< Type >::Heap( int max_size )
{
    if( max_size < MAX_HEAP_SIZE )
    {
        /* Yparxei xwros gia na apo8hkeytei o swros */
        _StoragePlace = new Type[ max_size ];

        _MaximumSize = (_UINT) max_size;    /* arxikopoihsh metavlhtwn */
        _CurrentSize = 0;
    } //else return NULL;
}

// ------------------------------------------------------------------------------------------------
/*
**  Destructor. Apodesmeysh mnhmhs.
*/
template <class Type>
Heap< Type >::~Heap( void )
{
    if( _StoragePlace != NULL )
        delete [] _StoragePlace;
}

// ------------------------------------------------------------------------------------------------
/*
**  Heapify. To stoixeio sthn 8esh Index mporei na mhn ikanopoiei thn idiotita tou swroy. H 
**  synarthsh ayth epanaferei thn idiothta swrou ston pinaka HeapMatrix, me anadromiko tropo. 
**  H synarthsh ayth mporei na epanaferei thn idiothta toy swroy se megistous alla kai se
**  elaxistous swrous.
**
**  Polyplokothta: O( log _CurrentSize ).
*/
template <class Type>
void Heap<Type>::Heapify( Type *HeapMatrix, _UINT Index, SORT_ORDER Order )
{
    _UINT left,                             /* Aristero paidi   */
          right,                            /* De3io paidi      */
          smaller,                          /* Mikrotero  paidi */
          greater,                          /* Megalytero paidi */
          chgnode;                          /* Komvos pros enallagi */

    left  = Left( Index );                  /* 8esh aristerou paidiou */
    right = Right( Index );                 /* 8esh deksiou   paidiou */

    /*
    **  Vres tautoxrona to megalytero kai to mikrotero paidi kai analoga me to eidos toy swrou 
    **  Min-Heap h Max-Heap diale3e to katallhlo.
    */
    if( left == _INFINITY ) {               /* Den yparxei aristero paidi */
        smaller = Index;
        greater = Index;
    }
    else if( HeapMatrix[left] < HeapMatrix[Index] )
    {
        /* To aristero paidi einai mikrotero apo ton patera */
        smaller = left;
        greater = Index;
    }
    else { /* To aristero paidi einai megalytero apo ton patera */
        greater = left;
        smaller = Index;
    }

    if( right < _INFINITY )                 /* Yparxei deksio paidi */
    {
        if( HeapMatrix[right] < HeapMatrix[smaller] )
        {
            /* To de3io paidi einai to mikrotero apo ola */
            smaller = right;
        }
        if( HeapMatrix[right] > HeapMatrix[greater] )
        {
            /* To de3io paidi einai to megalytero apo ola */
            greater = right;
        }
    }

    /* Elegxos gia Min-Heap h Max-Heap*/
    chgnode = (Order == DECREASE_ORDER) ? smaller : greater;

    if( Index != chgnode )
    {
        /*
        ** To stoixeio sthn 8esh Index DEN ikanopoiei thn idiothta toy swrou. Allakse 8esh me to 
        ** mikrotero paidi toy.
        */
        Type temp = HeapMatrix[ chgnode ];
        HeapMatrix[ chgnode ] = HeapMatrix[ Index ];
        HeapMatrix[ Index ]   = temp;

        /*
        ** Meta thn enallagi mporei na min ikanopoieitai h idiothta toy swroy sto stoixeio smaller.
        ** Eleg3e anadromika thn idiothena tou swroy, proxwrwntas epipedo parakatw. Kai yparxoun 
        ** logN epipeda.
        */
        Heapify( HeapMatrix, chgnode, Order );
    }

    return;
}

// ------------------------------------------------------------------------------------------------
/*
**  Eisagwgh stoixeioy ston swro. To stoixeio topo8eteitai ws fyllo kai anevainei sto dentro mexri 
**  na vreuei se enan komvo opoy na ikanopoieitai h idiothta toy swrou.
**
**  Polyplokothta: O( log _CurrentSize )
*/
template <class Type>
void Heap<Type>::Insert( Type _Node )
{
    if( _CurrentSize < _MaximumSize )
    {
        _UINT index = _CurrentSize++;

        _StoragePlace[ index ] = _Node;     /* Topo8ethsh ston pato */

        while( index > 0 &&
               _StoragePlace[index] < _StoragePlace[ Parent(index) ] )
        {
            /* Enallagi twn stoixeiwn */
            Type temp = _StoragePlace[ index ];
            _StoragePlace[ index ]  = _StoragePlace[ Parent(index) ];
            _StoragePlace[ Parent(index) ] = temp;

            /* Aneva ena epipedo parapanw */
            index = Parent( index );
        }
    }
}

// ------------------------------------------------------------------------------------------------
/*
**  Diagrafh toy mikroteroy stoixeioy. Enallagi me to teleytaio fyllo. Epanafora ths idiothtas toy 
**  swroy ston prwto komvo.
**
**  Polyplokothta: O( log _CurrentSize )
*/
template <class Type>
void Heap<Type>::DeleteMin( void )
{
    if( _CurrentSize > 0 )
    {
        /* Yparxoun stoixeia pros diagrafh */

        /* Diagrafh toy mikroteroy kai meiwsh toy mege8ous*/
        _StoragePlace[ 0 ] = _StoragePlace[ --_CurrentSize ];

        /* Epanafora idiothtas toy swrou sthn koryfh toy swrou */
        Heapify( _StoragePlace, 0, MINIMUM );
    }
}

// ------------------------------------------------------------------------------------------------
/*
**  Taksinomisi enos pinaka. Antigrafh toy pinaka ston swro. Metatroph toy pinaka se swro. E3agwgh 
**  toy meaglyteroy stoixeioy kai topo8ethsh sto telos toy pinaka. Epanalapsi toy prohgoymernoy 
**  vhmatos mexri na meinei 1 stoixeio ston swro.
**
**  Polyplokothta: O( Count * log(Count) ).
*/
template <class Type>
void Heap<Type>::Sort( SORT_ORDER SortOrder, Type *Array, _UINT Count )
{
    int iterator, backup;

    _CurrentSize = Count - 1;
    backup = _CurrentSize;

    // build heap
    for( iterator=_CurrentSize >> 1; iterator>-1; iterator-- )
    {
        /*
        **  Epanafora idiothtas swroy gia olous tous komvous, ektos apo ta fylla
        */
        Heapify( Array, (_UINT)iterator, SortOrder );
    }

    for( iterator=_CurrentSize; iterator>0; iterator-- )
    {
        /* Topo8ethsh toy mikroteroy stoixeioy e3w apo ton swro */
        _UINT temp = Array[ 0 ];
        Array[ 0 ] = Array[ iterator ];
        Array[ iterator ] = temp;

        /* Meiwsh toy mege8ous toy swroy */
        _CurrentSize--;

        /* Epanafora idiothtas toy swroy */
        Heapify( Array, 0, SortOrder );
    }

    _CurrentSize = backup;

    return;
}

#endif
// ------------------------------------------------------------------------------------------------
