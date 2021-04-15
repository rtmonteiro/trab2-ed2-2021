#ifndef PQ_H
#define PQ_H

#include <stdbool.h>
#include "item.h"

typedef struct pqStruct {
    Item *pq;
    int *map;
    int N;
} PQ_Struct;

PQ_Struct * PQ_init(int maxN); // cria uma min PQ com capacidade maxN
void PQ_insert(Item, PQ_Struct *); // Insere Item na PQ. A maior prioridade é a do Item com menor campo 'value'
Item PQ_delmin(PQ_Struct *pq_struct);       // Remove Item com menor campo 'value' da PQ
void PQ_decrease_key(int id, double value, PQ_Struct *pq_struct); // Muda a prioridade do nó com identificador 'id' para 'value'
Item PQ_min(PQ_Struct *pq_struct);          // Retorna Item com menor campo 'value' da PQ
bool PQ_empty(PQ_Struct *pq_struct);        // Retorna True se a PQ não tem elementos
int PQ_size(PQ_Struct *pq_struct);         // Número de elementos na PQ
void PQ_finish(PQ_Struct *pq_struct);       // Libera memória

#endif
