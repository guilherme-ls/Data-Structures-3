// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "funcionalidades_grafo.h"

#define TAM_ARQ_LEITURA 100

void funcionalidadae8(){
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

    //loop de leitura do arquivo de dados e criação do grafo
    while(1){
        // funcao de leitura dos registros, retornando 1 quando encontra EOF
        int end = ler_registro(arq_dados, &reg);
        if (end) {
            // break com fim do arquivo
            break;
        }

        
    }

}