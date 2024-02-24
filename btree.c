#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "btree.h"

btree *btree_new(size_t item_sz, size_t max_items, int (*comparator)(void *, void *))
{
    size_t size = btree_fit_size(item_sz);
    btree *btree = malloc(size);
    if (!btree)
    {
        return NULL;
    }

    btree->item_sz = item_sz;
    btree->count = 0;
    btree->height = 0;
    btree->max_items = max_items;
    btree->min_items = max_items / 2;
    btree->comparator = comparator;

    return btree;
}

static size_t btree_fit_size(size_t item_sz)
{
    size_t size = (sizeof(btree));
    size += item_sz;
    printf("TREE: %lu\n", size);
    return size;
}

bnode *btree_new_node(btree *btree, bool leaf)
{
    size_t size = sizeof(bnode);
    size += btree->item_sz * btree->max_items;
    bnode *node = malloc(size);
    if (!node)
    {
        return NULL;
    }

    // memset(node, 0, size);

    node->leaf = leaf;
    node->items = (char *)node + size;

    return node;
}

void btree_free(btree *btree)
{
    // free(btree->root);
    free(btree);
}

static void btree_set_item_at(btree *btree, bnode *node,
                              size_t index, const void *item)
{
    void *slot = node->items + btree->item_sz * index;
    printf("here22\n");
    memcpy(slot, item, btree->item_sz);
}

const void *btree_insert(btree *btree, const void *item)
{
    if (!btree->root)
    {
        // printf("Sizebtree: %lu - %lu - %lu\n", sizeof(btree), btree->item_sz, btree->max_items);
        // btree->root = malloc(sizeof(bnode) + (btree->item_sz * btree->max_items));
        // if (!btree->root)
        // {
        //     printf("why\n");
        //     return NULL;
        // }
        btree->root = btree_new_node(btree, true);
        if (!btree->root)
        {
            printf("why\n");
            return NULL;
        }
        printf("here3\n");
        // TODO: set the item
        btree_set_item_at(btree, btree->root, 0, item);
        btree->root->nitems = 1;
        btree->count++;
        btree->height++;

        return NULL;
    }
}

static void *btree_get_item_at(btree *btree, bnode *node, size_t index)
{
    return node->items + btree->item_sz * index;
}

const void *btree_get(const btree *btree, const void *key)
{
    return btree_get_item_at(btree, btree->root, 0);
}
