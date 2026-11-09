/*
Author: Saxon Meznarich
File name: huffman.c
Description: Creates a huffman tree by utlizing a priority queue containing the frequencies of
characters in a .txt file for file packing.
*/

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "pqueue.h"
#include "pack.h"

#define MAX_NODES 513 // 257(max leaves) + 256(max internal nodes) = 513(max nodes in huffman tree)

struct node *all_nodes[MAX_NODES];
int node_count = 0;

// creates a new node
struct node *mknode(int count, struct node *left_node, struct node *right_node)
{
    struct node *new_node = (struct node *)malloc(sizeof(struct node));
    new_node->count = count;
    new_node->parent = NULL;

    if (node_count < MAX_NODES)
    {
        all_nodes[node_count++] = new_node;
    }

    if (left_node != NULL)
    {
        left_node->parent = new_node;
    }
    if (right_node != NULL)
    {
        right_node->parent = new_node;
    }

    return new_node;
}

// compares two node count values, returns 1 if the count of node 1 is greater, 0 if counts
// are equal, and -1 if count is less than node 2
int cmp(struct node *t1, struct node *t2)
{
    return (t1->count < t2->count) ? -1 : (t1->count > t2->count);
}

// finds the depth of a specified node 'n' in the huffman tree recursively
int depth(struct node *n)
{
    if (n == NULL || n->parent == NULL)
        return 0;

    return 1 + depth(n->parent);
}

// prints the statistics for each ASCII value in the .txt file
// produces total bit counts
void print_bits(int *counts, struct node *nodes[257])
{
    int total_bits = 0;

    printf("Char\tCount\tDepth\t in theTotal Bits\n");

    for (int c = 0; c < 257; c++)
    {
        if (nodes[c] != NULL)
        {
            int d = depth(nodes[c]);
            int total = counts[c] * d;
            total_bits += total;

            if (c == 256)
            {
                printf("(400):\t%d\t%d\t%d\n", counts[c], d, total);
            }
            else if (c == 10)
            {
                printf("(012):\t%d\t%d\t%d\n", counts[c], d, total);
            }
            else if (isprint(c))
            {
                printf("'%c'\t%d\t%d\t%d\n", c, counts[c], d, total);
            }
            else
            {
                printf("\\%03o\t%d\t%d\t%d\n", c, counts[c], d, total);
            }
        }
    }
    printf("Total bits required = %d bits\n\n", total_bits);
}

// prepares huffman tree for pack function and then frees all variables
int main(int argc, char *argv[])
{
    FILE *fp;
    char *filename = argv[1];
    char *newfilename = argv[2];

    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <input_file_name> <output_file_name>\n", argv[0]);
        return 1;
    }

    struct stat cstate;
    if (stat(filename, &cstate) == 0)
    {
        if (S_ISDIR(cstate.st_mode))
        {
            fprintf(stderr, "This is a directory: %s \n", filename);
            return 1;
        }
    }

    fp = fopen(filename, "r");

    int counts[257];
    struct node *nodes[257];

    for (int i = 0; i < 257; i++)
    {
        counts[i] = 0;
        nodes[i] = NULL;
    }

    if (fp == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    int ch;

    while ((ch = getc(fp)) != EOF)
    {
        counts[ch]++;
    }

    fclose(fp);

    PQ *pq = createQueue(cmp);
    // EOF Node initialization into priority queue
    struct node *new_node = mknode(0, NULL, NULL);
    nodes[256] = new_node;
    addEntry(pq, new_node);

    for (int i = 255; i >= 0; i--)
    {
        if (counts[i] > 0)
        {
            struct node *new_node = mknode(counts[i], NULL, NULL);
            nodes[i] = new_node;
            addEntry(pq, new_node);
        }
    }

    while (numEntries(pq) > 1)
    {
        struct node *left_node = (struct node *)removeEntry(pq);
        struct node *right_node = (struct node *)removeEntry(pq);
        struct node *parent = mknode(left_node->count + right_node->count, left_node, right_node);
        addEntry(pq, parent);
    }

    print_bits(counts, nodes);

    pack(filename, newfilename, nodes);

    destroyQueue(pq);

    for (int i = 0; i < node_count; i++)
    {
        free(all_nodes[i]);
    }

    return 0;
}