// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#include "lista.h"
#include <stdlib.h>
#include <string.h>

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
 * @brief insere copia de string na lista
 * @param l ponteiro para a lista
 * @param x string cuja copia deve ser inserida
 */
void inserir(lista *l, char* x) {
    no *p = malloc(sizeof(no));
    p->info = malloc((strlen(x) + 1) * sizeof(char));
    strcpy(p->info, x);
    p->prox = NULL;

    if(l->ini == NULL){
        l->ini = p;
        l->tam++;
    }
    else{
        no* prev;
        no *atual = l->ini;
        while(atual != NULL) {
            if(strcmp(atual->info, p->info) == 0) {
                free(p->info);
                free(p);
                return;
            }
            prev = atual;
            atual = atual->prox;
        }
        prev->prox = p;
        l->tam++;
    }
}