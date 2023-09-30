// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registros.h"
#include "funcionalidades.h"
#include "funcoesAuxiliares.h"
#include "funcoesFornecidas.h"
#include "lista.h"

// define tamanho de strings para leitura do nome de arquivos
#define TAM_ARQ_LEITURA 100

/**
 * @brief Executa a funcionalidade 1, lendo um dado arquivo csv e convertendo seus dados para registros em binario
 */
void funcionalidade1() {
    // recebe arquivos de entrada e saida
    char nome_csv[TAM_ARQ_LEITURA];
    char nome_bin[TAM_ARQ_LEITURA];
    scanf("%s", nome_csv);
    scanf("%s", nome_bin);

    // abre arquivo de saida em modo de escrita
    FILE *arq_bin = fopen(nome_bin, "wb");
    if(arq_bin == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // abre arquivo de entrada em modo de leitura
    FILE *arq_csv = fopen(nome_csv, "r");
    if(arq_csv == NULL) {
        printf("Falha no processamento do arquivo.\n");
        fclose(arq_bin);
        return;
    }

    // pula o cabecalho do csv lido
    while(fgetc(arq_csv) != '\n');

    // cria registro de cabecalho e o inicializa com dados generalizados
    header cabecalho;
    inicializa_cabecalho(&cabecalho);

    // cria registro de dados e o inicializa com dados generalizados
    registro reg;
    inicializa_registro(&reg);

    // escreve o header
    escrever_header(arq_bin, cabecalho);

    // cria lista para serem inseridas as tecnologias lidas
    lista l;
    inicializa_lista(&l);

    // executa o loop ate chegar ao fim do arquivo
    while(1) {
        // le cada linha do csv e armazena em uma string (sai do loop caso encontre EOF)
        char* entrada = malloc(100 * sizeof(char));
        if(le_entrada(entrada, arq_csv)){
            break;
        }

        // divide a string lida nos componentes do struct
        divide_string(&reg, entrada);
        
        // salva o nome de cada tecnologia em uma lista sem repeticoes
        if(reg.tecnologiaOrigem.tamanho != 0)
            inserir(&l, reg.tecnologiaOrigem.nome);
        if(reg.tecnologiaDestino.tamanho != 0)
            inserir(&l, reg.tecnologiaDestino.nome);

        // escreve o registro
        escrever_registro(arq_bin, reg);

        // libera a entrada lida
        free(entrada);

        // atualiza informacoes do cabecalho
        if(reg.tecnologiaOrigem.tamanho != 0 && reg.tecnologiaDestino.tamanho != 0)
            cabecalho.nroParesTecnologias++;
        
        cabecalho.proxRRN++;
    }
    // define numero de tecnologias distintas com base no tamanho da lista criada (e destroi lista)
    cabecalho.nroTecnologias = l.tam;
    destruir(&l);

    // muda status do cabecalho com fim da escrita de dados
    cabecalho.status = '1';

    // escreve registro de cabecalho atualizado
    fseek(arq_bin, 0, SEEK_SET);
    escrever_header(arq_bin, cabecalho);

    // fecha arquivos de dados
    fclose(arq_csv);
    fclose(arq_bin);

    // aplica a funcao binarioNaTela, como solicitado
    binarioNaTela(nome_bin);
}

/**
 * @brief executa a funcionalidade 2, lendo dados de um arquivo em binario e printando seus registros na tela
 */
void funcionalidade2() {
    // recebe o nome do arquivo de entrada
    char nome_bin[TAM_ARQ_LEITURA];
    scanf("%s", nome_bin);

    // abre o arquivo de entrada
    FILE* arq_bin = fopen(nome_bin, "rb");
    if(arq_bin == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // le registro de cabecalho e vai ao primeiro RRN, retorna quaisquer erros
    header cabecalho;
    int erro = ler_header(arq_bin, &cabecalho);
    if(erro) {
        printf("Registro inexistente.\n");
        return;
    }
    
    // cria struct de registro a ser empregado nas leituras
    registro reg;

    // loop de leitura e escrita dos dados
    while(1) {
        // funcao de leitura dos registros, retornando quando encontra EOF
        int end = ler_registro(arq_bin, &reg);
        if(end) {
            // break com fim do arquivo
            break;
        }

        // imprime os dados contidos no registro lido, caso nao removido
        if(reg.removido != '1')
            imprime_registro(reg);

        // libera as strings alocadas
        free(reg.tecnologiaOrigem.nome);
        free(reg.tecnologiaDestino.nome);
    }
    // fecha arquivo de dados
    fclose(arq_bin);
}


/**
 * @brief executa a funcionalidade 3, recebendo a quantidade de execuções e lendo nomes e valores de campo para realizar busca no arquivo de dados a ser lido,
          retornando os registros que satisfazem a busca ou as respectivas exceções.
 */
void funcionalidade3(){
    char nome_bin[TAM_ARQ_LEITURA]; // Nome do arquivo binario
    char nomeCampo[TAM_ARQ_LEITURA]; // nome do campo a ser buscado
    int n; // Quantidade de busca;

    // Recebe o nome do arquivo de entrada e a quantidade de valores para buscar
    scanf("%s %d", nome_bin, &n);  

    // abre o arquivo de entrada
    FILE* arq_bin = fopen(nome_bin, "rb");
    // checa erros na abertura do arquivo
    if(arq_bin == NULL) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // le registro de cabecalho e vai ao primeiro RRN, retorna quaisquer erros
    header cabecalho;
    int saida = ler_header(arq_bin, &cabecalho);
    if(saida == 1) {
        printf("Falha no processamento do arquivo.\n");
        fclose(arq_bin);
        return;
    }
    
    // retorna erro caso 'status' do arquivo lido seja '0'
    if(cabecalho.status == '0') {
        printf("Falha no processamento do arquivo.\n");
        fclose(arq_bin);
        return;
    }

    // declara variáveis a serem empregadas no loop de leitura
    char* temp = malloc(80 *sizeof(char)); // valor temporario do campo a ser buscado
    char* valorCampoBuscado; // ponteiro para armazenamento do campo buscado apos remocao de ""

    // repete o processo de busca para cada campo distinto a ser avaliado 
    for(int i = 0; i < n; i++){
        // Recebo o nome e valor do campo a serem buscados
        scanf("%s", nomeCampo);
        scanf("%s", temp);

        valorCampoBuscado = strtok(temp, "\"");

        // variaveis de apoio
        registro reg; // registro a ser devolvido
        char* valorCampoAtual = malloc(sizeof(char) * 80); // valor do campo sendo lido no momento
        int contRRN = 0; // valor do RRN do registro a ser lido
        int contBuscado = 0; // Quantidade de registros que satisfazem busca
        while(1){
            // vai a posicao do RRN especificado
            fseek(arq_bin, calcula_byte_off(contRRN), SEEK_SET);
            int saida = ler_campo(arq_bin, &valorCampoAtual, nomeCampo);
            if(saida == 1) {
                // break com fim do arquivo
                break;
            }

            // Erro encontrado durante a busca no arquivo
            else if(saida == 2) {
                printf("Falha no processamento do arquivo.");
                free(valorCampoAtual); 
                free(temp);
                fclose(arq_bin);
                return;
            }
            
            // verifica se o registro atual satisfaz a busca
            if(strcmp(valorCampoBuscado, valorCampoAtual) == 0){
                contBuscado++;
                fseek(arq_bin, calcula_byte_off(contRRN), SEEK_SET);
                int end = ler_registro(arq_bin, &reg); // lê registro atual 
                if(end) {
                    // break com fim do arquivo
                    break;
                }


                // imprime os dados contidos no registro lido, caso nao removido
                if(reg.removido != 1)
                    imprime_registro(reg);

                // libera as strings alocadas
                free(reg.tecnologiaOrigem.nome);
                free(reg.tecnologiaDestino.nome);
            }
            // Acresce para busca no proximo registro.
            contRRN++;
        }
        if(contBuscado == 0){
            printf("Registro inexistente.\n");
        }
        free(valorCampoAtual);
        if(i < n-1){
            fseek(arq_bin, TAM_HEADER, SEEK_SET);
        }
    }
    free(temp);
    fclose(arq_bin); // Fechar arquivo
}   

void funcionalidade4(){
    char nome_bin[TAM_ARQ_LEITURA]; // Nome do arquivo binario
    int RRNbuscado; // Quantidade de busca;

    // Recebe o nome do arquivo de entrada e a quantidade de valores para buscar
    scanf("%s %d", nome_bin, &RRNbuscado);  

    // abre o arquivo de entrada
    FILE* arq_bin = fopen(nome_bin, "rb");

    if(arq_bin == NULL) {
        printf("Falha no processamento do arquivo.\n");
        exit(EXIT_FAILURE);
    }

    // Busca registro no RRN especificado
    fseek(arq_bin, calcula_byte_off(RRNbuscado), SEEK_SET);

    // cria struct de registro a ser empregado nas leituras
    registro reg;

    // lê registro atual
    int end = ler_registro(arq_bin, &reg);
    if(end){
        printf("Registro inexistente.\n");
        fclose(arq_bin);

        // break com fim do arquivo
        return;
    }

    // imprime os dados contidos no registro lido, caso nao removido
    if(reg.removido != 1)
        imprime_registro(reg);
    else {
        printf("Registro inexistente.\n");
    }

    // libera as strings alocadas
    free(reg.tecnologiaOrigem.nome);
    free(reg.tecnologiaDestino.nome);
    fclose(arq_bin);
}