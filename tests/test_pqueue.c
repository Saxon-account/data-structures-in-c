/*
 * Test for the binary-heap priority queue in priority-queue/.
 *
 * Entries must come out of removeEntry in priority order: with the
 * integer comparison below, smallest first.  More than ten entries
 * are added so the heap has to grow past its initial capacity.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "pqueue.h"

static int intcmp(int *a, int *b)
{
    return (*a < *b) ? -1 : (*a > *b);
}

static int *newint(int value)
{
    int *p = malloc(sizeof(int));
    assert(p != NULL);
    *p = value;
    return p;
}

int main(void)
{
    static const int input[] =
        {42, 7, 19, 73, 3, 55, 28, 91, 11, 64, 37, 88};
    static const int sorted[] =
        {3, 7, 11, 19, 28, 37, 42, 55, 64, 73, 88, 91};
    const int n = (int) (sizeof(input) / sizeof(input[0]));
    PQ *pq;
    int i, *p;

    pq = createQueue(intcmp);
    assert(pq != NULL);
    assert(numEntries(pq) == 0);

    for (i = 0; i < n; i++)
        addEntry(pq, newint(input[i]));
    assert(numEntries(pq) == n);

    for (i = 0; i < n; i++) {
        p = removeEntry(pq);
        assert(p != NULL && *p == sorted[i]);
        free(p);
        assert(numEntries(pq) == n - i - 1);
    }

    destroyQueue(pq);
    printf("PASS: test_pqueue\n");
    return 0;
}
