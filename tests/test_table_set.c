/*
 * Test for the generic hash table set in hash-table-set/ (project6).
 *
 * The implementation uses open addressing with linear probing, so
 * this test inserts enough string elements to force collisions and
 * probe chains, then deletes from the middle of the chains and
 * confirms the remaining elements are still found.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "set.h"

static unsigned strhash(char *s)
{
    unsigned hash = 0;

    while (*s != '\0')
        hash = 31 * hash + *s++;

    return hash;
}

int main(void)
{
    SET *sp;
    char word[16];
    char **elts;
    int i;

    sp = createSet(64, strcmp, strhash);
    assert(sp != NULL);
    assert(numElements(sp) == 0);

    /* 48 distinct words in a 64-slot table: plenty of collisions. */
    for (i = 0; i < 48; i++) {
        sprintf(word, "key%d", i);
        addElement(sp, strdup(word));
    }
    assert(numElements(sp) == 48);

    for (i = 0; i < 48; i++) {
        sprintf(word, "key%d", i);
        assert(findElement(sp, word) != NULL);
    }
    assert(findElement(sp, "key48") == NULL);

    /* Duplicates are rejected. */
    addElement(sp, "key3");
    assert(numElements(sp) == 48);

    /* Delete every third key; deleted slots must not break the probe
       chains leading to the keys behind them. */
    for (i = 0; i < 48; i += 3) {
        sprintf(word, "key%d", i);
        removeElement(sp, word);
    }
    assert(numElements(sp) == 32);
    for (i = 0; i < 48; i++) {
        sprintf(word, "key%d", i);
        if (i % 3 == 0)
            assert(findElement(sp, word) == NULL);
        else
            assert(findElement(sp, word) != NULL);
    }

    /* Free the stored strings (the set keeps the pointers it was
       given), then the array, then the set. */
    elts = (char **) getElements(sp);
    assert(elts != NULL);
    for (i = 0; i < numElements(sp); i++)
        free(elts[i]);
    free(elts);

    destroySet(sp);
    printf("PASS: test_table_set\n");
    return 0;
}
