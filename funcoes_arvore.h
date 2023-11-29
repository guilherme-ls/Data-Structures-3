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
dado* insere_loop(FILE* arquivo, header_arvore* cabecalho, dado *data, int* altura);
dado* insere_reg(FILE* arquivo, header_arvore* cabecalho, dado* data, registro_arvore* reg, int pos, int pont);
void divide_chaves(registro_arvore* reg, registro_arvore* new_reg, int comeco);
void insere_chave_reg(registro_arvore* reg, dado chave, int ponteiro, int pos);
void clear_dado(dado* data);
int pega_raiz(FILE* arq_arvore, header_arvore header, registro_arvore* reg);
int ler_reg_dados_do_indice(FILE* arq_dados, registro_arvore reg_arvore, registro* reg_dados, int iBuscaBinaria);
int busca_binaria_reg_arvore(char* chaveBuscada, registro_arvore reg);
void ler_chave_sem_lixo(char* chave_original, char* chave_limpa);

#endif