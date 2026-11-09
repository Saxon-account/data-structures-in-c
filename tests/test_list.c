/*
 * Test for the linked list and the set built on it in linked-list/.
 *
 * Part 1 exercises the deque operations from list.h.  Part 2 uses
 * set.h: a generic set whose buckets are lists from this same
 * directory (set.c is implemented on top of list.c).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "list.h"
#include "set.h"

static unsigned strhash(char *s)
{
    unsigned hash = 0;

    while (*s != '\0')
        hash = 31 * hash + *s++;

    return hash;
}

static void test_list(void)
{
    LIST *lp;
    char **items;

    lp = createList(strcmp);
    assert(lp != NULL);
    assert(numItems(lp) == 0);

    addFirst(lp, "b");
    addFirst(lp, "a");
    addLast(lp, "c");
    addLast(lp, "d");
    assert(numItems(lp) == 4);

    assert(strcmp((char *) getFirst(lp), "a") == 0);
    assert(strcmp((char *) getLast(lp), "d") == 0);

    /* getItems returns the items in list order. */
    items = (char **) getItems(lp);
    assert(items != NULL);
    assert(strcmp(items[0], "a") == 0);
    assert(strcmp(items[1], "b") == 0);
    assert(strcmp(items[2], "c") == 0);
    assert(strcmp(items[3], "d") == 0);
    free(items);

    assert(findItem(lp, "c") != NULL);
    assert(findItem(lp, "z") == NULL);

    removeItem(lp, "b");
    assert(numItems(lp) == 3);
    assert(findItem(lp, "b") == NULL);

    assert(strcmp((char *) removeFirst(lp), "a") == 0);
    assert(strcmp((char *) removeLast(lp), "d") == 0);
    assert(numItems(lp) == 1);
    assert(strcmp((char *) getFirst(lp), "c") == 0);
    assert(strcmp((char *) getLast(lp), "c") == 0);

    destroyList(lp);
}

static void test_set_on_list(void)
{
    SET *sp;
    char **elts;
    int i, count;

    sp = createSet(100, strcmp, strhash);
    assert(sp != NULL);
    assert(numElements(sp) == 0);

    addElement(sp, "red");
    addElement(sp, "green");
    addElement(sp, "blue");
    addElement(sp, "green");    /* duplicate */
    assert(numElements(sp) == 3);

    assert(findElement(sp, "green") != NULL);
    assert(findElement(sp, "yellow") == NULL);

    elts = (char **) getElements(sp);
    assert(elts != NULL);
    count = 0;
    for (i = 0; i < numElements(sp); i++)
        if (elts[i] != NULL)
            count++;
    assert(count == 3);
    free(elts);

    removeElement(sp, "green");
    assert(numElements(sp) == 2);
    assert(findElement(sp, "green") == NULL);
    assert(findElement(sp, "red") != NULL);
    assert(findElement(sp, "blue") != NULL);

    destroySet(sp);
}

int main(void)
{
    test_list();
    test_set_on_list();
    printf("PASS: test_list (list + set-on-list)\n");
    return 0;
}
