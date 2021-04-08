//
// Created by Pichau on 16/02/2021.
//

#include <stdlib.h>
#include "list.h"

node *initList(int v) {
    node *a;
    a = (node *) malloc(sizeof(node));
    a->next = a;
    a->key = v;
    return a;
}

node *insertNextNode(node *t, int v) {
    node *new = (node *) malloc(sizeof(node));
    new->next = NULL;
    t->next = new;
    new->key = v;
    return new;
}

void deleteNextNode(node *t) {
    node *p = t->next;
    t->next = t->next->next;
    p->next = NULL;
    free(p);
}

