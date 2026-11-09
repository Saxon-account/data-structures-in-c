/*
File: table.c
 
Author:	2025, Saxon Meznarich


Description: This file contains the functions for creating and *mainipulating
a hash table of generic datatype elements. *with functions for searching, adding, removing,
deleting the table and getting the generic datatype elements in the table.
       
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "set.h"

#define EMPTY 0
#define FILLED 1
#define DELETED 2

struct set
{
   int count;   // number of elements
   int length;  // table size
   void **elts; // array of generic pointer elements
   char *flags; // array of "states" of each slot in the table - uses char for memory efficiency
   int (*compare)();   //outside function for comparing elements
   unsigned (*hash)(); // outside function for hash function
};

// Linear probing for hash table
// O(1): average runtime for hash table search 
// O(n): worst case
static int search(SET *sp, void *elt, bool *found)
{
   unsigned hash = (*sp->hash)(elt);
   int firstDeleted = -1;
   for (int i = 0; i < sp->length; i++)
   {
       int index = (hash + i) % sp->length;
       // Immediately stops search (returns -1 if element does not exist in array,
       // else index of first available slot)
       if (sp->flags[index] == EMPTY)
       {
           *found = false;
           if (firstDeleted != -1)
               return firstDeleted;
           else
               return index;
       }
       // Possible insertion point
       if (sp->flags[index] == DELETED)
       {
           if (firstDeleted == -1)
               firstDeleted = index;
       }
       // Element exists in table
       else if ((*sp->compare)(sp->elts[index], elt) == 0)
       {
           *found = true;
           return index;
       }
   }
   *found = false;
   if (firstDeleted != -1)
       return firstDeleted;
   else
       return -1;
}

// Initializes the set struct
// O(n): loops through all elements in table and assigns flag values to EMPTY
SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)())
{
   SET *sp = malloc(sizeof(SET));
   assert(sp != NULL);
   sp->count = 0;
   sp->length = maxElts;
   sp->elts = malloc(sizeof(void *) * maxElts);
   sp->flags = malloc(sizeof(char) * maxElts);
   sp->compare = compare; // uses outside function for element comparison, returns int
   sp->hash = hash;
   assert(sp->elts != NULL && sp->flags != NULL);
   for (int i = 0; i < maxElts; i++)
       sp->flags[i] = EMPTY;
   return sp;
}

// Destroys the set struct by freeing all local variable memory slots
// O(1): User must free elements outside of function
void destroySet(SET *sp)
{
   assert(sp != NULL);
   free(sp->elts);
   free(sp->flags);
   free(sp);
}

// Returns the number of elements in the current table
// O(1): average runtime for hash table search
// O(n): worst case
int numElements(SET *sp)
{
   assert(sp != NULL);
   return sp->count;
}

// Adds an element that does not exist already in the table
// O(1): average runtime for hash table search 
// O(n): worst case
void addElement(SET *sp, void *elt)
{
   assert(sp != NULL && elt != NULL);
   bool found;
   int index = search(sp, elt, &found);
   if (!found)
   {
       sp->elts[index] = elt;
       sp->flags[index] = FILLED;
       sp->count++;
   }
}

// Removes an element if found in the table
// O(1): average runtime for hash table search
// O(n): worst case
void removeElement(SET *sp, void *elt)
{
   assert(sp != NULL && elt != NULL);
   bool found;
   int index = search(sp, elt, &found);
   if (found)
   {
       sp->flags[index] = DELETED;
       sp->count--;
   }
}

// Returns an element string if that element is found in the table else returns NULL
// O(1): average runtime for hash table search
// O(n): worst case
void *findElement(SET *sp, void *elt)
{
   assert(sp != NULL && elt != NULL);
   bool found;
   int index = search(sp, elt, &found);
   if (found)
       return sp->elts[index];
   else
       return NULL;
}

// Returns the current hash table with no empty slots
// O(n): loops through all n elements
void *getElements(SET *sp)
{
   assert(sp != NULL);
   void **array = malloc(sizeof(void *) * sp->count);
   assert(array != NULL);


   int j = 0;
   for (int i = 0; i < sp->length; i++)
   {
       if (sp->flags[i] == FILLED)
           array[j++] = sp->elts[i];
   }
   return (void *)array; // returns a void datatype pointer that points to the array of elements
}