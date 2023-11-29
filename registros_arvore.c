// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registros_arvore.h"

/**
 * @brief escreve o registro fornecido na posicao corrente no arquivo dado
 * @param arquivo arquivo onde sera escrito o registro
 * @param reg registro a ser escrito no arquivo
 */
void escrever_registro_arvore(FILE* arquivo, registro_arvore reg) {
    // escrita do registro no binário
    fwrite(&reg.nroChavesNo, sizeof(int), 1, arquivo);
    fwrite(&reg.alturaNo, sizeof(int), 1, arquivo);
    fwrite(&reg.RRNdoNo, sizeof(int), 1, arquivo);

    // escreve primeiro ponteiro do registro
    fwrite(&(reg.ponteiro_arvore[0]), sizeof(int), 1, arquivo);
    // escreve demais chaves e ponteiros de acordo com a ordem definida para a arvore
    for(int i = 0; i < ORDEM-1; i++) {
        fwrite(reg.dados[i].chave, sizeof(char), TAM_CHAVE, arquivo);
        fwrite(&(reg.dados[i].ponteiro_dado), sizeof(int), 1, arquivo);
        fwrite(&(reg.ponteiro_arvore[i+1]), sizeof(int), 1, arquivo);
    }
}

/**
 * @brief le um registro no arquivo fornecido a partir da posicao corrente e o armazena em reg
 * @param arquivo arquivo a ser lido (binario)
 * @param reg struct de registro no qual sera armazenado o registro lido
 * @return 1, quando encontrar fim do arquivo, ou 0, quando le com sucesso
 */
int ler_registro_arvore(FILE* arquivo, registro_arvore* reg) {
    // leitura dos dados numericos iniciais
    if(fread(&(reg->nroChavesNo), sizeof(int), 1, arquivo) == 0) {
        // caso encontre o fim do arquivo, retorna 1
        return 1;
    }
    fread(&(reg->alturaNo), sizeof(int), 1, arquivo);
    fread(&(reg->RRNdoNo), sizeof(int), 1, arquivo);

    // le primeiro ponteiro do registro
    fread(&(reg->ponteiro_arvore[0]), sizeof(int), 1, arquivo);
    // le demais chaves e ponteiros de acordo com a ordem definida para a arvore
    for(int i = 0; i < ORDEM-1; i++) {
        fread(reg->dados[i].chave, sizeof(char), TAM_CHAVE, arquivo);
        fread(&(reg->dados[i].ponteiro_dado), sizeof(int), 1, arquivo);
        reg->dados[i].chave[TAM_CHAVE] =  '\0';
        fread(&(reg->ponteiro_arvore[i+1]), sizeof(int), 1, arquivo);
    }

    // retorno com sucesso
    return 0;
}


/**
 * @brief escreve o cabecalho fornecido na posicao corrente no arquivo dado (binario)
 * @param arquivo arquivo no qual sera escrito o cabecalho
 * @param cabecalho cabecalho a ser escrito no arquivo
 */
void escrever_header_arvore(FILE* arquivo, header_arvore cabecalho) {
    // escreve os dados contidos no registro de cabecalho
    fwrite(&cabecalho.status, sizeof(char), 1, arquivo);
    fwrite(&cabecalho.noRaiz, sizeof(int), 1, arquivo);
    fwrite(&cabecalho.RRNproxNo, sizeof(int), 1, arquivo);

    // escreve bytes de lixo ao final do cabecalho
    char lixo[TAM_LIXO_HEADER];
    for(int i = 0; i < TAM_LIXO_HEADER; i++)
        lixo[i] = LIXO_ARVORE;
    fwrite(lixo, sizeof(char), TAM_LIXO_HEADER, arquivo);
}

/**
 * @brief le o registro de cabecalho no arquivo fornecido a partir da posicao corrente e o armazena em cabecalho
 * @param arquivo arquivo a ser lido (binario)
 * @param cabecalho struct de cabecalho no qual sera armazenado o cabecalho lido
 * @return 1, quando encontrar fim do arquivo, ou 0, quando le com sucesso
 */
int ler_header_arvore(FILE* arquivo, header_arvore* cabecalho) {
    // le os dados contidos no registro de cabecalho
    if(fread(&cabecalho->status, sizeof(char), 1, arquivo) == 0) {
        return 1;
    }
    fread(&cabecalho->noRaiz, sizeof(int), 1, arquivo);
    fread(&cabecalho->RRNproxNo, sizeof(int), 1, arquivo);

    return 0;
}

/**
 * @brief calcula o byte offset para o registro do RRN fornecido
 * @param RRN RRN do registro desejado
 * @return byte offset do registro correspondente ao RRN fornecido
 */
int calcula_byte_off_arvore(int RRN) {
    return TAM_HEADER_ARVORE + RRN * TAM_REG_ARVORE;
}

/**
 * @brief inicializa registro de dados com parametros iniciais
 * @param reg ponteiro para o registro de dados a ser inicializado
 */
void inicializa_registro_arvore(registro_arvore* reg) {
    // inicializacao dos dados iniciais do registro
    reg->nroChavesNo = 0;
    reg->alturaNo = 1;
    reg->RRNdoNo = 0;

    // inicializacao do primeiro ponteiro da arvore
    reg->ponteiro_arvore[0] = -1;
    // inicializacao das chaves e ponteiros de acordo com a ordem definida para a arvore
    for (int i = 0; i < ORDEM-1; i++) {
        // escrita das chaves como lixo
        for (int j = 0; j < TAM_CHAVE; j++) {
            reg->dados[i].chave[j] = LIXO_ARVORE;
        }
        reg->dados[i].chave[TAM_CHAVE] = '\0';

        // escrita dos demais ponteiros
        reg->dados[i].ponteiro_dado = -1;
        reg->ponteiro_arvore[i+1] = -1;
    }
    
}

/**
 * @brief inicializa registro de cabecalho com parametros iniciais
 * @param cabecalho ponteiro para o registro de cabecalho a ser inicializado
 */
void inicializa_cabecalho_arvore(header_arvore* cabecalho) {
    cabecalho->status = '0';
    cabecalho->noRaiz = -1;
    cabecalho->RRNproxNo = 0;
}
