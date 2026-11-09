/* 
Author: Saxon Meznarich, 2025
File name: list.c
Description: implements an unsorted doubly linked circular list ADT with a dummy node.
Provides functions for list creation, deletion, insertion, removal, retrieval,
searching, and returning all elements.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "list.h"

struct list
{
    int count;
    struct node *head;
    int (*compare)();
};

struct node
{
    void *data;
    struct node *next;
    struct node *prev;
};

// Initializes a list structure
// Time complexity: O(1)
LIST *createList(int (*compare)())
{
    LIST *lp;
    lp = malloc(sizeof(LIST));
    assert(lp != NULL);
    lp->count = 0;

    lp->head = malloc(sizeof(struct node)); // dummy node
    lp->head->next = lp->head;
    lp->head->prev = lp->head;

    lp->compare = compare;
    return lp;
}

// Removes all nodes in the list, head, and list pointer, destroying the list
// Time complexity: O(n)
void destroyList(LIST *lp)
{
    assert(lp != NULL);
    struct node *pDel = lp->head->next;
    struct node *pPrev;

    while (pDel != lp->head)
    {
        pPrev = pDel->next;
        free(pDel);
        pDel = pPrev;
    }
    free(pDel);
    free(lp);
}

// Returns the number of nodes in the list
// Time complexity: O(1)
int numItems(LIST *lp)
{
    assert(lp != NULL);
    return lp->count;
}

// Adds a node to the front of the list
// Time complexity: O(1)
void addFirst(LIST *lp, void *item)
{
    assert(lp != NULL);
    struct node *pNew = malloc(sizeof(struct node));
    assert(pNew != NULL);

    pNew->data = item;

    pNew->next = lp->head->next;
    pNew->prev = lp->head;

    lp->head->next->prev = pNew;
    lp->head->next = pNew;

    lp->count++;
}

// Adds a node to the end of the list
// Time complexity: O(1)
void addLast(LIST *lp, void *item)
{
    assert(lp != NULL);
    struct node *pNew = malloc(sizeof(struct node));
    assert(pNew != NULL);

    pNew->data = item;

    pNew->next = lp->head;
    pNew->prev = lp->head->prev;

    lp->head->prev->next = pNew;
    lp->head->prev = pNew;

    lp->count++;
}

// Removes the first node in the list
// Time complexity: O(1)
void *removeFirst(LIST *lp)
{
    assert(lp != NULL && lp->count > 0);

    struct node *pDel = lp->head->next;
    void *item = pDel->data;

    lp->head->next = pDel->next;
    pDel->next->prev = lp->head;

    free(pDel);
    lp->count--;

    return item;
}

// Removes the last node in the list
// Time complexity: O(1)
void *removeLast(LIST *lp)
{
    assert(lp != NULL && lp->count > 0);

    struct node *pDel = lp->head->prev;
    void *item = pDel->data;

    lp->head->prev = pDel->prev;
    pDel->prev->next = lp->head;

    free(pDel);
    lp->count--;

    return item;
}

// Returns the first item in the list
// Time complexity: O(1)
void *getFirst(LIST *lp)
{
    assert(lp != NULL);
    return lp->head->next->data;
}

// Returns the last item in the list
// Time complexity: O(1)
void *getLast(LIST *lp)
{
    assert(lp != NULL);
    return lp->head->prev->data;
}

// Removes a specified item from the list
// Time complexity: O(n)
void removeItem(LIST *lp, void *item)
{
    assert(lp != NULL && item != NULL);
    assert(lp->compare != NULL);

    struct node *pDel = lp->head->next;

    while (pDel != lp->head)
    {
        if (lp->compare(item, pDel->data) == 0)
        {
            pDel->prev->next = pDel->next;
            pDel->next->prev = pDel->prev;

            free(pDel);
            lp->count--;
            return;
        }
        pDel = pDel->next;
    }
}

// Returns a specified item from the list or NULL if it is not present
// Time complexity: O(n)
void *findItem(LIST *lp, void *item)
{
    assert(lp != NULL && item != NULL);
    assert(lp->compare != NULL);

    struct node *pLoc = lp->head->next;

    while (pLoc != lp->head)
    {
        if (lp->compare(item, pLoc->data) == 0)
            return pLoc->data;
        pLoc = pLoc->next;
    }
    return NULL;
}

// Returns the current list
// Time complexity: O(n)
void *getItems(LIST *lp)
{
    assert(lp != NULL);
    void **array = malloc(sizeof(void *) * lp->count);
    assert(array != NULL);

    int i = 0;
    struct node *pLoc = lp->head->next;
    while (pLoc != lp->head)
    {
        array[i++] = pLoc->data;
        pLoc = pLoc->next;
    }
    return (void *)array;
}