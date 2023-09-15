#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registros.h"
#include "funcionalidades.h"

/**
 * @brief Executa a funcionalidade 1, lendo um dado arquivo csv e convertendo seus dados para registros em binario
 */
void funcionalidade1() {
    // recebe arquivos de entrada e saida
    char nome_csv[30];
    char nome_bin[30];
    scanf("%s", nome_csv);
    scanf("%s", nome_bin);

    // abre arquivo de saida em modo de escrita
    FILE *arq_bin = fopen(nome_bin, "wb");

    // abre arquivo de entrada em modo de leitura
    FILE *arq_csv = fopen(nome_csv, "r");

    // pula o cabecalho do csv lido
    fscanf(arq_csv, "nomeTecnologiaOrigem,grupo,popularidade,nomeTecnologiaDestino,peso\n", NULL);

    // cria registro de cabecalho
    header cabecalho;
    cabecalho.status = 1;
    cabecalho.proxRRN = 0;
    cabecalho.nroTecnologias = 0;
    cabecalho.nroParesTecnologias = 0;

    fseek(arq_bin, TAM_HEADER, SEEK_SET);

    // executa o loop ate chegar ao fim do arquivo
    int quit = 0;
    while(!quit) {
        // gera struct de registro e o marca como nao removido
        registro reg;
        reg.removido = 0;

        // le cada linha do csv e armazena em uma string
        char* entrada = malloc(100 * sizeof(char));
        if(fgets(entrada, 100, arq_csv) == NULL) {
            free(entrada);
            break;
        }

        // divide a string lida nos componentes do struct
        reg.nomeTecnologiaOrigem = strtok(entrada, ",");
        reg.grupo = atoi(strtok(NULL, ","));
        reg.popularidade = atoi(strtok(NULL, ","));
        reg.nomeTecnologiaDestino = strtok(NULL, ",");
        reg.peso = atoi(strtok(NULL, ","));

        // preenche  o tamanho dos nomes armazenados
        reg.tamanhoTecnologiaOrigem = strlen(reg.nomeTecnologiaOrigem);
        reg.tamanhoTecnologiaDestino = strlen(reg.nomeTecnologiaDestino);

        // calcula tamanho do lixo e o cria
        escrever_registros(arq_bin, reg);

        // libera a entrada lida
        free(entrada);
    }

    fclose(arq_csv);
    fclose(arq_bin);
}

/**
 * @brief executa a funcionalidade 2, lendo dados de um arquivo em binario e printando seus registros na tela
 */
void funcionalidade2() {
    // recebe o nome do arquivo de entrada
    char nome_bin[30];
    scanf("%s", nome_bin);

    // abre o arquivo de entrada
    FILE* arq_bin = fopen(nome_bin, "rb");

    // pula o registro de cabecalho
    fseek(arq_bin, TAM_HEADER, SEEK_SET);

    // cria struct de registro a ser empregado nas leituras
    registro reg;

    // loop de leitura dos dados
    while(1) {
        // funcao de leitura dos registros
        if(ler_registro(arq_bin, &reg)) {
            // break com fim do arquivo
            break;
        }

        // imprime os dados contidos no registro lido
        printf("%s, %d, %d, %s, %d\n", reg.nomeTecnologiaOrigem, reg.grupo, reg.popularidade, reg.nomeTecnologiaDestino, reg.peso);

        // libera as strings alocadas
        free(reg.nomeTecnologiaOrigem);
        free(reg.nomeTecnologiaDestino);
    }
    // fecha arquivo de dados
    fclose(arq_bin);
}

