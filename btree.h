#define BTREE_H
#ifdef BTREE_H

#include <stdlib.h>

typedef struct
{
    bool leaf;
    int nitems; // : 16; // or something else than 16
    char *items;
    bnode *children[];
} bnode;

typedef struct
{
    void *(*malloc)(size_t);
    void *(*remalloc)(size_t);
    void *(*free)(void *);

    int (*comparator)(void *, void *);

    size_t order;     // number of items in the tree
    size_t height;    // height of the tree
    bnode *root;      // root of the tree
    size_t max_items; // max items allowed per node before splitting
    size_t min_items; // min items allowed per node to join

} btree;

#endif BTREE_H
