/*
File:        table.c
 
Author:	2025, Saxon Meznarich


Description: This file contains the functions for creating and *mainipulating
a hash table of strings. *with functions for searching, adding, removing,
deleting the table and getting the strings in the table.
       
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

// Declares string hash function
// O(n): while loop runtime goes through n elements
unsigned strhash(char *s) {
   unsigned hash = 0;
   while (*s != '\0')
       hash = 31 * hash + *s++;
   return hash;
}

struct set {
   int count;        // number of elements
   int length;       // table size
   char **elts;      // array of string pointer elements
   char *flags;      // array of "states" of each slot in the table - uses char for memory efficiency
};

// Linear probing for hash table
// O(1): average runtime for hash table search 
// O(n): worst case
static int search(SET *sp, char *elt, bool *found) {
   unsigned hash = strhash(elt);
   int firstDeleted = -1;
   for (int i = 0; i < sp->length; i++) {
       int index = (hash + i) % sp->length;
       // Immediately stops search (returns -1 if element does not exist in array,
       // else index of first available slot)
       if (sp->flags[index] == EMPTY) {
           *found = false;
           if (firstDeleted != -1)
               return firstDeleted;
           else
               return index;
       }
       // Possible insertion point
       if (sp->flags[index] == DELETED) {
           if (firstDeleted == -1)
               firstDeleted = index;
       }
       // Element exists in table
       else if (strcmp(sp->elts[index], elt) == 0) {
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
SET *createSet(int maxElts) {
   SET *sp = malloc(sizeof(SET));
   assert(sp != NULL);
   sp->count = 0;
   sp->length = maxElts;
   sp->elts = malloc(sizeof(char *) * maxElts);
   sp->flags = malloc(sizeof(char) * maxElts);
   assert(sp->elts != NULL && sp->flags != NULL);
   for (int i = 0; i < maxElts; i++)
       sp->flags[i] = EMPTY;
   return sp;
}

// Destroys the set struct by freeing all variable memory slots
// O(n): loops through all elements in table
void destroySet(SET *sp) {
   assert(sp != NULL);
   for (int i = 0; i < sp->length; i++) {
       if (sp->flags[i] == FILLED)
           free(sp->elts[i]);
   }
   free(sp->elts);
   free(sp->flags);
   free(sp);
}

// Returns the number of elements in the current table
// O(1): average runtime for hash table search
// O(n): worst case
int numElements(SET *sp) {
   assert(sp != NULL);
   return sp->count;
}

// Adds an element that does not exist already in the table
// O(1): average runtime for hash table search 
// O(n): worst case
void addElement(SET *sp, char *elt) {
    assert(sp != NULL && elt != NULL);
    bool found;
    char *copy;
    int index = search(sp, elt, &found);
    if (!found){
        assert(sp->count < sp->length);
        copy = strdup(elt);
        assert(copy != NULL);
        sp->elts[index] = copy;
        sp->flags[index] = FILLED;
        sp->count++;
    }
}

// Removes an element if found in the table
// O(1): average runtime for hash table search
// O(n): worst case
void removeElement(SET *sp, char *elt) {
   assert(sp != NULL && elt != NULL);
   bool found;
   int index = search(sp, elt, &found);
   if (found){
       free(sp->elts[index]);
       sp->flags[index] = DELETED;
       sp->count--;
   }
}

// Returns an element string if that element is found in the table else returns NULL
// O(1): average runtime for hash table search
// O(n): worst case
char *findElement(SET *sp, char *elt) {
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
char **getElements(SET *sp) {
   assert(sp != NULL);
   char **array = malloc(sizeof(char *) * sp->count);
   assert(array != NULL);
   int j = 0;
   for (int i = 0; i < sp->length; i++) {
       if (sp->flags[i] == FILLED)
           array[j++] = sp->elts[i];
   }
   return array;
}