#include <stdio.h>
#include <limits.h>
#include "pq/item.h"
#include "pq/PQ.h"

Item make_item(int id, double value) {
    Item t;
    id(t) = id;
    value(t) = value;
    return t;
}

int main1() {

    // inicializando a PQ
    PQ_init(10);

    // inserindo alguns elementos na PQ
    PQ_insert(make_item(1, 10.0));
    PQ_insert(make_item(6, 5.0));
    PQ_insert(make_item(3, 3.0));
    PQ_insert(make_item(4, 4.0));
    PQ_insert(make_item(7, 7.0));
    PQ_insert(make_item(2, 1.0));

    // alterando a prioridade de alguns elementos
    PQ_decrease_key(4, 0.5); // agora o nó 4 tem valor 0.5, sua posição no heap deve mudar
    PQ_decrease_key(6, 0.1); // agora o nó 6 tem valor 0.1, sua posição no heap deve mudar

    // removendo o menor elemento e imprimindo
    while (!PQ_empty()) {
        Item p = PQ_delmin();
        printf("Identificador %d, prioridade %lf\n", id(p), value(p));
    }
}

int main(int argc, char* argv[]) {

    FILE *f = fopen(argv[1], "r");

    int V, E;
    fscanf(f, "%d %d%c", &V, &E, NULL);

    int S, C, M;
    fscanf(f, "%d %d %d%c", &S, &C, &M, NULL);

    int id_S[S], id_C[C], id_M[M];

    int i;
    for (i = 0; i < S; ++i) {
        fscanf(f, "%d%c", &id_S[i], NULL);
    }
    for (i = 0; i < C; ++i) {
        fscanf(f, "%d%c", &id_C[i], NULL);
    }
    for (i = 0; i < M; ++i) {
        fscanf(f, "%d%c", &id_M[i], NULL);
    }

    int id_Nos[V];

    for (i = 0; i < E; ++i) {
        // TODO Ler os vértices e armazenar na lista de adjacência
    }

}