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

struct btree *btree_new(size_t item_sz, size_t max_items, int (*comparator)(const void *, const void *));

static size_t btree_fit_size(size_t item_sz);

static struct bnode *btree_new_node(struct btree *btree, bool leaf);

static void btree_free_int(struct bnode *node);

void btree_free(struct btree *btree);

static void *btree_get_item_at(struct btree *btree, struct bnode *node, size_t index);

const void *btree_insert(struct btree *btree, const void *item);

static void *btree_insert_int(struct btree *btree, const void *item);

static enum btree_result btree_insert_result(struct btree *btree, struct bnode *node, const void *item, int depth);

static void btree_split(struct btree *btree, struct bnode *old_root, struct bnode **right, void **median);

static size_t btree_search(struct btree *btree, struct bnode *node, const void *item, int depth, bool *found);

static void btree_shift_forward(struct btree *btree, struct bnode *node, size_t index);

const void *btree_get(const struct btree *btree, const void *key);

static const void *btree_get_int(const struct btree *btree, const void *key);

const void *btree_remove(const struct btree *btree, const void *key);

static const void *btree_remove_int(const struct btree *btree, const void *key);

static enum btree_result btree_remove_result(struct btree *btree, struct bnode *node, const void *key, int depth);

static void btree_shift_backward(struct btree *btree, struct bnode *node, size_t index);

bool btree_has(const struct btree *btree, const void *key);

const void *btree_min(const struct btree *btree);

const void *btree_max(const struct btree *btree);

void btree_print(struct btree *btree);

#endif // BTREE_H
