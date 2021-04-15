#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PQ.h"

static void swap(int i, int j, PQ_Struct *pq_struct) {
    exch(pq_struct->pq[i], pq_struct->pq[j]);
    pq_struct->map[id(pq_struct->pq[i])] = i;
    pq_struct->map[id(pq_struct->pq[j])] = j;
}

void fix_up(Item *a, int k, PQ_Struct *pq_struct) {  
    while (k > 1 && more(a[k/2], a[k])) {
        swap(k, k/2, pq_struct);
        k = k/2;
    }
}

void fix_down(PQ_Struct *pq_struct, int sz, int k){
  while (2*k <= sz) {
    int j = 2*k;
    if (j < sz && more(pq_struct->pq[j], pq_struct->pq[j + 1])){
      j++;
    }
    if (!more(pq_struct->pq[k], pq_struct->pq[j])) {
      break;
    }
    swap(k, j, pq_struct);
    k = j;
  }
}

PQ_Struct * PQ_init(int maxN) {
    PQ_Struct *new = (PQ_Struct *) malloc(sizeof(PQ_Struct));
    new->pq = (Item *) malloc((maxN+1) * sizeof (Item));
    new->map = (int *) malloc((maxN+1) * sizeof (int));
    for(int i = 0; i < maxN; i++) new->map[i] = -1;
    new->N = 0;

    return new;
}

void PQ_insert(Item v, PQ_Struct *pq_struct) {
    pq_struct->N++;
    pq_struct->pq[pq_struct->N] = v;
    pq_struct->map[id(v)] = pq_struct->N;
    fix_up(pq_struct->pq, pq_struct->N, pq_struct);
}

Item PQ_delmin(PQ_Struct *pq_struct) {
    Item min = pq_struct->pq[1];
    swap(1, pq_struct->N, pq_struct);
    pq_struct->N--;
    fix_down(pq_struct, pq_struct->N, 1);
    return min;
}

Item PQ_min(PQ_Struct *pq_struct) {
    return pq_struct->pq[1];
}

void PQ_decrease_key(int id, double value, PQ_Struct *pq_struct) {
    int i = pq_struct->map[id];
    value(pq_struct->pq[i]) = value;
    fix_up(pq_struct->pq, i, pq_struct);
}

bool PQ_empty(PQ_Struct *pq_struct) {
    return pq_struct->N == 0;
}

int PQ_size(PQ_Struct *pq_struct) {
    return pq_struct->N;
}

void PQ_finish(PQ_Struct *pq_struct) {
    free(pq_struct->pq);
    free(pq_struct->map);
}


