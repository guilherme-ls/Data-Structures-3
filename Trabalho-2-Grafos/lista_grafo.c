// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#include "lista_grafo.h"

/**
 * @brief inicializa a lista
 * @param l ponteiro para a lista a ser inicializada
 */
void inicializa_lista(lista *l) {
    l->ini = NULL;
    l->tam = 0;
}

/**
 * @brief destroi a lista
 * @param l ponteiro para a lista a ser destruida
 */
void destruir(lista *l) {
    if(l != NULL){
        no *p = l->ini;

        while(p != NULL){
            l->ini = p->prox;
            free(p->info);
            free(p);
            p = l->ini;
        }
    }
}

/**
 * @brief insere arestas na lista de forma ordenada
 * @param l ponteiro para a lista
 * @param aresta aresta a ser inserida
 */
void inserir(lista *l, aresta_grafo *aresta) {
    no *p = malloc(sizeof(no));
    p->info = aresta;
    p->prox = NULL;

    if(l->ini == NULL) {
        l->ini = p;
        l->tam++;
    }
    else {
        no* prev = NULL;
        no* atual = l->ini;
        while(atual != NULL) {
            if(strcmp(atual->info->tecDestino->nomeTecOrigem, p->info->tecDestino->nomeTecOrigem) > 0) {
                break;
            }
            prev = atual;
            atual = atual->prox;
        }
        if(prev != NULL) {
            prev->prox = p;
        }
        else {
            l->ini = p;
        }
        p->prox = atual;
        l->tam++;
    }
}

/**
 * @brief funcao para busca da aresta de tecnologia destino com um dado nome
 * @param l lista onde realizar a busca
 * @param nomeTec nome da tecnologia destino buscada
 * @return posicao onde encontrou a tecnologia, ou -1 quando nao encontrar
 */
int busca(lista *l, char* nomeTec) {
    if(l->ini == NULL) {
        return -1;
    }
    no* atual = l->ini;
    int pos = 0;
    while(atual != NULL) {
        if(strcmp(atual->info->tecDestino->nomeTecOrigem, nomeTec) == 0) {
            return pos;
        }
        pos++;
        atual = atual->prox;
    }
    return -1;
}