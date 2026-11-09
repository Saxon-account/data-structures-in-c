//Saxon Meznarich


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "set.h"


struct set {
   int count;
   int length;
   char **elts;
};

// Returns index of element if found, otherwise index for add function
// O(log n): Binary search
static int search(SET *sp, char *elt, bool *found) {
   int low = 0, high = sp->count - 1, mid, cmp;
   while (low <= high) {
       mid = (low + high) / 2;
       cmp = strcmp(elt, sp->elts[mid]);
       if (cmp < 0)
           high = mid - 1;
       else if (cmp > 0)
           low = mid + 1;
       else {
           *found = true;
           return mid;
       }
   }
   *found = false;
   return low; // starting index for add function
}

// Create a new set with maxElts capacity
// O(1): no for loops required
SET *createSet(int maxElts) {
   SET *sp = malloc(sizeof(SET));
   assert(sp != NULL);
   sp->count = 0;
   sp->length = maxElts;
   sp->elts = malloc(sizeof(char *) * maxElts);
   assert(sp->elts != NULL);
   return sp;
}

// Free memory for the set and its elements
// O(n)
void destroySet(SET *sp) {
   assert(sp != NULL);
   for (int i = 0; i < sp->count; i++)
       free(sp->elts[i]);
   free(sp->elts);
   free(sp);
}

// Return number of elements
// O(1): return a single value
int numElements(SET *sp) {
   assert(sp != NULL);
   return sp->count;
}

// Find an element, return it if found, shifts elements up to accomodate empty space
// O(n): shifting loop for shifting n elements
void addElement(SET *sp, char *elt) {
   assert(sp != NULL && elt != NULL);
   bool found;
   int index = search(sp, elt, &found);
   if (found) // already in set (cannot add)
       return;
   assert(sp->count < sp->length); // ensures shifting is possible with the max number of elts in the set
   for (int i = sp->count; i > index; i--) //shifting UP loop
       sp->elts[i] = sp->elts[i - 1];
   sp->elts[index] = strdup(elt);
   sp->count++;
}

// Find an element, return it if found, shifts elements down to accomodate empty space
// O(n): shifting loop for shifting n elements
void removeElement(SET *sp, char *elt) {
   assert(sp != NULL && elt != NULL);
   bool found;
   int index = search(sp, elt, &found);
   if (!found) // not in set (can't remove)
       return;
   free(sp->elts[index]);
   for (int i = index + 1; i < sp->count; i++) // shifting DOWN loop
       sp->elts[i - 1] = sp->elts[i];
   sp->count--;
}

// Add an element if not already present
// O(log n): Binary search
char *findElement(SET *sp, char *elt) {
   assert(sp != NULL && elt != NULL);
   bool found;
   int index = search(sp, elt, &found);
   if (found)
   return sp->elts[index];
   else
   return NULL;
}

// return an array of all words
// O(n): for loop for n elements
char **getElements(SET *sp) {
   assert(sp != NULL);
   char **array = malloc(sizeof(char *) * sp->count);
   assert(array != NULL);
   for (int i = 0; i < sp->count; i++){
    array[i] = sp->elts[i];
   }
   return array;
}