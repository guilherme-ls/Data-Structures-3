// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registros.h"
#include "funcoesAuxiliares.h"

/**
 * @brief checa se um campo inteiro lido como string eh nulo, retornando -1 caso seja, ou seu valor convertido, caso nao
 * @param campo string correspondente ao campo que deve ser checado
 * @return valor do campo ou -1
 */
int checa_int_nulo(char* campo) {
    if(campo != NULL) {
        if(*campo != '\0')
            return atoi(campo);
    }
    return -1;
}

/**
 * @brief divide a string fornecida nos campos do registro e os armazeno em reg
 * @param reg registro no qual os campo serao armazenados
 * @param entrada string a ser dividida em campos
 */
void divide_string(registro* reg, char* entrada) {
    char* campo = entrada;

    // no caso de campos campos inteiros, checa-se por NULL antes de converter a string
    reg->tecnologiaOrigem.nome = strsep(&campo, ",");

    reg->grupo = checa_int_nulo(strsep(&campo, ","));

    reg->popularidade = checa_int_nulo(strsep(&campo, ","));

    reg->tecnologiaDestino.nome = strsep(&campo, ",");
    
    reg->peso = checa_int_nulo(strsep(&campo, ","));

    // preenche  o tamanho dos nomes armazenados
    if(reg->tecnologiaOrigem.nome != NULL)
        reg->tecnologiaOrigem.tamanho = strlen(reg->tecnologiaOrigem.nome);
    if(reg->tecnologiaDestino.nome != NULL)
        reg->tecnologiaDestino.tamanho = strlen(reg->tecnologiaDestino.nome);
}


/**
 * @brief le uma linha do arquivo especificado e armazena na string fornecida
 * @param entrada string onde sera armazenada a linha lida
 * @param arq_csv arquivo a ser lido 
 * @return 1 caso EOF, 0 caso executado com sucesso
 */
int le_entrada(char* entrada, FILE* arq_csv) {
    if(fgets(entrada, 100, arq_csv) == NULL) {
        free(entrada);
        return 1;
    }

    // remove caracteres '\n' e '\r'
    int size = strlen(entrada);
    for(int i = 0; i < size; i++) {
        if(entrada[i] == '\n' || entrada[i] == '\r') {
            entrada[i] = '\0';
        }
    }

    return 0;
}