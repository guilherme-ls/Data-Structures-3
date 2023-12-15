// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "funcionalidades_grafo.h"
#include "funcoesFornecidas.h"

#define TAM_ARQ_LEITURA 100

void funcionalidade8() {
    char nome_dados[TAM_ARQ_LEITURA];
    scanf("%s", nome_dados);

    // abre arquivo de entrada em modo de leitura
    FILE *arq_dados;
    if (open(&arq_dados, nome_dados, "rb")) {
        // fim da execucao em caso de erros
        return;
    }

    // le registro de cabecalho do arquivo de dados e retorna quaisquer erros
    header cabecalho_dados;
    int erro = check_cabecalho(arq_dados, &cabecalho_dados);
    if (erro == 1) {
        // fim da execucao em caso de erros
        return;
    }

    registro reg;  // Registro de dados a ser empregrado na leitura

    // cria e inicializa grafo
    grafo g;
    inicializa_grafo(&g, cabecalho_dados.nroTecnologias);

    //loop de leitura do arquivo de dados e criação do grafo
    while(1) {
        // funcao de leitura dos registros, retornando 1 quando encontra EOF
        int end = ler_registro(arq_dados, &reg);
        if (end) {
            // break com fim do arquivo
            break;
        }

        // insere aresta lida no grafo, caso valida
        if(reg.tecnologiaOrigem.tamanho != 0 && reg.tecnologiaDestino.tamanho != 0)
            insere_aresta(&g, reg.tecnologiaOrigem, reg.grupo, reg.tecnologiaDestino, 0, reg.peso);

        // libera variaveis alocadas
        free(reg.tecnologiaOrigem.nome);
        free(reg.tecnologiaDestino.nome);
    }
    // printa o grafo criado
    print_grafo(g);

    // fecha arquivo de dados e libera grafo
    fclose(arq_dados);
    libera_grafo(g);
}


void funcionalidade9() {
    char nome_dados[TAM_ARQ_LEITURA];
    scanf("%s", nome_dados);

    // abre arquivo de entrada em modo de leitura
    FILE *arq_dados;
    if (open(&arq_dados, nome_dados, "rb")) {
        // fim da execucao em caso de erros
        return;
    }

    // le registro de cabecalho do arquivo de dados e retorna quaisquer erros
    header cabecalho_dados;
    int erro = check_cabecalho(arq_dados, &cabecalho_dados);
    if (erro == 1) {
        // fim da execucao em caso de erros
        return;
    }

    registro reg;  // Registro de dados a ser empregrado na leitura

    // cria e inicializa grafo
    grafo g;
    inicializa_grafo(&g, cabecalho_dados.nroTecnologias);

    //loop de leitura do arquivo de dados e criação do grafo
    while(1) {
        // funcao de leitura dos registros, retornando 1 quando encontra EOF
        int end = ler_registro(arq_dados, &reg);
        if (end) {
            // break com fim do arquivo
            break;
        }

        // insere aresta lida no grafo, invertendo-a para o grafo transposto
        if(reg.tecnologiaOrigem.tamanho != 0 && reg.tecnologiaDestino.tamanho != 0)
            insere_aresta(&g, reg.tecnologiaDestino, 0, reg.tecnologiaOrigem, reg.grupo, reg.peso);

        // libera variaveis alocadas
        free(reg.tecnologiaOrigem.nome);
        free(reg.tecnologiaDestino.nome);
    }
    // printa o grafo criado
    print_grafo(g);

    // fecha arquivo de dados e libera grafo
    fclose(arq_dados);
    libera_grafo(g);
}
void funcionalidade10() {
    char nome_dados[TAM_ARQ_LEITURA];
    int numero_buscas;
    scanf("%s", nome_dados);
    scanf("%d", &numero_buscas);

    // abre arquivo de entrada em modo de leitura
    FILE *arq_dados;
    if (open(&arq_dados, nome_dados, "rb")) {
        // fim da execucao em caso de erros
        return;
    }

    // le registro de cabecalho do arquivo de dados e retorna quaisquer erros
    header cabecalho_dados;
    int erro = check_cabecalho(arq_dados, &cabecalho_dados);
    if (erro == 1) {
        // fim da execucao em caso de erros
        return;
    }

    registro reg;  // Registro de dados a ser empregrado na leitura

    // cria e inicializa grafo
    grafo g;
    inicializa_grafo(&g, cabecalho_dados.nroTecnologias);

    //loop de leitura do arquivo de dados e criação do grafo
    while(1) {
        // funcao de leitura dos registros, retornando 1 quando encontra EOF
        int end = ler_registro(arq_dados, &reg);
        if (end) {
            // break com fim do arquivo
            break;
        }

        // insere aresta lida no grafo, caso valida
        if(reg.tecnologiaOrigem.tamanho != 0 && reg.tecnologiaDestino.tamanho != 0)
            insere_aresta(&g, reg.tecnologiaDestino, 0, reg.tecnologiaOrigem, reg.grupo, reg.peso);

        // libera variaveis alocadas
        free(reg.tecnologiaOrigem.nome);
        free(reg.tecnologiaDestino.nome);
    }
    
    for(int i = 0; i < numero_buscas; i++) {
        char tecnologia_buscada[TAM_ARQ_LEITURA];
        scan_quote_string(tecnologia_buscada);

        busca_tecnologias_entrada(g, tecnologia_buscada);
    }

    // fecha arquivo de dados e libera grafo
    fclose(arq_dados);
    libera_grafo(g);
}
void funcionalidade11() {
    ;
}
void funcionalidade12() {
    char nome_dados[TAM_ARQ_LEITURA];
    int numero_buscas;
    scanf("%s", nome_dados);
    scanf("%d", &numero_buscas);

    // abre arquivo de entrada em modo de leitura
    FILE *arq_dados;
    if (open(&arq_dados, nome_dados, "rb")) {
        // fim da execucao em caso de erros
        return;
    }

    // le registro de cabecalho do arquivo de dados e retorna quaisquer erros
    header cabecalho_dados;
    int erro = check_cabecalho(arq_dados, &cabecalho_dados);
    if (erro == 1) {
        // fim da execucao em caso de erros
        return;
    }

    registro reg;  // Registro de dados a ser empregrado na leitura

    // cria e inicializa grafo
    grafo g;
    inicializa_grafo(&g, cabecalho_dados.nroTecnologias);

    //loop de leitura do arquivo de dados e criação do grafo
    while(1) {
        // funcao de leitura dos registros, retornando 1 quando encontra EOF
        int end = ler_registro(arq_dados, &reg);
        if (end) {
            // break com fim do arquivo
            break;
        }

        // insere aresta lida no grafo, caso valida
        if(reg.tecnologiaOrigem.tamanho != 0 && reg.tecnologiaDestino.tamanho != 0)
            insere_aresta(&g, reg.tecnologiaDestino, 0, reg.tecnologiaOrigem, reg.grupo, reg.peso);

        // libera variaveis alocadas
        free(reg.tecnologiaOrigem.nome);
        free(reg.tecnologiaDestino.nome);
    }

    for(int i = 0; i < numero_buscas; i++){
        char tecnologia_origem[TAM_ARQ_LEITURA]; 
        char tecnologia_destino[TAM_ARQ_LEITURA];
        scan_quote_string(tecnologia_origem);
        scan_quote_string(tecnologia_destino);

        int distancia = dijkstra(g, tecnologia_origem, tecnologia_destino);
        if(distancia < INFINITO){
            printf("%s %s: %d\n", tecnologia_origem, tecnologia_destino, distancia);
        }else{
            printf("%s %s: CAMINHO INEXISTENTE\n", tecnologia_origem, tecnologia_destino);
        }
        

    }
}