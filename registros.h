// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#ifndef REGISTROS
#define REGISTROS

// tamanho do registro de cabecalho
#define TAM_HEADER 13

// tamanho do registro de dados
#define TAM_REG 76

// tamanho da parte alocada estaticamente do registro de dados
#define TAM_REG_STATIC 21

// caractere a ser empregado como lixo na escrita do registro
#define LIXO '$'

// registros 'headers' com 13 bytes, com campos especificados
struct header {
    char status;
    int proxRRN;
    int nroTecnologias;
    int nroParesTecnologias;
};
typedef struct header header;

// struct para strings com indicadores de tamanho
struct string {
    int tamanho;
    char* nome;
};
typedef struct string string;

// registros de 76 bytes, com campos especificados
struct registro {
    char removido;
    int grupo;
    int popularidade;
    int peso;
    string tecnologiaOrigem;
    string tecnologiaDestino;
};
typedef struct registro registro;


// funcoes de manipulacao de registros

void escrever_registro(FILE* arquivo, registro reg);
int ler_registro(FILE* arquivo, registro* reg);
int ler_campo(FILE* arquivo, char** valCampo, char* nomeCampo);
void escrever_header(FILE* arquivo, header cabecalho);
int ler_header(FILE* arquivo, header* cabecalho);
int calcula_byte_off(int RRN);
void inicializa_registro(registro* reg);
void inicializa_cabecalho(header* cabecalho);
void imprime_registro(registro reg);
void imprime_campo_texto(string texto, char* fim);
void imprime_campo_numerico(int num, char* fim);

#endif