//
// Created by Pichau on 16/02/2021.
//

#ifndef PROBLEMA_DE_JOSEFO_LIST_H
#define PROBLEMA_DE_JOSEFO_LIST_H

#include "pq/item.h"

typedef struct List {
    struct Node *start;
    struct Node *end;
} list;

typedef struct Node {
    Item vertice;
    struct Node *next;
} node;

list *initList();

list *insertNode(list *l, Item vertice);

void deleteNode(list *l);

int isEmpty(list *list);



#endif //PROBLEMA_DE_JOSEFO_LIST_H
