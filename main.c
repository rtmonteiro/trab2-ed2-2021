#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "pq/item.h"
#include "pq/PQ.h"
#include "list.h"

typedef struct Inflacao {
      int id_S;
      int id_C;
      double infl;
 } inflacao;

inflacao *make_inflacao(double infl, int s, int c);

void calculaInflacoes(inflacao **vecInflacao,
                      int S, int C, int M,
                      double RTT_SM[S][M], double RTT_CM[C][M], double RTT_SC[S][C],
                      int *id_S, int *id_C);

Item make_item(int id, double value) {
    Item t;
    id(t) = id;
    value(t) = value;
    return t;
}

int compareDistancia (const void *a, const void *b);

double *dixcasca(int s, int V, list *grafo[V]);

//int main1() {
//
//    // inicializando a PQ
//    PQ_init(10);
//
//    // inserindo alguns elementos na PQ
//    PQ_insert(make_item(1, 10.0));
//    PQ_insert(make_item(6, 5.0));
//    PQ_insert(make_item(3, 3.0));
//    PQ_insert(make_item(4, 4.0));
//    PQ_insert(make_item(7, 7.0));
//    PQ_insert(make_item(2, 1.0));
//
//    // alterando a prioridade de alguns elementos
//    PQ_decrease_key(4, 0.5); // agora o nó 4 tem valor 0.5, sua posição no heap deve mudar
//    PQ_decrease_key(6, 0.1); // agora o nó 6 tem valor 0.1, sua posição no heap deve mudar
//
//    // removendo o menor elemento e imprimindo
//    while (!PQ_empty()) {
//        Item p = PQ_delmin();
//        printf("Identificador %d, prioridade %lf\n", id(p), value(p));
//    }
//
//    return 0;
//}

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

    list **id_Nos = (list **) malloc(sizeof(list *) * V);

    for (i = 0; i < E; ++i) {
        int id, v;
        double weight;
        fscanf(f, "%d %d %lf", &id, &v, &weight);
        if (id_Nos[id] == NULL) id_Nos[id] = initList();
        id_Nos[id] = insertNode(id_Nos[id], make_item(v, weight));
    }

    double RTT_SC[S][C];
    double RTT_SM[S][M];
    double RTT_CM[C][M];

    // RTT(a, b) = 𝛿(a, b) + 𝛿(b, a)

    // RTT_aprox(a, b) = min(RTT(a, m) + RTT(m, b))
    
    int j;

    for(j = 0; j < S; ++j){
        double *dist_min = dixcasca(id_S[j], V, id_Nos);
        
        //𝛿(S->C)
        for(i = 0; i < C; ++i){
            RTT_SC[j][i] = dist_min[id_C[i]];
        }

        //𝛿(S->M)
        for(i = 0; i < M; ++i){
            RTT_SM[j][i] = dist_min[id_M[i]];
        }
    }


    for(j = 0; j < C; ++j){
        double *dist_min = dixcasca(id_C[j], V, id_Nos);
        
        //𝛿(C->S)
        for(i = 0; i < S; ++i){
            RTT_SC[i][j] += dist_min[id_S[i]];
        }

        //𝛿(C->M)
        for(i = 0; i < M; ++i){
            RTT_CM[j][i] = dist_min[id_M[i]];
        }

    }

    for(j = 0; j < M; ++j){
        double *dist_min = dixcasca(id_M[j], V, id_Nos);
        
        //𝛿(M->S)
        for(i = 0; i < S; ++i){
            RTT_SM[i][j] += dist_min[id_S[i]];
        }

        //𝛿(M->C)
        for(i = 0; i < C; ++i){
            RTT_CM[j][i] += dist_min[id_C[i]];
        }

    }

    inflacao** vecInflacao = (inflacao**)malloc(sizeof(inflacao*)*S*C);
    
    calculaInflacoes(vecInflacao, S, C, M, RTT_SM, RTT_CM, RTT_SC, id_S, id_C);
    
    qsort(vecInflacao, S*C, sizeof(inflacao*), compareDistancia);
    

// TODO Algoritmo dijkstra
// TODO Ordenar o vetor de inflações
// TODO Imprimir os valores ordenados de inflações
    return 0;
}

void calculaInflacoes(inflacao **vecInflacao,
                      int S, int C, int M,
                      double RTT_SM[S][M], double RTT_CM[C][M], double RTT_SC[S][C],
                      int *id_S, int *id_C){
    int i, j, k;
    double distancia, rtt_prox;
    for(i = 0; i < S; ++i){
        for(j = 0; j < C; ++j){
            rtt_prox = INT_MAX;
            for(k = 0; k < M; ++k){
                distancia = RTT_SM[i][k] + RTT_CM[j][k];
                if(distancia < rtt_prox) rtt_prox = distancia;
            }
            vecInflacao[i + (j * S)] = make_inflacao(rtt_prox / RTT_SC[i][j], id_S[i], id_C[j]);
        }
    }
}


// S = 2, C = 5
//   01234 j
// 0 abcde
// 1 fghij
// i

// a = [0][0] = 0 = 0 + 0 * 2
// f = [1][0] = 1 = 1 + 0 * 2
// c = [0][2] = 4 = 0 + 2 * 2
// h = [1][2] = 5 = 1 + 2 * 2
// x = [i][j] = pos = i + j * S
// 0123456789
// afbgchdiej

inflacao *make_inflacao(double infl, int s, int c) {
    inflacao *new = (inflacao *) malloc(sizeof(inflacao));
    new->id_S = s;
    new->id_C = c;
    new->infl = infl;
    return new;
}

 int compareDistancia (const void *a, const void *b) {	
    const inflacao **a1 = (const inflacao **) a;
    const inflacao **a2 = (const inflacao **) b;
    if ( (*a1)->infl == (*a2)->infl ){
        return 0;
    }
    else if ( (*a1)->infl < (*a2)->infl ){
        return -1;
    }else 
        return 1;
}

double *dixcasca(int s, int V, list **grafo) {
    double *dist_min = (double *) malloc(sizeof(double) * V);

    dist_min[s] = 0;

    PQ_Struct *pq_struct = PQ_init(V);

    for (int i = 0; i < V; ++i) {
        if (i != s) dist_min[i] = INT_MAX;

        PQ_insert(make_item(i, INT_MAX), pq_struct);
    }

    while (!PQ_empty(pq_struct)) {
        Item u = PQ_delmin(pq_struct);
        for (node *p = grafo[u.id]->start; p != NULL; p = p->next) {
            double dist = dist_min[u.id] + p->vertice.value;
            if (dist < dist_min[p->vertice.id]) {
                dist_min[s] = dist;
                PQ_decrease_key(p->vertice.id, dist, pq_struct);
            }
        }
    }
    return dist_min;
}

/*
1 function Dijkstra(Graph, source):
2      dist[source] ← 0                           // Initialization
3
4      create vertex priority queue Q
5
6      for each vertex v in Graph:
7          if v ≠ source
8              dist[v] ← INFINITY                 // Unknown distance from source to v
9              prev[v] ← UNDEFINED                // Predecessor of v
10
11         Q.add_with_priority(v, dist[v])
12
13
14     while Q is not empty:                      // The main loop
15         u ← Q.extract_min()                    // Remove and return best vertex
16         for each neighbor v of u:              // only v that are still in Q
17             alt ← dist[u] + length(u, v)
18             if alt < dist[v]
19                 dist[v] ← alt
20                 prev[v] ← u
21                 Q.decrease_priority(v, alt)
22
23     return dist, prev
 */
 