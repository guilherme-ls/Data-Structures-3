// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

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
    while(fgetc(arq_csv) != '\n');

    // cria registro de cabecalho
    header cabecalho;
    cabecalho.status = 1;
    cabecalho.proxRRN = 0;
    cabecalho.nroTecnologias = 0;
    cabecalho.nroParesTecnologias = 0;

    // gera struct de registro e o marca como nao removido
    registro reg;
    reg.removido = 0;
    
    char* tec_prev = malloc(100 * sizeof(char));

    escrever_header(arq_bin, cabecalho);
    // executa o loop ate chegar ao fim do arquivo
    while(1) {
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
        
        // salva o nome da tecnologia previa, para avaliacao do numero de tecnologias distintas (o csv providenciado esta ordenado nesse sentido)
        if(cabecalho.nroParesTecnologias != 0) {
            if(strcmp(tec_prev, reg.nomeTecnologiaDestino) != 0) {
                cabecalho.nroTecnologias++;
            }
        }
        strcpy(tec_prev, reg.nomeTecnologiaDestino);

        // preenche  o tamanho dos nomes armazenados
        reg.tamanhoTecnologiaOrigem = strlen(reg.nomeTecnologiaOrigem);
        reg.tamanhoTecnologiaDestino = strlen(reg.nomeTecnologiaDestino);

        // calcula tamanho do lixo e o cria
        escrever_registro(arq_bin, reg);

        // libera a entrada lida
        free(entrada);
        cabecalho.nroParesTecnologias++;

    }
    free(tec_prev);

    // escreve registro de cabecalho atualizado
    fseek(arq_bin, 0, SEEK_SET);
    escrever_header(arq_bin, cabecalho);

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

    // le registro de cabecalho e vai ao primeiro RRN
    header cabecalho;
    if(ler_header(arq_bin, &cabecalho) == 1){
        printf("Registro inexistente.");
        return;
    }
    fseek(arq_bin, calcula_byte_off(cabecalho.proxRRN), SEEK_SET);

    // cria struct de registro a ser empregado nas leituras
    registro reg;

    // loop de leitura e escrita dos dados
    while(1) {
        // funcao de leitura dos registros
        if(ler_registro(arq_bin, &reg)) {
            // break com fim do arquivo
            break;
        }

        // checa se registros de texto sao nulos
        if(reg.tamanhoTecnologiaOrigem == 1) {
            free(reg.nomeTecnologiaOrigem);
            reg.nomeTecnologiaOrigem = malloc(5 * sizeof(char));
            strcpy(reg.nomeTecnologiaOrigem, "NULO");
        }
        if(reg.tamanhoTecnologiaDestino == 1) {
            free(reg.nomeTecnologiaDestino);
            reg.nomeTecnologiaDestino = malloc(5 * sizeof(char));
            strcpy(reg.nomeTecnologiaDestino, "NULO");
        }


        // imprime os dados contidos no registro lido, caso nao removido
        if(reg.removido != 1)
            printf("%s, %d, %d, %s, %d\n", reg.nomeTecnologiaOrigem, reg.grupo, reg.popularidade, reg.nomeTecnologiaDestino, reg.peso);

        // libera as strings alocadas
        free(reg.nomeTecnologiaOrigem);
        free(reg.nomeTecnologiaDestino);
    }
    // fecha arquivo de dados
    fclose(arq_bin);
}

