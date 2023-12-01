// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registros_arvore.h"
#include "registros_dados.h"
#include "subfuncoes_internas_arvore.h"

#ifndef FUNCOES_ARVORE
#define FUNCOES_ARVORE

void insere_raiz(FILE* arquivo, header_arvore* cabecalho, dado *data, registro_arvore* raiz);
dado* insere_loop(FILE* arquivo, header_arvore* cabecalho, dado *data, int* altura, registro_arvore* registro_carregado);
dado* insere_reg(FILE* arquivo, header_arvore* cabecalho, dado* data, registro_arvore* reg, int pos, int pont);
int busca_em_indice(FILE* arq_arvore, FILE* arq_dados, char *valorCampoBuscado, header_arvore cabecalho_arvore, int *primeiraBuscaArvore, registro_arvore* reg_raiz_arvore);
int busca_em_arvore(FILE *arq_arvore, char *valorCampoBuscado, registro_arvore *reg_no_atual);

#endif