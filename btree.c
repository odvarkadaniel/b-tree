#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "btree.h"

struct btree *btree_new(size_t item_sz, size_t max_items, int (*comparator)(const void *, const void *))
{
    size_t size = btree_fit_size(item_sz);
    struct btree *btree = malloc(size);
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
    size_t size = (sizeof(struct btree));
    size += item_sz;
    return size;
}

struct bnode *btree_new_node(struct btree *btree, bool leaf)
{
    size_t size = sizeof(struct bnode);
    size += btree->item_sz * btree->max_items;

    if (!leaf)
    {
        size += sizeof(struct bnode *) * (btree->max_items + 1);
    }
    struct bnode *node = malloc(size);
    if (!node)
    {
        return NULL;
    }

    memset(node, 0, size);
    node->leaf = leaf;
    node->items = (char *)node + sizeof(struct bnode);

    return node;
}

void btree_free(struct btree *btree)
{
    free(btree->root);
    free(btree);
}

static void btree_set_item_at(struct btree *btree, struct bnode *node,
                              size_t index, const void *item)
{
    void *slot = btree_get_item_at(btree, node, index);
    memcpy(slot, item, btree->item_sz);
}

const void *btree_insert(struct btree *btree, const void *item)
{
    return btree_insert_int(btree, item);
}

typedef struct
{
    int id;
    char *name;
    char *email;
} user;

static void btree_split(struct btree *btree, struct bnode *old_root, struct bnode **right, void **median)
{
    *right = btree_new_node(btree, old_root->leaf);
    if (!*right)
    {
        return;
    }

    int mid_pos = (int)btree->max_items / 2;

    (*right)->nitems = old_root->nitems - (mid_pos + 1);
    printf("Moving: %lu\n", (size_t)(*right)->nitems * btree->item_sz);
    memmove((*right)->items, old_root->items + (int)btree->item_sz * (mid_pos + 1),
            (size_t)(*right)->nitems * btree->item_sz);

    *median = btree_get_item_at(btree, old_root, (size_t)mid_pos);

    old_root->nitems = mid_pos;
}

static void *btree_insert_int(struct btree *btree, const void *item)
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

    while (true)
    {
        btree_result result = btree_insert_result(btree, btree->root, item, 0);
        switch (result)
        {
        case BTREE_INSERTED:
            btree->count++;
            return NULL;
        case BTREE_REPLACED_ITEM:
            return NULL;
            // case BTREE_SPLIT_NEEDED:
            //     break;
        }

        struct bnode *old_root = btree->root;
        struct bnode *new_root = btree_new_node(btree, false);
        if (!new_root)
        {
            printf("error - should not happen alloc\n");
        }
        struct bnode *right = NULL;
        void *median = NULL;
        btree_split(btree, old_root, &right, &median);
        if (!right)
        {
            printf("jklasjdkasld\n");
            return NULL;
        }

        const user *root = (user *)median;
        printf("Root: %d - %s - %s\n", root->id, root->name, root->email);
        const user *left = (user *)old_root->items;
        printf("Left: %d - %s - %s\n", left->id, left->name, left->email);
        const user *r = (user *)right->items;
        printf("Right: %d - %s - %s\n", r->id, r->name, r->email);

        btree->root = new_root;
        btree->root->children[0] = old_root;
        btree_set_item_at(btree, btree->root, 0, median);
        btree->root->nitems = 1;
        btree->root->children[1] = right;
        btree->height++;

        // TODO: Remove the break
        // break;
    }

    return NULL;
}

static btree_result btree_insert_result(struct btree *btree, struct bnode *node, const void *item, int depth)
{
    bool found = false;
    size_t index = btree_search(btree, node, item, depth, &found);

    if (found)
    {
        // TODO: Actually replace the items.
        return BTREE_REPLACED_ITEM;
    }

    if (node->leaf)
    {
        if (node->nitems == btree->max_items)
        {
            return BTREE_SPLIT_NEEDED;
        }

        btree_shift_items(btree, node, index);
        btree_set_item_at(btree, node, index, item);
        node->nitems++;

        return BTREE_INSERTED;
    }

    btree_result result = btree_insert_result(btree, node->children[index], item, depth + 1);
    switch (result)
    {
    case BTREE_INSERTED:
        return result;
    default:
        break;
    }
    printf("Should be an insert above...\n");
    if (node->nitems == btree->max_items)
    {
        return BTREE_SPLIT_NEEDED;
    }

    printf("I should not get here yet...");
    exit(EXIT_FAILURE);
}

static void btree_shift_items(struct btree *btree, struct bnode *node, size_t index)
{
    size_t to_shift = node->nitems - index;
    memmove(node->items + btree->item_sz * (index + 1), node->items + btree->item_sz * index, to_shift * btree->item_sz);

    // TODO: What about this?
    // if (!node->leaf) {}
}

static size_t btree_search(struct btree *btree, struct bnode *node, const void *item, int depth, bool *found)
{
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

static void *btree_get_item_at(struct btree *btree, struct bnode *node, size_t index)
{
    return node->items + btree->item_sz * index;
}

const void *btree_get(const struct btree *btree, const void *key)
{
    return btree_get_int(btree, key);
}

const void *btree_get_int(const struct btree *btree, const void *key)
{
    struct bnode *current = btree->root;
    if (!current) // empty btree
    {
        return NULL;
    }

    bool found = false;
    int depth = 0;

    while (true)
    {
        size_t index = btree_search((void *)btree, current, key, depth, &found);
        if (found)
        {
            return btree_get_item_at((void *)btree, current, index);
        }

        if (current->leaf)
        {
            return NULL;
        }

        current = current->children[index];
        depth++;
    }
}
