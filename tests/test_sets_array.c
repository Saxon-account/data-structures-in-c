/*
 * Test for the array-based string set in sets-array/.
 *
 * The same test is compiled against both implementations of the set
 * API (sorted.c and unsorted.c).  The Makefile defines IMPL_NAME so
 * each binary reports which implementation it exercised.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "set.h"

#ifndef IMPL_NAME
#define IMPL_NAME "array"
#endif

int main(void)
{
    SET *sp;
    char *found, **elts;
    int i, seen_apple, seen_banana, seen_cherry;

    sp = createSet(100);
    assert(sp != NULL);
    assert(numElements(sp) == 0);
    assert(findElement(sp, "apple") == NULL);

    addElement(sp, "banana");
    addElement(sp, "apple");
    addElement(sp, "cherry");
    assert(numElements(sp) == 3);

    /* Adding a duplicate must not change the set. */
    addElement(sp, "apple");
    assert(numElements(sp) == 3);

    found = findElement(sp, "apple");
    assert(found != NULL && strcmp(found, "apple") == 0);
    assert(findElement(sp, "durian") == NULL);

    /* getElements must return exactly the elements that were added. */
    elts = getElements(sp);
    assert(elts != NULL);
    seen_apple = seen_banana = seen_cherry = 0;
    for (i = 0; i < numElements(sp); i++) {
        if (strcmp(elts[i], "apple") == 0)
            seen_apple = 1;
        else if (strcmp(elts[i], "banana") == 0)
            seen_banana = 1;
        else if (strcmp(elts[i], "cherry") == 0)
            seen_cherry = 1;
    }
    assert(seen_apple && seen_banana && seen_cherry);
    free(elts);

    removeElement(sp, "banana");
    assert(numElements(sp) == 2);
    assert(findElement(sp, "banana") == NULL);
    assert(findElement(sp, "cherry") != NULL);

    /* Removing an element that is not in the set is a no-op. */
    removeElement(sp, "durian");
    assert(numElements(sp) == 2);

    destroySet(sp);
    printf("PASS: test_sets_array (%s)\n", IMPL_NAME);
    return 0;
}
