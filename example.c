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
    // user user = {.id = 10, .name = "danieol", .email = "email"};

    user *us = malloc(sizeof(user));
    us->id = 20;
    us->name = "daniel";
    us->email = "email@email.com";

    // size_t user_sz = sizeof(user);
    // printf("Sz: %lu - %d - %s - %s\n", user_sz, user->id, user->name, user->email);

    btree *btree = btree_new(sizeof(user), 3, cmp);
    // printf("SzTree: %lu\n", sizeof(*btree));

    btree_insert(btree, us);
    btree_insert(btree, &(user){.id = 10, .name = "daniel22", .email = "email12@email12.com"});
    btree_insert(btree, &(user){.id = 15, .name = "daniel15", .email = "email15@email15.com"});

    // This procs the split of the root node.
    // btree_insert(btree, &(user){.id = 25, .name = "daniel15", .email = "email15@email15.com"});

    const user *us1;
    us1 = btree_get(btree, &(user){.id = 20});

    const user *us2;
    us2 = btree_get(btree, &(user){.id = 10});

    const user *us3;
    us3 = btree_get(btree, &(user){.id = 15});

    printf("Root: %d - %s - %s\n", us1->id, us1->name, us1->email);
    printf("Root: %d - %s - %s\n", us2->id, us2->name, us2->email);
    printf("Root: %d - %s - %s\n", us3->id, us3->name, us3->email);

    btree_free(btree);
    free(us);
}