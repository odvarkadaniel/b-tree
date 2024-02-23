#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "btree.h"

static size_t btree_fit_size(size_t item_sz)
{
    size_t size = (sizeof(btree));
    size += item_sz;

    return size;
}

btree *btree_new(size_t item_sz, size_t max_items, int (*comparator)(void *, void *))
{
    size_t size = btree_fit_size(item_sz);
    btree *btree = malloc(size);
    if (!btree)
    {
        return NULL;
    }

    btree->count = 0;
    btree->height = 0;
    btree->max_items = max_items;
    btree->min_items = max_items / 2;
    btree->comparator = comparator;

    return btree;
}

void btree_free(btree *btree)
{
    // free(btree->root);
    free(btree);
}
