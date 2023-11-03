// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registros_arvore.h"

#ifndef FUNCOES_ARVORE
#define FUNCOES_ARVORE

void insere_raiz(FILE* arquivo, header_arvore* cabecalho, dado *data);
dado* insert(FILE* arquivo, header_arvore* cabecalho, dado *data, int* altura);
dado* insert_here(FILE* arquivo, header_arvore* cabecalho, dado* data, registro_arvore* reg, int pos, int pont);
void clear_dado(dado* data);

#endif