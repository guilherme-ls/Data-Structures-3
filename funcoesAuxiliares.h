// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#ifndef FUNCOES_AUXILIARES
#define FUNCOES_AUXILIARES

#include "registros_arvore.h"
#include "lista.h"

void concatena_chave(registro reg, char* string);
int check_cabecalho(FILE* arq_bin, header* cabecalho);
int check_cabecalho_arvore(FILE* arq_bin, header_arvore* cabecalho);
int open(FILE** arq, char* nome, char* mode);
void divide_string(registro* reg, char* entrada);
int checa_int_nulo(char* campo);
int le_entrada(char* entrada, FILE* arq_csv);
int busca_em_arq_dados(FILE *arq_bin, char* nomeCampo, char* valorCampoBuscado);
void entrada_para_registro(registro* reg);
void le_arquivo_de_dados(FILE *arq_dados, lista *l);

#endif