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
    struct btree *btree = btree_new(sizeof(user), 3, cmp);

    btree_insert(btree, &(user){.id = 1, .name = "daniel1", .email = "email111@email111.com"});
    btree_insert(btree, &(user){.id = 2, .name = "daniel22", .email = "email12@email12.com"});
    btree_insert(btree, &(user){.id = 3, .name = "daniel333", .email = "email321@email15.com"});

    // This procs the split of the root node.
    // btree_insert(btree, &(user){.id = 4, .name = "daniel15", .email = "email15@email15.com"});
    // btree_insert(btree, &(user){.id = 5, .name = "daniel5", .email = "email5@email15.com"});

    // This procs the split of a child node at index 1
    // Before:
    //          2
    //         / \
    //        1   3 | 4 | 5
    // After this, the tree should look like this:
    //          2 | 4
    //         / \   \
    //        1   3   5 | 6
    // btree_insert(btree, &(user){.id = 6, .name = "daniel6", .email = "email6@email15.com"});
    // btree_insert(btree, &(user){.id = 7, .name = "daniel7", .email = "email321@email15.com"});
    // btree_insert(btree, &(user){.id = 8, .name = "daniel8", .email = "email321@email15.com"});
    // btree_insert(btree, &(user){.id = 9, .name = "daniel9", .email = "email321@email15.com"});
    // btree_insert(btree, &(user){.id = 10, .name = "daniel10", .email = "email321@email15.com"});
    // btree_insert(btree, &(user){.id = 11, .name = "daniel10", .email = "email321@email15.com"});
    // btree_insert(btree, &(user){.id = 12, .name = "daniel10", .email = "email321@email15.com"});
    // btree_insert(btree, &(user){.id = 13, .name = "daniel10", .email = "email321@email15.com"});
    // btree_insert(btree, &(user){.id = 14, .name = "daniel10", .email = "email321@email15.com"});
    // btree_insert(btree, &(user){.id = 15, .name = "daniel10", .email = "email321@email15.com"});
    // btree_insert(btree, &(user){.id = 16, .name = "daniel10", .email = "email321@email15.com"});
    // btree_insert(btree, &(user){.id = 17, .name = "daniel10", .email = "email321@email15.com"});
    // btree_insert(btree, &(user){.id = 18, .name = "daniel10", .email = "email321@email15.com"});
    // btree_insert(btree, &(user){.id = 19, .name = "daniel10", .email = "email321@email15.com"});
    // btree_insert(btree, &(user){.id = 20, .name = "daniel10", .email = "email321@email15.com"});
    // btree_insert(btree, &(user){.id = 21, .name = "daniel10", .email = "email321@email15.com"});
    // btree_insert(btree, &(user){.id = 22, .name = "daniel10", .email = "email321@email15.com"});
    // btree_insert(btree, &(user){.id = 312, .name = "daniel10", .email = "email321@email15.com"});
    // btree_insert(btree, &(user){.id = 1423, .name = "daniel10", .email = "email321@email15.com"});
    // btree_insert(btree, &(user){.id = 123, .name = "daniel10", .email = "email321@email15.com"});
    // btree_insert(btree, &(user){.id = 12142, .name = "daniel10", .email = "email321@email15.com"});
    // btree_insert(btree, &(user){.id = 510, .name = "daniel10", .email = "email321@email15.com"});
    // btree_insert(btree, &(user){.id = 2410, .name = "daniel10", .email = "email321@email15.com"});
    // btree_insert(btree, &(user){.id = 8910, .name = "daniel10", .email = "email321@email15.com"});

    btree_remove(btree, &(user){.id = 1});

    const user *us1;
    us1 = btree_get(btree, &(user){.id = 1});
    if (!us1)
    {
        printf("Couldn't find us1\n");
    }
    else
    {
        printf("Root: %d - %s - %s\n", us1->id, us1->name, us1->email);
    }

    const user *us2;
    us2 = btree_get(btree, &(user){.id = 2});
    printf("Root: %d - %s - %s\n", us2->id, us2->name, us2->email);

    const user *us3;
    us3 = btree_get(btree, &(user){.id = 3});
    printf("Root: %d - %s - %s\n", us3->id, us3->name, us3->email);

    // const user *us4;
    // us4 = btree_get(btree, &(user){.id = 4});
    // printf("Root: %d - %s - %s\n", us4->id, us4->name, us4->email);

    // const user *us5;
    // us5 = btree_get(btree, &(user){.id = 5});
    // printf("Root: %d - %s - %s\n", us5->id, us5->name, us5->email);

    // const user *us6;
    // us6 = btree_get(btree, &(user){.id = 6});
    // printf("Root: %d - %s - %s\n", us6->id, us6->name, us6->email);

    // const user *us7;
    // us7 = btree_get(btree, &(user){.id = 7});
    // printf("Root: %d - %s - %s\n", us7->id, us7->name, us7->email);

    // const user *us8;
    // us8 = btree_get(btree, &(user){.id = 8});
    // printf("Root: %d - %s - %s\n", us8->id, us8->name, us8->email);

    // const user *us9;
    // us9 = btree_get(btree, &(user){.id = 9});
    // printf("Root: %d - %s - %s\n", us9->id, us9->name, us9->email);

    // const user *us0;
    // us0 = btree_get(btree, &(user){.id = 10});
    // printf("Root: %d - %s - %s\n", us0->id, us0->name, us0->email);

    // printf("Height: %lu\n", btree->height);

    // const user *max;
    // max = btree_max(btree);
    // printf("Max: %d - %s - %s\n", max->id, max->name, max->email);

    // const user *min;
    // min = btree_min(btree);
    // printf("Min: %d - %s - %s\n", min->id, min->name, min->email);

    btree_free(btree);
}