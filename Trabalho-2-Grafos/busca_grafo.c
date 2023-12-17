// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#include <stdlib.h>
#include <string.h>
#include "busca_grafo.h"
#include "heap.h"

/**
 * @brief busca a tecnologia origem fornecida e imprime todas as tecnologias destino para a qual possui arestas de saida
 * @param g grafo onde buscar
 * @param tecnologia tecnologia origem buscada
 */
void busca_tecnologias_entrada(grafo g, char* tecnologia) {
    // realiza busca binaria na lista de adjacencias para encontrar o vertice buscado
    int pos = busca_binaria_vertices(g, tecnologia);

    // verifica se o vertice foi encontrado
    vertice_grafo vertice = *(g.lista_vertices[pos]);
    if(strcmp(tecnologia, vertice.nomeTecOrigem) != 0) {
        printf("Registro inexistente.\n");
        return;
    }

    // se encontrado, verifica o tamanho de sua lista de arestas
    if(vertice.grau_saida == 0) {
        printf("Registro inexistente.\n");
        return;
    }

    // caso existam elementos, printa a saida desejada
    printf("%s: ", vertice.nomeTecOrigem);
    no *aresta = vertice.lista_arestas.ini;
    for(int i = 0; i < vertice.lista_arestas.tam - 1; i++) {
        printf("%s, ", aresta->info->tecDestino->nomeTecOrigem);
        aresta = aresta->prox;
    }
    printf("%s\n\n", aresta->info->tecDestino->nomeTecOrigem);
}

/**
 * @brief verifica se um vertice fornecido ainda nao foi descoberto (cor branca)
 * @param g grafo onde o vertice se encontra
 * @param vertice vertice a ser verificado
 * @param cor vetor de cores dos vertices
 * @return 1 caso nao tenha sido descoberto, 0 caso tenha
 */
int verificaNaoDescoberto(grafo *g, vertice_grafo *vertice, int cor[]){
    // determina a posicao do vertice
    int pos = busca_binaria_vertices(*g, vertice->nomeTecOrigem);

    // verifica sua cor e retorna a saida adequada
    if(cor[pos] == 0){
        return 1;
    }
    return 0;
}  

/**
 * @brief realiza busca em profundidade armazenando vertices visitados na pilha
 * @param g grafo em que a busca e realizada
 * @param verticeAtual vertice visitado
 * @param pilha pilha para ordenar vertices na ordem decrescente de visita 
 * @param indicePilha indice atual para inserção na pilha
 * @param cor vetor que armazena qual o cor (estado) o vertice se encontra
 * 
*/
void procedimentoBuscaDFS(grafo *g, vertice_grafo *verticeAtual, int *pilha, int *indicePilha, int *cor){
    int posAtual = busca_binaria_vertices(*g, verticeAtual->nomeTecOrigem);
    // Pinta vertice de cinza e marca tempo de descoberta
    cor[posAtual] = 1;

    // Empilha vertices adjacentes nao descobertos
    no *noAdjacente = verticeAtual->lista_arestas.ini;
    aresta_grafo *aresta_adjacente;
    while(noAdjacente != NULL){
        aresta_adjacente = noAdjacente->info;
        if(verificaNaoDescoberto(g, aresta_adjacente->tecDestino, cor)){
            procedimentoBuscaDFS(g, aresta_adjacente->tecDestino, pilha, indicePilha, cor);
        }
        noAdjacente = noAdjacente->prox;
    }

    // Adiciona a pilha de ordem
    pilha[(*indicePilha)++] = posAtual;

    // Sem vertices adjacentes pinta de preto e marca tempo de termino
    cor[posAtual] = 2;
}

/**
 * @brief realiza busca em profundidade no grafo transposto para contar quantidade de componentes formente conexos
 * @param g grafo em que a busca e realizada
 * @param verticeAtual vertice visitado
 * @param cor vetor que armazena qual o cor (estado) o vertice se encontra
 * 
*/
void procedimentoBuscaDFSTransposto(grafo *g, vertice_grafo *verticeAtual, int *cor){
    int posAtual = busca_binaria_vertices(*g, verticeAtual->nomeTecOrigem);
    // Pinta vertice de cinza e marca tempo de descoberta
    cor[posAtual] = 1;

    // Empilha vertices adjacentes nao descobertos
    no *noAdjacente = verticeAtual->lista_arestas.ini;
    aresta_grafo *aresta_adjacente;
    while(noAdjacente != NULL){
        aresta_adjacente = noAdjacente->info;
        if(verificaNaoDescoberto(g, aresta_adjacente->tecDestino, cor)){
            procedimentoBuscaDFSTransposto(g, aresta_adjacente->tecDestino, cor);
        }
        noAdjacente = noAdjacente->prox;
    }

    // Sem vertices adjacentes pinta de preto e marca tempo de termino
    cor[posAtual] = 2;
}

/**
 * @brief Realiza a contagem da quantidade de componentes fortemente conexos a partir de busca em profundidade (Algoritmo de Kosaraju)
 * @param g grafo para contagem de componentes fortemente conexos
 * @param g_transposto transposto do grafo de interesse.
*/
void ContaCFC(grafo *g, grafo *g_transposto){
    int cor[g->num_vertices]; // 0 (branco) - Nao descoberto; 1 (cinza) - Descoberto; 2 (preto) - Finalizado
    int *pilha = (int*) malloc(g->num_vertices * sizeof(int));  // Pilha para armazenar ordem dos vertices
    int indicePilha = 0;

    // Inicializa cor branca (= 0) em todos
    for(int i = 0; i < g->num_vertices; i++){
        cor[i] = 0;
    } 

    // Declara variavel para vertice atual
    vertice_grafo *verticeAtual;

    int normal = 0;
    // Loop para busca em profundidadae no grafo normal
    for(int i = 0; i < g->num_vertices; i++){
        // Caso ainda haja vertices nao descobertos
        if(cor[i] == 0){
            normal++;
            verticeAtual = g->lista_vertices[i];
            procedimentoBuscaDFS(g, verticeAtual, pilha, &indicePilha, cor);
        }
    }

    // Reinicializa cor branca (= 0) em todos
    for(int i = 0; i < g->num_vertices; i++){
        cor[i] = 0;
    } 
    
    int cont = 0;
    // Loop para busca em profundidadae no grafo transposto
    for(int i = indicePilha-1; i >= 0; i--){
        // Caso ainda haja vertices nao descobertos
        int idAtual = pilha[i];
        if(cor[idAtual] == 0){
            cont++;
            verticeAtual = g_transposto->lista_vertices[idAtual];
            procedimentoBuscaDFSTransposto(g_transposto, verticeAtual, cor);
        }
    }
    
    if(cont == 1){
        printf("Sim, o grafo é fortemente conexo e possui 1 componente.\n");
    }else{
        printf("Não, o grafo não é fortemente conexo e possui %d componentes.\n", cont);
    }

    // Libera memoria da pilha
    free(pilha);

}

/**
 * @brief Busca o menor caminho entre uma tecnologia origem a uma tecnologia destino
 * @param g grafo onde busca e realizada
 * @param tecnologiaInicial nome da tecnologia inicial
 * @param tecnologiaFinal nome da tecnologia final
 * @return distancia para sair do vertice inicial e chegar no final
*/
int dijkstra(grafo g, char* tecnologiaInicial, char* tecnologiaFinal){
    // Realiza busca binaria na lista de adjacencia para encontrar o vertice de origem
    int posInicial = busca_binaria_vertices(g, tecnologiaInicial);  // Posicao vertice origem.
    int posFinal = busca_binaria_vertices(g, tecnologiaFinal);  // Posicao vertice destino.
    int size = g.num_vertices;
    int distancias[g.num_vertices];  // Vetor para armazenar distancias minímas a partir do vertice inicial
    int heap[g.num_vertices];
    
    // Verifica se existem os vertices inicial e final em questao
    vertice_grafo verticeInicial = *(g.lista_vertices[posInicial]);
    if(strcmp(tecnologiaInicial, verticeInicial.nomeTecOrigem) != 0) {
        return INFINITO;
    }
    vertice_grafo verticeFinal = *(g.lista_vertices[posFinal]);
    if(strcmp(tecnologiaFinal, verticeFinal.nomeTecOrigem) != 0) {
        return INFINITO;
    }

    // Inicializacao das distancias
    for(int i = 0; i < g.num_vertices; i++) {
        heap[i] = i;
        distancias[i] = INFINITO;
    }

    // Começa pelo vertice origem
    vertice_grafo vertice_atual = verticeInicial;
    distancias[posInicial] = 0;
    int posAtual = posInicial;
    organiza_heap(heap, size, distancias);

    // Loop para encontrar menor caminho entre os vertices
    while(1) {
        // define lista de arestas a ser analisada
        no *no_atual_lista = vertice_atual.lista_arestas.ini;
        aresta_grafo aresta_adjacente;
        for(int i = 0; i < vertice_atual.lista_arestas.tam; i++) {
            aresta_adjacente = *(no_atual_lista->info);
            
            // Para todo vertice (v) adjacente ao atual (w) D[v] = min(D[v], D[w] + peso aresta(w, v)) 
            int posAdjacente = busca_binaria_vertices(g, aresta_adjacente.tecDestino->nomeTecOrigem);
            if(distancias[posAtual] + aresta_adjacente.peso < distancias[posAdjacente]) {
                distancias[posAdjacente] = distancias[posAtual] + aresta_adjacente.peso;
            }

            // Vai para proxima aresta
            no_atual_lista = no_atual_lista->prox;
        }
        if(size == 0) {
            // Nao ha mais vertices para analisar
            break;
        }

        // reorganiza a heap com base nas novas posicoes e seleciona seu topo para ser o novo vertice a ser analisado
        organiza_heap(heap, size, distancias);
        posAtual = remove_top(heap, &size, distancias);
        size--;
        vertice_atual = *(g.lista_vertices[posAtual]);
    }

    // Retorna tamanho do menor caminho para vertice final.
    return distancias[posFinal];
}