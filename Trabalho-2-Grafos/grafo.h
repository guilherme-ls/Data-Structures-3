// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#ifndef GRAFO
#define GRAFO

#include "registros_dados.h"

// Definir estrutura para areasta do grafo
struct aresta_grafo {
    struct vertice_grafo *tecDestino;
    struct aresta_grafo *prox;
    int peso;
};
typedef struct aresta_grafo aresta_grafo;

// Definir estrutura para vertice do grafo
struct vertice_grafo {
    char *nomeTecOrigem;
    int grupo;
    int grau_entrada;
    int grau_saida;
    int grau;
    struct aresta_grafo *aresta_head;
};
typedef struct vertice_grafo vertice_grafo;

// Definir estrutura para grafo de tecnologias
struct grafo {
    int num_vertices;
    int num_arestas;
    struct vertice_grafo *lista_vertices;
};
typedef struct grafo grafo;

/******** Funcoes para manipulacao do grafo ********/
void inicializa_grafo(grafo *g, int num_tecnologias);
void cria_vertice(vertice_grafo *vertice, registro reg);
void cria_aresta(aresta_grafo *aresta, vertice_grafo *vertice, registro reg);
void insere_aresta_no_grafo(grafo *g, registro reg);



#endif