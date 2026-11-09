/*
Author: Saxon Meznarich, 2025
File name: set.c
Description: This file contains the functions for creating and mainipulating
a set ADT for generic pointer types. Each generic pointer points to
a list ADT containing elements. Contains functions for searching, adding, removing,
deleting and returning all elements in an array.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "set.h"
#include "list.h"

struct set
{
   int count;
   int length;
   void **lps;
   int (*compare)();
   unsigned (*hash)();
};

// Initializes the set struct with a load factor of 20
// O(n): loops through all n list pointers and initializes each with a list
SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)())
{
   SET *sp = malloc(sizeof(SET));
   assert(sp != NULL);
   sp->count = 0;

   int alpha = 20;
   sp->length = (maxElts / alpha) + ((maxElts % alpha) != 0);

   sp->lps = malloc(sizeof(void *) * sp->length);
   sp->compare = compare; // uses outside function for element comparison, returns int
   sp->hash = hash;

   assert(sp->lps != NULL);
   for (int i = 0; i < sp->length; i++)
      sp->lps[i] = createList(sp->compare);
   return sp;
}

// Destroys the set struct by freeing all local variable memory slots
// O(n): For n list pointers in the set, all n need to be freed
void destroySet(SET *sp)
{
   assert(sp != NULL);
   for (int i = 0; i < sp->length; i++)
      destroyList(sp->lps[i]);
   free(sp->lps);
   free(sp);
}

// Returns the number of elements in the current table in each list
// O(1)
int numElements(SET *sp)
{
   assert(sp != NULL);
   return sp->count;
}

// Adds an element if it does not exist already in the table
// O(1): average runtime for hash table search
// O(n): worst case, search through all n elements to find an item
void addElement(SET *sp, void *elt)
{
   assert(sp != NULL && elt != NULL);
   unsigned hash = (*sp->hash)(elt);
   int index = hash % sp->length;
   if (findItem(sp->lps[index], elt) == NULL)
   {
      addFirst(sp->lps[index], elt);
      sp->count++;
   }
}

// Removes an element if found in the table
// O(1): average runtime for hash table search
// O(n): worst case, search through all n elements to find an item
void removeElement(SET *sp, void *elt)
{
   assert(sp != NULL && elt != NULL);
   unsigned hash = (*sp->hash)(elt);
   int index = hash % sp->length;
   if (findItem(sp->lps[index], elt) != NULL)
   {
      removeItem(sp->lps[index], elt);
      sp->count--;
   }
}

// Returns an element if that element is found in the table else returns NULL
// O(1): average runtime for hash table search
// O(n): worst case, search through all n elements to find an item
void *findElement(SET *sp, void *elt)
{
   assert(sp != NULL && elt != NULL);
   unsigned hash = (*sp->hash)(elt);
   int index = hash % sp->length;

   return findItem(sp->lps[index], elt);
}

// Returns the current hash table with no empty slots
// O(n): loops through all n elements
void *getElements(SET *sp)
{
   assert(sp != NULL);
   void **array = malloc(sizeof(void *) * sp->count);
   assert(array != NULL);

   int index = 0;
   for (int i = 0; i < sp->length; i++)
   {
      LIST *chain = sp->lps[i];
      int listSize = numItems(chain);

      if (listSize > 0)
      {
         void **lArray = getItems(chain);
         memcpy(array + index, lArray, listSize * sizeof(void *));
         free(lArray);
      }
      index += listSize;
   }
   return (void *)array;
}
