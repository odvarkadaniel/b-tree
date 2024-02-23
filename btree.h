#define BTREE_H
#ifdef BTREE_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
    bool leaf;
    int nitems; // : 16; // or something else than 16
    char *items;
    struct bnode *children[];
} bnode;

typedef struct
{
    // We don't care about these custom allocators for now.
    void *(*malloc)(size_t);
    void *(*remalloc)(size_t);
    void *(*free)(void *);

    int (*comparator)(void *, void *);

    size_t count;     // number of items in the tree
    size_t height;    // height of the tree
    bnode *root;      // root of the tree
    size_t max_items; // max items allowed per node before splitting
    size_t min_items; // min items allowed per node to join

} btree;

btree *btree_new(size_t item_sz, size_t max_items, int (*comparator)(void *, void *));

static size_t btree_fit_size(size_t item_sz);

void btree_free(btree *btree);

void btree_clear(btree *btree);

const void *btree_min(const btree *btree);

const void *btree_max(const btree *btree);

void btree_print(btree *btree);

#endif // BTREE_H
