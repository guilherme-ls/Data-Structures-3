// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#ifndef FUNCOES_DADOS
#define FUNCOES_DADOS

#include "registros_dados.h"
#include "registros_arvore.h"
#include "lista.h"

void divide_string(registro* reg, char* entrada);
int checa_int_nulo(char* campo);
int le_entrada(char* entrada, FILE* arq_csv);
int busca_em_arq_dados(FILE *arq_bin, char* nomeCampo, char* valorCampoBuscado);
void entrada_para_registro(registro* reg);
void le_arquivo_de_dados(FILE *arq_dados, lista *l);

#endif