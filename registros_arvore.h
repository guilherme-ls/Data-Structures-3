// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#ifndef REGISTROS_ARVORE
#define REGISTROS_ARVORE

// tamanho do registro de cabecalho
#define TAM_HEADER_ARVORE 205

// define tamanho do lixo a ser preenchido ao final do registro de cabecalho
#define TAM_LIXO_HEADER 196

// tamanho do registro de dados
#define TAM_REG_ARVORE 205

// tamanho da chave de busca no registro de dados
#define TAM_CHAVE 55

// caractere a ser empregado como lixo na escrita do registro
#define LIXO_ARVORE '$'

// define ordem da arvore-B construida
#define ORDEM 4

// registros 'headers' de 205 bytes, com campos especificados
struct header_arvore {
    char status;
    int noRaiz;
    int RRNproxNo;
};
typedef struct header_arvore header_arvore;

// struct de dados
struct dado {
    char chave[TAM_CHAVE + 1]; // utiliza-se 1 espaco de memoria a mais para levar em conta o '\0' ao final da e string (nao escrito no arquivo de dados)
    int ponteiro_dado;
};
typedef struct dado dado;

// registros de 205 bytes, com campos especificados
struct registro_arvore {
    int nroChavesNo;
    int alturaNo;
    int RRNdoNo;
    int ponteiro_arvore[ORDEM];
    dado dados[ORDEM-1];
};
typedef struct registro_arvore registro_arvore;


// funcoes de manipulacao de registros

void escrever_registro_arvore(FILE* arquivo, registro_arvore reg);
int ler_registro_arvore(FILE* arquivo, registro_arvore* reg);
void escrever_header_arvore(FILE* arquivo, header_arvore cabecalho);
int ler_header_arvore(FILE* arquivo, header_arvore* cabecalho);
int calcula_byte_off_arvore(int RRN);
void inicializa_registro_arvore(registro_arvore* reg);
void inicializa_registro_preparado_arvore(registro_arvore* reg, header_arvore* cabecalho, int altura);
void inicializa_cabecalho_arvore(header_arvore* cabecalho);

#endif