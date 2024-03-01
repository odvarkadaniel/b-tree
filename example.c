#include <stdio.h>
#include <string.h>

#include "btree.h"

typedef struct
{
    int id;
    char *name;
    char *email;
} user;

int cmp(const void *a, const void *b)
{
    const user *u1 = a;
    const user *u2 = b;

    return u1->id - u2->id;
}

int main()
{
    btree *btree = btree_new(sizeof(user), 3, cmp);

    btree_insert(btree, &(user){.id = 1, .name = "daniel1", .email = "email111@email111.com"});
    btree_insert(btree, &(user){.id = 2, .name = "daniel22", .email = "email12@email12.com"});
    btree_insert(btree, &(user){.id = 4, .name = "daniel15", .email = "email15@email15.com"});

    // This procs the split of the root node.
    btree_insert(btree, &(user){.id = 3, .name = "daniel333", .email = "email321@email15.com"});

    // bnode *current = btree->root;
    // const user *us;
    // const user *us1;

    // for (size_t i = 0; i < current->nitems; i++)
    // {
    //     us = (user *)current->items + sizeof(user) * i;
    //     if (!us)
    //     {
    //         printf("NULL\n");
    //         exit(EXIT_FAILURE);
    //     }

    //     printf("Item: %d - %s - %s\n", us->id, us->name, us->email);
    //     bnode *ch = current->children[0];
    //     if (!ch)
    //     {
    //         printf("CHILD IS NULL\n");
    //         exit(EXIT_FAILURE);
    //     }
    //     printf("Child: %d -- %d\n", ch->leaf, ch->nitems);
    //     us1 = (user *)ch->items + sizeof(user) * 0;
    //     printf("Item: %d - %s - %s\n", us1->id, us1->name, us1->email);
    // }

    const user *us1;
    us1 = btree_get(btree, &(user){.id = 1});
    printf("Root: %d - %s - %s\n", us1->id, us1->name, us1->email);

    const user *us2;
    us2 = btree_get(btree, &(user){.id = 2});
    printf("Root: %d - %s - %s\n", us2->id, us2->name, us2->email);

    const user *us3;
    us3 = btree_get(btree, &(user){.id = 3});
    printf("Root: %d - %s - %s\n", us3->id, us3->name, us3->email);

    const user *us4;
    us4 = btree_get(btree, &(user){.id = 4});
    printf("Root: %d - %s - %s\n", us4->id, us4->name, us4->email);

    btree_free(btree);
}