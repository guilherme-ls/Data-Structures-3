// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#include "pilha_grafo.h"

// Funcao para incializar pilha
void inicializa_pilha(pilha *p){
    p->topo = NULL;
    p->tam = 0;
}

// Funcao para destruir pilha
void destroi_pilha(pilha *p){
    if(p != NULL){
        no_pilha *tp = p->topo;

        while(tp != NULL){
            p->topo = tp->prox;
            free(tp->info);
            free(tp);
            tp = p->topo;
        }
    }
}

// Funcao para empilhar vertice
void push(pilha *p, vertice_grafo *vertice){
    if(p == NULL || vertice == NULL){
        return;
    }
    no_pilha *no = malloc(siseof(no_pilha));

    no->vertice = vertice;
    no->prox = p->topo;

    p->topo = no;
    p->tam++;
}

// Funcao para desempilhar vertice
vertice_grafo *pop(pilha *p){
    if(p == NULL || p->tam == 0){
        return NULL;
    }
    vertice_grafo *vertice_topo = p->topo->vertice;
    p->topo = p->topo->prox;
    p->tam--;
    
    return vertice_topo;
}




