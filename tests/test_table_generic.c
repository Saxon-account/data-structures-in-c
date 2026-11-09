/*
 * Test for the generic hash table in hash-table-generic/.
 *
 * Uses heap-allocated integers as elements, with caller-supplied
 * compare and hash functions, the way the generic set API is meant
 * to be used.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "set.h"

static int intcmp(int *a, int *b)
{
    return (*a < *b) ? -1 : (*a > *b);
}

static unsigned inthash(int *a)
{
    return (unsigned) *a * 31u;
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
    SET *sp;
    int probe, i, *found, **elts;

    sp = createSet(100, intcmp, inthash);
    assert(sp != NULL);
    assert(numElements(sp) == 0);

    /* Insert the values 0, 10, 20, ..., 90. */
    for (i = 0; i < 10; i++)
        addElement(sp, newint(i * 10));
    assert(numElements(sp) == 10);

    /* Lookup is by value, not by pointer identity. */
    probe = 40;
    found = findElement(sp, &probe);
    assert(found != NULL && *found == 40);

    probe = 45;
    assert(findElement(sp, &probe) == NULL);

    /* A duplicate value must not be added twice.  The set stores the
       first pointer, so the extra one is still ours to free. */
    {
        int *dup = newint(40);
        addElement(sp, dup);
        assert(numElements(sp) == 10);
        probe = 40;
        assert(findElement(sp, &probe) != (void *) dup);
        free(dup);
    }

    /* getElements must return the 10 stored pointers. */
    elts = (int **) getElements(sp);
    assert(elts != NULL);
    {
        int sum = 0;
        for (i = 0; i < numElements(sp); i++)
            sum += *elts[i];
        assert(sum == 0 + 10 + 20 + 30 + 40 + 50 + 60 + 70 + 80 + 90);
    }
    free(elts);

    /* Remove one value and confirm the rest are unaffected. */
    probe = 40;
    removeElement(sp, &probe);
    assert(numElements(sp) == 9);
    probe = 40;
    assert(findElement(sp, &probe) == NULL);
    probe = 50;
    assert(findElement(sp, &probe) != NULL);

    /* Free the elements through the array, then the array itself. */
    elts = (int **) getElements(sp);
    for (i = 0; i < numElements(sp); i++)
        free(elts[i]);
    free(elts);

    destroySet(sp);
    printf("PASS: test_table_generic\n");
    return 0;
}
