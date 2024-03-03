#define BTREE_H
#ifdef BTREE_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct bnode
{
    bool leaf;
    int nitems;
    char *items;
    struct bnode *children[];
};

struct btree
{
    // We don't care about these custom allocators for now.
    void *(*malloc)(size_t);
    void *(*remalloc)(size_t);
    void *(*free)(void *);

    int (*comparator)(const void *, const void *);

    void (*formatter)(const struct btree *);

    size_t count;       // number of items in the tree
    size_t height;      // height of the tree
    struct bnode *root; // root of the tree
    size_t item_sz;     // size of the user item
    size_t max_items;   // max items allowed per node before splitting
    size_t min_items;   // min items allowed per node to join
};

enum btree_result
{
    BTREE_INSERTED,
    BTREE_SPLIT_NEEDED,
    BTREE_REPLACED_ITEM,
    BTREE_REMOVED,
    BTREE_REBALANCE_NEEDED,
};

// btree_new creates a new instance of a b-tree structure.
struct btree *btree_new(size_t item_sz, size_t max_items, int (*comparator)(const void *, const void *));

// btree_free frees the whole b-tree structure from memory.
void btree_free(struct btree *btree);

// btree_insert either inserts or replaces an item in the b-tree.
// If an item gets replaced, we return the old item.
// If an item gets inserted, we return NULL, if everything goes well.
const void *btree_insert(struct btree *btree, const void *item);

// btree_get tries to find an item by a key.
// If an item is found, we return the item.
// If an item doesn't get found, we return NULL.
const void *btree_get(const struct btree *btree, const void *key);

// btree_remove removes an item from the b-tree.
// If an item is removed successfully, we return NULL.
// TODO: Change this if we move forward with returning the old item on succession.
const void *btree_remove(const struct btree *btree, const void *key);

// btree_has looks for a key in the b-tree.
// If an item is found, we return true, otherwise false.
bool btree_has(const struct btree *btree, const void *key);

// btree_min finds the smallest item in the b-tree.
const void *btree_min(const struct btree *btree);

// btree_max finds the biggest item in the b-tree.
const void *btree_max(const struct btree *btree);

// btree_print outputs the whole b-tree structure to stdout.
// TODO: This or formatter?
void btree_print(struct btree *btree);

#endif // BTREE_H
