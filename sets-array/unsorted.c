//Saxon Meznarich


#include <stdio.h> // using standard io functions
#include <stdlib.h> // memory management functions like malloc or free
#include <string.h> // string duplicate
#include <assert.h> // assert function
#include "set.h"

typedef struct set {
    int count;
    int length;
    char **elts;   
} SET;

// Returns index of element if found, otherwise -1
// O(n)
static int search(SET *sp, char *elt) {
    assert(sp != NULL && elt != NULL);
    for (int i = 0; i < sp->count; i++) {
        if (strcmp(sp->elts[i], elt) == 0) {
            return i; // found
        }
    }
    return -1;
}

// Create a new set with maxElts capacity
// O(1)
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
// O(n): frees all n elements
void destroySet(SET *sp) {
    assert(sp != NULL);
    for (int i = 0; i < sp->count; i++) {
        free(sp->elts[i]);
    }
    free(sp->elts);
    free(sp);
}

// Return number of elements
// O(1): returns 1 value
int numElements(SET *sp) {
    assert(sp != NULL);
    return sp->count;
}

// Add an element if not already present
// O(1): inserts into unsorted array, no shifting 
void addElement(SET *sp, char *elt) {
    assert(sp != NULL && elt != NULL);
    if (sp->count >= sp->length) {
        printf("Set is full!\n");
        return;
    }
    if (search(sp, elt) != -1) {
        return; // element exists already in array
    }
    sp->elts[sp->count] = strdup(elt);
    assert(sp->elts[sp->count] != NULL);
    sp->count++;
}

// Remove an element if present
// O(1): no shifting required to remove elements 
void removeElement(SET *sp, char *elt) {
    assert(sp != NULL && elt != NULL);
    int index = search(sp, elt);
    if (index == -1) {
        return;
    }
    free(sp->elts[index]);
    // move last element into the removed spot (O(1))
    sp->elts[index] = sp->elts[sp->count - 1];
    sp->count--;
}

// Find an element, return it if found, NULL if not
// O(n): requires search
char *findElement(SET *sp, char *elt) {
    assert(sp != NULL && elt != NULL);
    int index = search(sp, elt);
    if (index == -1){
        return NULL;
    }
    return sp->elts[index];
}
// Return an array of all elements
// O(n): assigning all n elements to array 
char **getElements(SET *sp) {
    assert(sp != NULL);
    char **array = malloc(sizeof(char *) * sp->count);
    assert(array != NULL);
    for (int i = 0; i < sp->count; i++) {
        array[i] = sp->elts[i];
    }
    return array;
}
