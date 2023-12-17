// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#ifndef BUSCA_GRAFO
#define BUSCA_GRAFO

#include "lista_grafo.h"

#define INFINITO 1000000

/******** Funcoes para manipulacao do grafo ********/

void busca_tecnologias_entrada(grafo g, char* tecnologia);
void procedimentoBuscaDFS(grafo *g, vertice_grafo *vertice, int *pilha, int *indicePilha, int *cor);
void procedimentoBuscaDFSTransposto(grafo *g, vertice_grafo *verticeAtual, int *cor);
void ContaCFC(grafo *g, grafo *g_transposto);
int dijkstra(grafo g, char* tecnologiaInicial, char* tecnologiaFinal);

#endif