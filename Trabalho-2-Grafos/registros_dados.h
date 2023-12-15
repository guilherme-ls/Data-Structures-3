// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef REGISTROS_DADOS
#define REGISTROS_DADOS

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

int ler_registro(FILE* arquivo, registro* reg);
int ler_header(FILE* arquivo, header* cabecalho);
int calcula_byte_off(int RRN);
int check_cabecalho(FILE* arq_bin, header* cabecalho);

#endif