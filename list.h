//
// Created by Pichau on 16/02/2021.
//

#ifndef PROBLEMA_DE_JOSEFO_LIST_H
#define PROBLEMA_DE_JOSEFO_LIST_H

typedef struct vertex {
    int id;
    double peso;
} Vex;

typedef struct Node {
    Vex *vertice;
    struct Node *next;
} node;

node *initList(int v);

node *insertNextNode(node *t, int v);

void deleteNextNode(node *t);


#endif //PROBLEMA_DE_JOSEFO_LIST_H
