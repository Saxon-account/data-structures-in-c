/*
Author: Saxon Meznarich
File name: pqueue.c
Description: implements a priority queue using a dynamic array. provides functions for
queue creation, destruction, insertion, deletion of the minimum value (root), and returning
all elements.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "pqueue.h"

#define PARENT(x) (((x) - 1) / 2)
#define LEFT(x) ((x) * 2 + 1)
#define RIGHT(x) ((x) * 2 + 2)

struct pqueue
{
    int count;        /* number of entries in array*/
    int length;       /* length of allocated array */
    void **data;      /* allocated array of entries */
    int (*compare)(); /* comparison function */
};

// Creates a new priority queue
// O(1)
PQ *createQueue(int (*compare)())
{
    PQ *pq = malloc(sizeof(PQ));
    assert(pq != NULL);
    pq->count = 0;
    pq->length = 10;
    pq->data = malloc(sizeof(void *) * pq->length);
    pq->compare = compare; // uses outside function for element comparison, returns int
    return pq;
}

// Frees all elements in the priority queue
// O(n)
void destroyQueue(PQ *pq)
{
    assert(pq != NULL);
    free(pq->data);
    free(pq);
}

// Returns the number of entries in the priority queue
// O(1)
int numEntries(PQ *pq)
{
    assert(pq != NULL);
    return pq->count;
}

// Inserts a value entry using heap up insertion
// O(log n): A binary heap has a height of log n because of its shape 
// (complete/nearly complete tree). Worst case insertion requires swapping all elements
// for the height of the heap (log n)
void addEntry(PQ *pq, void *entry)
{
    assert((pq != NULL)&&(entry != NULL));

    if (pq->count >= pq->length)
    {
        int tempLength = pq->length * 2;
        void **tempArr = realloc(pq->data, tempLength * sizeof(void *));
        assert(tempArr != NULL);
        pq->data = tempArr;
        pq->length = tempLength;
    }

    int index = pq->count;

    while (index > 0)
    {
        int pIndex = (PARENT(index));
        void *pVal = pq->data[pIndex];

        if (pq->compare(entry, pVal) < 0)
        {
            pq->data[index] = pVal;
            index = PARENT(index);
        }
        else
        {
            break;
        }
    }
    pq->data[index] = entry;
    pq->count++;
}

// Removes the minimum value entry using heap down deletion
// O(log n): Same concept as insertion
void *removeEntry(PQ *pq)
{
    assert(pq != NULL);

    if (pq->count == 0)
        return NULL;

    int pIndex = 0;
    void *min = pq->data[pIndex];

    void *lastVal = pq->data[pq->count - 1];
    pq->count--;

    while (LEFT(pIndex) < pq->count)
    {
        int lIndex = LEFT(pIndex);
        int rIndex = RIGHT(pIndex);
        int minIndex = lIndex;

        if (rIndex < pq->count)
        {
            if (pq->compare(pq->data[rIndex], pq->data[lIndex]) < 0)
            {
                minIndex = rIndex;
            }
        }

        if (pq->compare(lastVal, pq->data[minIndex]) > 0)
        {
            pq->data[pIndex] = pq->data[minIndex];
            pIndex = minIndex;
        }
        else
        {
            break;
        }
    }
    pq->data[pIndex] = lastVal;
    return min;
}