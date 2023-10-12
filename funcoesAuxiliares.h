// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#ifndef FUNCOES_AUXILIARES
#define FUNCOES_AUXILIARES

int check_cabecalho(FILE* arq_bin, header* cabecalho);
int open(FILE** arq, char* nome, char* mode);
void divide_string(registro* reg, char* entrada);
int checa_int_nulo(char* campo);
int le_entrada(char* entrada, FILE* arq_csv);

#endif