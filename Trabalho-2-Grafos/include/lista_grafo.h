// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#ifndef LISTA_GRAFO
#define LISTA_GRAFO

typedef struct no no;
// definicao da lista
struct lista {
    no *ini;
    int tam;
};
typedef struct lista lista;

#include "grafo.h"
#include <stdlib.h>
#include <string.h>

// define os nos da lista
struct no {
    aresta_grafo *info;
    struct no *prox;
};

// funcoes de lista a serem empregadas

void inicializa_lista(lista *l);
void destruir(lista *l);
void inserir(lista *l, aresta_grafo *aresta);

#endif