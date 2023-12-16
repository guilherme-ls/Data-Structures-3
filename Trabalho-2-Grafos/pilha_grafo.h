// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#ifndef PILHA_GRAFO
#define PILHA_GRAFO

#include "grafo.h"

typedef struct no_pilha no_pilha;

// Definicao da pilha
struct pilha{
    no_pilha *topo;
    int tam;
};
typedef struct pilha pilha;

// Definindo no da pilha
struct no_pilha{
    vertice_grafo *vertice;
    struct no_pilha *prox;
}

// Funcoes da pilha a serem empregadas

void inicializa_pilha(pilha *p);
void destroi_pilha(pilha *p);
void push(pilha *p, vertice_grafo *vertice);
vertice_grafo *pop(pilha *p);





#endif