// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoesAuxiliares.h"

/**
 * @brief abre um arquivo com o nome e modo especificados
 * @param arq variavel na qual se armazena o ponteiro para o arquivo aberto
 * @param nome nome do arquivo a ser aberto
 * @param mode modo em que o arquivo sera aberto
 * @return 0, caso o arquivo seja aberto com sucesso, ou 1, em caso de falha
 */
int open(FILE** arq, char* nome, char* mode) {
    *arq = fopen(nome, mode);
    if(*arq == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return 1;
    }
    return 0;
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