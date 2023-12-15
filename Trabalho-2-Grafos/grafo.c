// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#include "grafo.h"
#include <stdlib.h>
#include <string.h>

// Funcao para incializar o grafo
void inicializa_grafo(grafo *g, int num_tecnologias){
    g->num_vertices = 0;
    g->num_arestas = 0;
    g->lista_vertices = (vertice_grafo *) malloc(num_tecnologias * sizeof(vertice_grafo));
}

// Funcao para criar um vertice do grafo a partir do registro de dados
void cria_vertice(vertice_grafo *vetice, registro reg){
    vertice->nomeTecOrigem = (char*) malloc(80 * sizeof(char));
    vertice->nomeTecOrigem = reg.nomeTecnologiaOrigem;
    vertice->grupo = reg.grupo;
    vertice->grau_entrada = 0;
    vertice->grau_saida = 0;
    vertice->grau = 0; 
    vertice->aresta_head = NULL;
}   

// Funcao para criar uma aresta do grafo a partir do vertice de origem e do registro de dados
void cria_aresta(aresta_grafo *aresta, vertice_grafo *vertice, registro reg){
    aresta->tecDestino = vertice;
    aresta->prox = NULL;
    aresta->peso = reg.peso;
}



// Funcao para inserir aresta no grafo
void insere_aresta_no_grafo(grafo *g, registro reg){
    vertice_grafo *vertice;
    aresta_grafo *aresta;
    if(g->num_vertices == 0){
        // Primeira inserção, insere vertice e aresta direto
        
        // Cria o vertice a ser inserido no grafo
        cria_vertice(vertice, reg); 

        // Cria aresta a ser inserida no grafo
        cria_aresta(aresta, vertice, reg);

        // Adiciona aresta na lista de arestas do vertice
        vertice->aresta_head = aresta;

        // Adiciona vertice na lista de vertices do grafo
        g->lista_vertices[0] = vertice;

        // Incrementa numero de vertices e de arestas
        g->num_vertices++;
        g->num_arestas++;

        // Fim da funcao
        return;
    }

    // Loop que percorre vertices do grafo
    for(int i = 0; i < g->num_vertices; i++){
        if(strcmp(g->lista_vertices[0].nomeTecOrigem, reg.nomeTecnologiaOrigem) == 0){
            // Achou o vertice, insere aresta nele
            

        }
    }

}
