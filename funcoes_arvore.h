// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registros_arvore.h"
#include "registros_dados.h"

#ifndef FUNCOES_ARVORE
#define FUNCOES_ARVORE

void insere_raiz(FILE* arquivo, header_arvore* cabecalho, dado *data);
dado* insert(FILE* arquivo, header_arvore* cabecalho, dado *data, int* altura);
dado* insert_here(FILE* arquivo, header_arvore* cabecalho, dado* data, registro_arvore* reg, int pos, int pont);
void clear_dado(dado* data);
int pega_raiz(FILE* arq_arvore, header_arvore header, registro_arvore* reg);
int ler_reg_dados_do_indice(FILE* arq_dados, registro_arvore reg_arvore, registro* reg_dados, int iBuscaBinaria);

#endif