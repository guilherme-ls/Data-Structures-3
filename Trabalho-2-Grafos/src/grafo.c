// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#include <stdlib.h>
#include <string.h>
#include "grafo.h"

/**
 * @brief funcao para a inicializacao do grafo com valores padrao
 * @param g ponteiro do grafo a ser inicializado
 * @param num_tecnologias numero de tecnologias contidas no grafo
 */
void inicializa_grafo(grafo *g, int num_tecnologias) {
    g->num_vertices = 0;
    g->num_arestas = 0;
    g->lista_vertices = (vertice_grafo**) malloc((num_tecnologias + 1) * sizeof(vertice_grafo*));
}

/**
 * @brief funcao para criar um vertice do grafo a partir dos dados fornecidos
 * @param vertice ponteiro para o vertice a ser criado
 * @param tecnologia string com a tecnologia origem do vertice
 * @param grupo grupo do vertice
 */
void cria_vertice(vertice_grafo *vertice, string tecnologia, int grupo) {
    vertice->nomeTecOrigem = (char*) malloc((tecnologia.tamanho + 1) * sizeof(char));
    strcpy(vertice->nomeTecOrigem, tecnologia.nome);
    vertice->grupo = grupo;
    vertice->grau_entrada = 0;
    vertice->grau_saida = 0;
    vertice->grau = 0;
    inicializa_lista(&(vertice->lista_arestas));
}

/**
 * @brief funcao para criar uma aresta do grafo a partir do vertice de origem e do registro de dados
 * @param aresta ponteiro para a aresta a ser criada
 * @param vertice_destino vertice de destino a ser indicado pela aresta
 * @param peso peso da aresta
 */
void cria_aresta(aresta_grafo *aresta, vertice_grafo *vertice_destino, int peso) {
    aresta->tecDestino = vertice_destino;
    aresta->peso = peso;
}

/**
 * @brief insere uma aresta em um dado vertice
 * @param vertice vertice onde inserir a aresta
 * @param aresta aresta a ser inserida
 */
void insere_aresta_vertice(vertice_grafo *vertice, aresta_grafo *aresta) {
    inserir(&(vertice->lista_arestas), aresta);
    vertice->grau_saida++;
    vertice->grau++;
    aresta->tecDestino->grau++;
    aresta->tecDestino->grau_entrada++;
}

/**
 * @brief realiza busca binaria na lista de adjacencias para encontrar o vertice com uma dada tecnologia origem
 * @param g grafo onde realizar a busca
 * @param nomeTec tecnologia origem do vertice buscado
 * @return posicao do vertice buscado na lista de adjacencias
 */
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

/**
 * @brief insere um dado vertice na lista de adjacencias do grafo
 * @param g grafo onde realizar a insercao
 * @param vertice vertice a ser inserido
 * @param pos posicao onde inserir o vertice
 */
void insere_vertice(grafo *g, vertice_grafo *vertice, int pos) {
    for(int i = g->num_vertices; i > pos; i--) {
        g->lista_vertices[i] = g->lista_vertices[i-1];
    }
    g->lista_vertices[pos] = vertice;
    g->num_vertices++;
}

/**
 * @brief insere uma aresta no grafo, definida pelos parametros da funcao
 * @param g grafo onde realizar a insercao
 * @param tecnologiaOrigem string da tecnologia origem
 * @param grupo1 grupo ao qual a tecnologia origem pertence
 * @param tecnologiaDestino string da tecnologia destino
 * @param grupo2 grupo ao qual a tecnologia destino pertence
 * @param peso peso da aresta
 */
void insere_aresta(grafo *g, string tecnologiaOrigem, int grupo1, string tecnologiaDestino, int grupo2, int peso) {
    int pos;
    vertice_grafo *verticeOrigem = malloc(sizeof(vertice_grafo));
    vertice_grafo *verticeDestino = malloc(sizeof(vertice_grafo));
    aresta_grafo *aresta = malloc(sizeof(aresta_grafo));


    // Primeira inserção, insere vertice e aresta direto
    if(g->num_vertices == 0) {
        // Cria os vertices a serem inseridos no grafo e os insere
        cria_vertice(verticeOrigem, tecnologiaOrigem, grupo1);
        cria_vertice(verticeDestino, tecnologiaDestino, grupo2);
        insere_vertice(g, verticeOrigem, 0);
        pos = busca_binaria_vertices(*g, tecnologiaDestino.nome);
        insere_vertice(g, verticeDestino, pos);

        // Cria aresta a ser inserida no grafo
        cria_aresta(aresta, verticeDestino, peso);

        // Adiciona aresta na lista de arestas do vertice
        insere_aresta_vertice(verticeOrigem, aresta);
        g->num_arestas++;
    }
    else {
        // checa se o vertice origem ja existe na lista de adjacencias, criando e inserindo caso necessario
        pos = busca_binaria_vertices(*g, tecnologiaOrigem.nome);
        if(pos < g->num_vertices && strcmp(g->lista_vertices[pos]->nomeTecOrigem, tecnologiaOrigem.nome) == 0) {
            // atribui a verticeOrigem o vertice ja existente correspondente
            free(verticeOrigem);
            verticeOrigem = g->lista_vertices[pos];
            if(verticeOrigem->grupo == 0) {
                verticeOrigem->grupo = grupo1;
            }
        }
        else {
            // cria e insere o vertice
            cria_vertice(verticeOrigem, tecnologiaOrigem, grupo1);
            insere_vertice(g, verticeOrigem, pos);
        }

        // checa se o vertice destino ja existe na lista de adjacencias, criando e inserindo caso necessario
        pos = busca_binaria_vertices(*g, tecnologiaDestino.nome);
        if(pos < g->num_vertices && strcmp(g->lista_vertices[pos]->nomeTecOrigem, tecnologiaDestino.nome) == 0) {
            // atribui a verticeDestino o vertice ja existente correspondente
            free(verticeDestino);
            verticeDestino = g->lista_vertices[pos];
            if(verticeDestino->grupo == 0) {
                verticeDestino->grupo = grupo2;
            }
        }
        else {
            // cria e insere o vertice
            cria_vertice(verticeDestino, tecnologiaDestino, grupo2);
            insere_vertice(g, verticeDestino, pos);
        }

        // cria aresta a ser inserida
        cria_aresta(aresta, verticeDestino, peso);
        
        // insere aresta criada
        insere_aresta_vertice(verticeOrigem, aresta);
        g->num_arestas++;
    }
}

/**
 * @brief libera memoria alocada pelo grafo
 * @param g grafo a ser liberado
 */
void libera_grafo(grafo g) {
    for(int i = 0; i < g.num_vertices; i++) {
        vertice_grafo *vertice = g.lista_vertices[i];
        destruir(&(vertice->lista_arestas));
        free(vertice->nomeTecOrigem);
        free(vertice);
    }

    free(g.lista_vertices);
}