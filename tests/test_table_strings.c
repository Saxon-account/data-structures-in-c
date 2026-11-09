/*
 * Test for the string hash table in hash-table-strings/.
 *
 * Exercises the set API (set.h) against the hash table implementation
 * in table.c, including enough elements to force collisions and a
 * remove followed by lookups of the remaining elements.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "set.h"

int main(void)
{
    SET *sp;
    char word[16];
    char **elts;
    int i;

    sp = createSet(50);
    assert(sp != NULL);
    assert(numElements(sp) == 0);

    /* Insert 40 distinct words into a 50-slot table. */
    for (i = 0; i < 40; i++) {
        sprintf(word, "word%d", i);
        addElement(sp, word);
    }
    assert(numElements(sp) == 40);

    /* Every inserted word must be findable. */
    for (i = 0; i < 40; i++) {
        sprintf(word, "word%d", i);
        assert(findElement(sp, word) != NULL);
    }
    assert(findElement(sp, "word40") == NULL);

    /* A duplicate insert must not change the count. */
    addElement(sp, "word7");
    assert(numElements(sp) == 40);

    /* getElements must return all 40 elements. */
    elts = getElements(sp);
    assert(elts != NULL);
    free(elts);

    /* Remove the even-numbered words; the odd ones must remain. */
    for (i = 0; i < 40; i += 2) {
        sprintf(word, "word%d", i);
        removeElement(sp, word);
    }
    assert(numElements(sp) == 20);
    for (i = 0; i < 40; i++) {
        sprintf(word, "word%d", i);
        if (i % 2 == 0)
            assert(findElement(sp, word) == NULL);
        else
            assert(findElement(sp, word) != NULL);
    }

    destroySet(sp);
    printf("PASS: test_table_strings\n");
    return 0;
}
