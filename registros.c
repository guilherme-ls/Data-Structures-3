// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#include <stdio.h>
#include <stdlib.h>
#include "registros.h"

/**
 * @brief calcula o tamanho do lixo ao final do registro
 * @param reg registro cujo tamanho do lixo deve ser calculado
 * @return tamanho do lixo
 */
int calcula_lixo(registro reg) {
    return (TAM_REG - (TAM_REG_STATIC + reg.tamanhoTecnologiaOrigem + reg.tamanhoTecnologiaDestino));
}

/**
 * @brief escreve o registro fornecido na posicao corrente no arquivo dado
 * @param arquivo arquivo onde sera escrito o registro
 * @param reg registro a ser escrito no arquivo
 */
void escrever_registro(FILE* arquivo, registro reg) {
    // calcula tamanho do lixo e o cria
    int tam_lixo = calcula_lixo(reg);
    char* lixo = (char *)malloc(tam_lixo * sizeof(char));
    for(int i = 0; i < tam_lixo; i++)
        lixo[i] = '$';

    // escrita do registro no binário
    fwrite(&reg.removido, sizeof(char), 1, arquivo);
    fwrite(&reg.grupo, sizeof(int), 1, arquivo);
    fwrite(&reg.popularidade, sizeof(int), 1, arquivo);
    fwrite(&reg.peso, sizeof(int), 1, arquivo);
    fwrite(&reg.tamanhoTecnologiaOrigem, sizeof(int), 1, arquivo);
    fwrite(reg.nomeTecnologiaOrigem, sizeof(char), reg.tamanhoTecnologiaOrigem, arquivo);
    fwrite(&reg.tamanhoTecnologiaDestino, sizeof(int), 1, arquivo);
    fwrite(reg.nomeTecnologiaDestino, sizeof(char), reg.tamanhoTecnologiaDestino, arquivo);
    fwrite(lixo, sizeof(char), tam_lixo, arquivo);

    // libera o lixo
    free(lixo);
}

/**
 * @brief le um registro no arquivo fornecido a partir da posicao corrente e o armazena em reg
 * @param arquivo arquivo a ser lido (binario)
 * @param reg struct de registro no qual sera armazenado o registro lido
 * @return 1, quando encontrar fim do arquivo, ou 0, quando le com sucesso
 */
int ler_registro(FILE* arquivo, registro* reg) {
    // leitura do registro
    if(fread(&(reg->removido), sizeof(char), 1, arquivo) == 0) {
        // retorno com fim do arquivo
        return 1;
    }
    // leitura dos dados numericos iniciais
    fread(&(reg->grupo), sizeof(int), 1, arquivo);
    fread(&(reg->popularidade), sizeof(int), 1, arquivo);
    fread(&(reg->peso), sizeof(int), 1, arquivo);

    // leitura do tamanho do registro de texto inicial e alocacao da string necessaria
    fread(&(reg->tamanhoTecnologiaOrigem), sizeof(int), 1, arquivo);
    reg->nomeTecnologiaOrigem = malloc((reg->tamanhoTecnologiaOrigem + 1) * sizeof(char));

    // leitura da string e preenchimento do terminador nulo
    fread(reg->nomeTecnologiaOrigem, sizeof(char), reg->tamanhoTecnologiaOrigem, arquivo);
    reg->nomeTecnologiaOrigem[reg->tamanhoTecnologiaOrigem] = '\0';

    // leitura do tamanho do segundo registro de texto e alocacao da string necessaria
    fread(&(reg->tamanhoTecnologiaDestino), sizeof(int), 1, arquivo);
    reg->nomeTecnologiaDestino = malloc((reg->tamanhoTecnologiaDestino + 1) * sizeof(char));

    // leitura da string e preenchimento do terminador nulo
    fread(reg->nomeTecnologiaDestino, sizeof(char), reg->tamanhoTecnologiaDestino, arquivo);
    reg->nomeTecnologiaDestino[reg->tamanhoTecnologiaDestino] = '\0';

    // calcula tamanho do lixo e o pula
    fseek(arquivo, calcula_lixo(*reg), SEEK_CUR);

    // retorno com sucesso
    return 0;
}

/**
 * @brief escreve o cabecalho fornecido na posicao corrente no arquivo dado (binario)
 * @param arquivo arquivo no qual sera escrito o cabecalho
 * @param cabecalho cabecalho a ser escrito no arquivo
 */
void escrever_header(FILE* arquivo, header cabecalho) {
    // escreve os dados contidos no registro de cabecalho
    fwrite(&cabecalho.status, sizeof(char), 1, arquivo);
    fwrite(&cabecalho.proxRRN, sizeof(int), 1, arquivo);
    fwrite(&cabecalho.nroTecnologias, sizeof(int), 1, arquivo);
    fwrite(&cabecalho.nroParesTecnologias, sizeof(int), 1, arquivo);
}

/**
 * @brief le o registro de cabecalho no arquivo fornecido a partir da posicao corrente e o armazena em cabecalho
 * @param arquivo arquivo a ser lido (binario)
 * @param cabecalho struct de cabecalho no qual sera armazenado o cabecalho lido
 * @return 1, quando encontrar fim do arquivo, ou 0, quando le com sucesso
 */
int ler_header(FILE* arquivo, header* cabecalho) {
    // le os dados contidos no registro de cabecalho
    if(fread(&cabecalho->status, sizeof(char), 1, arquivo) == 0) {
        return 1;
    }
    fread(&cabecalho->proxRRN, sizeof(int), 1, arquivo);
    fread(&cabecalho->nroTecnologias, sizeof(int), 1, arquivo);
    fread(&cabecalho->nroParesTecnologias, sizeof(int), 1, arquivo);

    return 0;
}

/**
 * @brief calcula o byte offset para o registro do RRN fornecido
 * @param RRN RRN do registro desejado
 * @return byte offset do registro correspondente ao RRN fornecido
 */
int calcula_byte_off(int RRN) {
    return TAM_HEADER + RRN * TAM_REG;
}

/**
 * @brief inicializa registro de dados com parametros iniciais
 * @param reg ponteiro para o registro de dados a ser inicializado
 */
void inicializa_registro(registro* reg) {
    reg->removido = '0';
    reg->grupo = -1;
    reg->popularidade = -1;
    reg->peso = -1;
    reg->tamanhoTecnologiaOrigem = 0;
    reg->nomeTecnologiaOrigem = NULL;
    reg->tamanhoTecnologiaDestino = 0;
    reg->nomeTecnologiaDestino = NULL;
}

/**
 * @brief inicializa registro de cabecalho com parametros iniciais
 * @param cabecalho ponteiro para o registro de cabecalho a ser inicializado
 */
void inicializa_cabecalho(header* cabecalho) {
    cabecalho->status = 0;
    cabecalho->proxRRN = 0;
    cabecalho->nroTecnologias = 0;
    cabecalho->nroParesTecnologias = 0;
}

/**
 * @brief imprime registro de dados inseridos, considerando campos nulos
 * @param reg registro a ser impresso
 */
void imprime_registro(registro reg) {
    // printa primeiro campo de texto, caso exista, ou NULO, caso nao
    imprime_campo_texto(reg.nomeTecnologiaOrigem, reg.tamanhoTecnologiaOrigem, ", ");

    // printa campos numericos, caso positivos, ou NULO, caso -1
    imprime_campo_numerico(reg.grupo, ", ");
    imprime_campo_numerico(reg.popularidade, ", ");

    // printa segundo campo de texto, conforme especificacoes ja destacadas
    imprime_campo_texto(reg.nomeTecnologiaDestino, reg.tamanhoTecnologiaDestino, ", ");

    // printa ultimo campo numerico, conforme especificacoes ja destacadas
    imprime_campo_numerico(reg.peso, "\n");
}

/**
 * @brief imprime string inserido, caso distinto nao nulo, ou "NULO, ", caso seja nulo
 * @param texto string a ser printado
 * @param tam tamanho do texto a ser printado
 * @param fim string a ser inserida apos o campo impresso (para separacao)
 */
void imprime_campo_texto(char* texto, int tam, char* fim) {
    if(tam == 0) {
        printf("NULO");
    }
    else {
        printf("%s", texto);
    }
    printf("%s", fim);
}

/**
 * @brief imprime numero inserido, caso distinto de -1, ou "NULO, ", caso seja -1
 * @param num numero a ser printado
 * @param fim string a ser inserida apos o campo impresso (para separacao)
 */
void imprime_campo_numerico(int num, char* fim) {
    if(num == -1) {
        printf("NULO");
    }
    else {
        printf("%d", num);
    }
    printf("%s", fim);
}