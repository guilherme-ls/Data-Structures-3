// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#include "registros_dados.h"
#include "funcoesAuxiliares.h"

/**
 * @brief calcula o tamanho do lixo ao final do registro
 * @param reg registro cujo tamanho do lixo deve ser calculado
 * @return tamanho do lixo
 */
int calcula_lixo(registro reg) {
    return (TAM_REG - (TAM_REG_STATIC + reg.tecnologiaOrigem.tamanho + reg.tecnologiaDestino.tamanho));
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
    fread(&(reg->tecnologiaOrigem.tamanho), sizeof(int), 1, arquivo);
    reg->tecnologiaOrigem.nome = malloc((reg->tecnologiaOrigem.tamanho + 1) * sizeof(char));

    // leitura da string e preenchimento do terminador nulo
    fread(reg->tecnologiaOrigem.nome, sizeof(char), reg->tecnologiaOrigem.tamanho, arquivo);
    reg->tecnologiaOrigem.nome[reg->tecnologiaOrigem.tamanho] = '\0';

    // leitura do tamanho do segundo registro de texto e alocacao da string necessaria
    fread(&(reg->tecnologiaDestino.tamanho), sizeof(int), 1, arquivo);
    reg->tecnologiaDestino.nome = malloc((reg->tecnologiaDestino.tamanho + 1) * sizeof(char));

    // leitura da string e preenchimento do terminador nulo
    fread(reg->tecnologiaDestino.nome, sizeof(char), reg->tecnologiaDestino.tamanho, arquivo);
    reg->tecnologiaDestino.nome[reg->tecnologiaDestino.tamanho] = '\0';

    // calcula tamanho do lixo e o pula
    fseek(arquivo, calcula_lixo(*reg), SEEK_CUR);

    // retorno com sucesso
    return 0;
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
 * @brief le o registro de cabecalho e checa pela ocorrencia de erros, lidando com estes
 * @param arq_bin arquivo binario ja aberto no qual deve ser lido o registro
 * @param cabecalho struct de cabecalho no qual devem ser armazenadas as informacoes lidas
 * @return 0, caso o registro seja lido com sucesso, ou 1, em caso de falha
 */
int check_cabecalho(FILE* arq_bin, header* cabecalho) {
    int erro = ler_header(arq_bin, cabecalho);
    if(erro == 1) {
        // fim da execucao em caso de erros
        printf("Falha na execução da funcionalidade.\n");
        fclose(arq_bin);
        return 1;
    }

    // retorna erro caso 'status' do arquivo lido seja '0'
    if(cabecalho->status == '0') {
        printf("Falha na execução da funcionalidade.\n");
        fclose(arq_bin);
        return 1;
    }

    return 0;
}

