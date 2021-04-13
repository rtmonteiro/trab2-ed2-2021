//
// Created by Pichau on 16/02/2021.
//

#include <stdlib.h>
#include "list.h"

list *initList() {
    list *l = (list *) malloc(sizeof(list));
    l->start = NULL;
    l->end = NULL;
    return l;
}

list *insertNode(list *l, Item vertice) {
    node *new = (node *) malloc(sizeof(node));
    new->next = NULL;
    new->vertice = vertice;
    if (isEmpty(l)) l->start = l->end = new;
    else {
        l->end->next = new;
        l->end = new;
    }
    return l;
}

void deleteNode(list *l) {
    node *p = l->end;
    l->end = l->end->next;
    p->next = NULL;
    free(p);
}

int isEmpty(list *list) {
    return !list->start;
}

