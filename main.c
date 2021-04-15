#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "item.h"
#include "PQ.h"
#include "list.h"

typedef struct Inflacao {
      int id_S;
      int id_C;
      double infl;
} inflacao;

inflacao *make_inflacao(double infl, int s, int c);

inflacao **calculaInflacoes(int S, int C, int M,
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

    fclose(f);


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
            RTT_CM[i][j] += dist_min[id_C[i]];
        }

    }

    inflacao **vecInflacao = calculaInflacoes(S, C, M, RTT_SM, RTT_CM, RTT_SC, id_S, id_C);

    FILE *out = fopen(argv[2], "w");

    for(int g = 0; g < S*C; g++){
        fprintf(out,"%d %d %.16lf\n", vecInflacao[g]->id_S, vecInflacao[g]->id_C, vecInflacao[g]->infl );
    }

    fclose(out);
    return 0;
}

inflacao **calculaInflacoes(int S, int C, int M, double RTT_SM[S][M], double RTT_CM[C][M], double RTT_SC[S][C], int *id_S, int *id_C) {

    inflacao** vecInflacao = (inflacao**) malloc(sizeof(inflacao*)*S*C);

    int i, j, k, count = 0;
    double distancia, rtt_prox;
    for(i = 0; i < S; ++i){
        for(j = 0; j < C; ++j){
            rtt_prox = INT_MAX;
            for(k = 0; k < M; ++k){
                distancia = RTT_SM[i][k] + RTT_CM[j][k];
                if(distancia < rtt_prox) rtt_prox = distancia;
            }
            vecInflacao[count++] = make_inflacao(rtt_prox / RTT_SC[i][j], id_S[i], id_C[j]);
        }
    }
    
    qsort(vecInflacao, S*C, sizeof(inflacao*), compareDistancia);

    return vecInflacao;
}

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
    PQ_insert(make_item(s, 0), pq_struct);

    for (int i = 0; i < V; ++i) {
        if (i != s) {
            dist_min[i] = INT_MAX;
            PQ_insert(make_item(i, INT_MAX), pq_struct);
        }
    }

    while (!PQ_empty(pq_struct)) {
        Item u = PQ_delmin(pq_struct);
        for (node *p = grafo[u.id]->start; p != NULL; p = p->next) {
            double dist = dist_min[u.id] + p->vertice.value;
            if (dist < dist_min[p->vertice.id]) {
                dist_min[p->vertice.id] = dist;
                PQ_decrease_key(p->vertice.id, dist, pq_struct);
            }
        }
    }

    return dist_min;
}
 