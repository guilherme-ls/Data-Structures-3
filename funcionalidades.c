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
    if(arq_bin == NULL) {
        printf("Falha no processamento do arquivo.");
        exit(EXIT_FAILURE);
    }

    // abre arquivo de entrada em modo de leitura
    FILE *arq_csv = fopen(nome_csv, "r");
    if(arq_csv == NULL) {
        printf("Falha no processamento do arquivo.");
        exit(EXIT_FAILURE);
    }

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

    // checa se houveram erros na escrita do header
    if(escrever_header(arq_bin, cabecalho) == 1) {
        printf("Falha no processamento do arquivo.");
        return;
    }

    char* tec_prev = malloc(100 * sizeof(char));

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

        // escreve o registro e checa erros
        if(escrever_registro(arq_bin, reg) == 1) {
            printf("Falha no processamento do arquivo.");
            return;
        }

        // libera a entrada lida
        free(entrada);
        cabecalho.nroParesTecnologias++;

    }
    free(tec_prev);

    // escreve registro de cabecalho atualizado e checa erros
    fseek(arq_bin, 0, SEEK_SET);
    if(escrever_header(arq_bin, cabecalho) == 1) {
        printf("Falha no processamento do arquivo.");
        return;
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
    if(arq_bin == NULL) {
        printf("Falha no processamento do arquivo.");
        exit(EXIT_FAILURE);
    }

    // le registro de cabecalho e vai ao primeiro RRN, retorna quaisquer erros
    header cabecalho;
    int saida = ler_header(arq_bin, &cabecalho);
    if(saida == 1) {
        printf("Registro inexistente.");
        return;
    }
    else if(saida == 2) {
        printf("Falha no processamento do arquivo.");
        return;
    }
    fseek(arq_bin, calcula_byte_off(cabecalho.proxRRN), SEEK_SET);

    // cria struct de registro a ser empregado nas leituras
    registro reg;

    // loop de leitura e escrita dos dados
    while(1) {
        // funcao de leitura dos registros, retornando erros
        int saida = ler_registro(arq_bin, &reg);
        if(saida == 1) {
            // break com fim do arquivo
            break;
        }
        else if(saida == 2) {
            printf("Falha no processamento do arquivo.");
            return;
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


/**
 * @brief executa a funcionalidade 3, recebendo a quantidade de execuções e lendo nomes e valores de campo para realizar busca no arquivo de dados a ser lido,
          retornando os registros que satisfazem a busca ou as respectivas exceções.
 */
void funcionalidade3(){
    char nome_bin[30]; // Nome do arquivo binario
    char nomeCampo[30]; // nome do campo a ser buscado
    char* valorCampoBuscado = malloc(80 *sizeof(char)); // valor do campo a ser buscado
    int n; // Quantidade de busca;

    // Recebe o nome do arquivo de entrada e a quantidade de valores para buscar
    scanf("%s %d", nome_bin, &n);  

    // abre o arquivo de entrada
    FILE* arq_bin = fopen(nome_bin, "rb");

    if(arq_bin == NULL) {
        printf("Falha no processamento do arquivo.");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < n; i++){
        
        // Recebo o nome e valor do campo a serem buscados
        scanf("%s \"%s", nomeCampo, valorCampoBuscado);
        valorCampoBuscado = strtok(valorCampoBuscado, "\"");

        
        // le registro de cabecalho e vai ao primeiro RRN, retorna quaisquer erros
        header cabecalho;
        int saida = ler_header(arq_bin, &cabecalho);
        if(saida == 1) {
            printf("Registro inexistente.");
            return;
        }
        else if(saida == 2) {
            printf("Falha no processamento do arquivo.");
            return;
        }
        

        // variaveis de apoio
        registro reg; // registro a ser devolvido
        char valorCampoAtual[80]; // valor do campo sendo lido no momento
        int contRRN = 0; // valor do RRN do registro a ser lido
        int contArquivosEncontrados = 0; // quantidade de arquivos que satisfazem busca
        while(1){
            fseek(arq_bin, calcula_byte_off(contRRN), SEEK_SET);
            int saida = ler_campo(arq_bin, valorCampoAtual, nomeCampo);
            if(saida == 1) {
                // break com fim do arquivo
                break;
            }
            else if(saida == 2) {
                printf("Falha no processamento do arquivo.");
                return;
            }

            // verifica se o registro atual satisfaz a busca
            if(strcmp(valorCampoBuscado, valorCampoAtual)){ 
                ler_registro(arq_bin, &reg); // lê registro atual 
                
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

            contRNN++; // Acresce para busca no proximo registro.
        }
    }
}    char nomeCampo[30]; // nome do campo a ser buscado


funcionalidade4(){
    char nome_bin[30]; // Nome do arquivo binario
    int RRNbuscado; // Quantidade de busca;

    // Recebe o nome do arquivo de entrada e a quantidade de valores para buscar
    scanf("%s %d", nome_bin, &RRNbuscado);  

    // abre o arquivo de entrada
    FILE* arq_bin = fopen(nome_bin, "rb");

    if(arq_bin == NULL) {
        printf("Falha no processamento do arquivo.");
        exit(EXIT_FAILURE);
    }

    fseek(arq_bin, calcula_byte_off(RRNbuscado), SEEK_SET);

    // cria struct de registro a ser empregado nas leituras
    registro reg;

    ler_registro(arq_bin, &reg); // lê registro atual 
                
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
    else{
        printf("Registro inexistente.\n")
    }

    // libera as strings alocadas
    free(reg.nomeTecnologiaOrigem);
    free(reg.nomeTecnologiaDestino);

}

