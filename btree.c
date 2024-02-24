#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "btree.h"

btree *btree_new(size_t item_sz, size_t max_items, int (*comparator)(const void *, const void *))
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
    btree->root = NULL;

    return btree;
}

static size_t btree_fit_size(size_t item_sz)
{
    size_t size = (sizeof(btree));
    size += item_sz;
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

    memset(node, 0, size);
    node->leaf = leaf;
    node->items = (char *)node + sizeof(bnode);

    return node;
}

void btree_free(btree *btree)
{
    free(btree->root);
    free(btree);
}

static void btree_set_item_at(btree *btree, bnode *node,
                              size_t index, const void *item)
{
    void *slot = btree_get_item_at(btree, btree->root, index);
    memcpy(slot, item, btree->item_sz);
}

const void *btree_insert(btree *btree, const void *item)
{
    return btree_insert_int(btree, item);
}

static void *btree_insert_int(btree *btree, const void *item)
{
    if (!btree->root)
    {
        btree->root = btree_new_node(btree, true);
        if (!btree->root)
        {
            return NULL;
        }

        btree_set_item_at(btree, btree->root, 0, item);
        btree->root->nitems = 1;
        btree->count++;
        btree->height++;

        return NULL;
    }

    // TODO: In this case:
    // 1. We have atleast one node in the tree
    // 1a. With this insertion, we might need to
    //     either split the node, or we can just insert.
    // 2. Split the node if needed -> rotate

    // Start looking from the root.
    btree_result result = btree_insert_result(btree, btree->root, item, 0);
    switch (result)
    {
    case BTREE_INSERTED:
        btree->count++;
        return NULL;
    default:
        printf("Failure on result: Should not happen.\n");
        exit(EXIT_FAILURE);
    }

    return NULL;
}

static btree_result btree_insert_result(btree *btree, bnode *node, const void *item, int depth)
{
    bool found = false;
    size_t index = btree_search(btree, node, item, depth, &found);

    if (!found)
    {
        if (node->nitems == btree->max_items)
        {
            return BTREE_SPLIT_NEEDED;
        }

        // TODO: Shift the items in the node.
        btree_shift_items(btree, node, index);
        btree_set_item_at(btree, node, index, item);
        node->nitems++;

        return BTREE_INSERTED;
    }

    printf("I should not get here yet...");
    exit(EXIT_FAILURE);
}

static void btree_shift_items(btree *btree, bnode *node, size_t index)
{
    size_t to_shift = node->nitems - index;
    memmove(node->items + btree->item_sz * (index + 1), node->items + btree->item_sz * index, to_shift * btree->item_sz);
}

static size_t btree_search(btree *btree, bnode *node, const void *item, int depth, bool *found)
{
    // *found = true;
    size_t index = 0;
    size_t nitems = node->nitems;

    while (index < nitems)
    {
        size_t sep = (index + nitems) / 2;
        void *node_it = btree_get_item_at((void *)btree, node, sep);

        int cmp = btree->comparator((void *)item, node_it);
        if (cmp < 0) // new item is smaller
        {
            nitems = sep;
        }
        else if (cmp > 0)
        {
            index = sep + 1;
        }
        else
        {
            *found = true;
            return sep;
        }
    }

    *found = false;

    return index;
}

static void *btree_get_item_at(btree *btree, bnode *node, size_t index)
{
    return node->items + btree->item_sz * index;
}

const void *btree_get(const btree *btree, const void *key, size_t index)
{
    return btree_get_item_at((void *)btree, btree->root, index);
}
