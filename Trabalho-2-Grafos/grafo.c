// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#include <stdlib.h>
#include <string.h>
#include "grafo.h"

// Funcao para incializar o grafo
void inicializa_grafo(grafo *g, int num_tecnologias) {
    g->num_vertices = 0;
    g->num_arestas = 0;
    g->lista_vertices = (vertice_grafo**) malloc((num_tecnologias + 1) * sizeof(vertice_grafo*));
}

// Funcao para criar um vertice do grafo a partir do registro de dados
void cria_vertice(vertice_grafo *vertice, registro reg) {
    vertice->nomeTecOrigem = (char*) malloc((reg.tecnologiaOrigem.tamanho + 1) * sizeof(char));
    strcpy(vertice->nomeTecOrigem, reg.tecnologiaOrigem.nome);
    vertice->grupo = reg.grupo;
    vertice->grau_entrada = 0;
    vertice->grau_saida = 0;
    vertice->grau = 0;
    inicializa_lista(&(vertice->lista_arestas));
}

// Funcao para criar um vertice do grafo a partir do registro de dados
void cria_vertice_destino(vertice_grafo *vertice, registro reg) {
    vertice->nomeTecOrigem = (char*) malloc((reg.tecnologiaDestino.tamanho + 1) * sizeof(char));
    strcpy(vertice->nomeTecOrigem, reg.tecnologiaDestino.nome);
    vertice->grupo = 0;
    vertice->grau_entrada = 0;
    vertice->grau_saida = 0;
    vertice->grau = 0;
    inicializa_lista(&(vertice->lista_arestas));
}

// Funcao para criar uma aresta do grafo a partir do vertice de origem e do registro de dados
void cria_aresta(aresta_grafo *aresta, vertice_grafo *vertice_destino, int peso) {
    aresta->tecDestino = vertice_destino;
    aresta->peso = peso;
}

void insere_aresta_vertice(vertice_grafo *vertice, aresta_grafo *aresta) {
    inserir(&(vertice->lista_arestas), aresta);
    vertice->grau_saida++;
    vertice->grau++;
    aresta->tecDestino->grau++;
    aresta->tecDestino->grau_entrada++;
}

int busca_binaria_vertices(grafo g, char* nomeTec) {
    // variaveis de apoio
    int menor = 0;
    int maior = g.num_vertices - 1;
    int meio = 0;

    while (menor <= maior) {
        // calculo da posicao do meio
        meio = menor + (maior - menor) / 2;

        if (strcmp(nomeTec, g.lista_vertices[meio]->nomeTecOrigem) == 0) {
            // Encontrou chave
            return meio;
        }
        else if (strcmp(nomeTec, g.lista_vertices[meio]->nomeTecOrigem) > 0) {
            menor = meio + 1;
        }
        else {
            maior = meio - 1;
        }
    }

    // retorna posicao onde chave foi encontrada (ou deveria ser inserida)
    return menor;
}

void insere_vertice(grafo *g, vertice_grafo *vertice, int pos) {
    for(int i = g->num_vertices; i > pos; i--) {
        g->lista_vertices[i] = g->lista_vertices[i-1];
    }
    g->lista_vertices[pos] = vertice;
    g->num_vertices++;
}

// Funcao para inserir aresta no grafo
void insere_aresta(grafo *g, registro reg) {
    int pos;
    vertice_grafo *verticeOrigem = malloc(sizeof(vertice_grafo));
    vertice_grafo *verticeDestino = malloc(sizeof(vertice_grafo));
    aresta_grafo *aresta = malloc(sizeof(aresta_grafo));

    // Primeira inserção, insere vertice e aresta direto
    if(g->num_vertices == 0) {
        // Cria o vertice a ser inserido no grafo
        cria_vertice(verticeOrigem, reg);
        cria_vertice_destino(verticeDestino, reg);
        insere_vertice(g, verticeOrigem, 0);
        pos = busca_binaria_vertices(*g, reg.tecnologiaDestino.nome);
        insere_vertice(g, verticeDestino, pos);

        // Cria aresta a ser inserida no grafo
        cria_aresta(aresta, verticeDestino, reg.peso);

        // Adiciona aresta na lista de arestas do vertice
        insere_aresta_vertice(verticeOrigem, aresta);
        g->num_arestas++;
    }
    else {
        // checa se o vertice origem ja existe na lista de adjacencias, criando e inserindo caso necessario
        pos = busca_binaria_vertices(*g, reg.tecnologiaOrigem.nome);
        if(pos < g->num_vertices && strcmp(g->lista_vertices[pos]->nomeTecOrigem, reg.tecnologiaOrigem.nome) == 0) {
            free(verticeOrigem);
            verticeOrigem = g->lista_vertices[pos];
            verticeOrigem->grupo = reg.grupo;
        }
        else {
            cria_vertice(verticeOrigem, reg);
            insere_vertice(g, verticeOrigem, pos);
        }

        // checa se o vertice destino ja existe na lista de adjacencias, criando e inserindo caso necessario
        pos = busca_binaria_vertices(*g, reg.tecnologiaDestino.nome);
        if(pos < g->num_vertices && strcmp(g->lista_vertices[pos]->nomeTecOrigem, reg.tecnologiaDestino.nome) == 0) {
            free(verticeDestino);
            verticeDestino = g->lista_vertices[pos];
        }
        else {
            cria_vertice_destino(verticeDestino, reg);
            insere_vertice(g, verticeDestino, pos);
        }

        // cria aresta a ser inserida
        cria_aresta(aresta, verticeDestino, reg.peso);
        
        // insere aresta criada
        insere_aresta_vertice(verticeOrigem, aresta);
        g->num_arestas++;
    }
}
