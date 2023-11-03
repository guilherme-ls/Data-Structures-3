// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#include "funcionalidades_arvores.h"

// define tamanho de strings para leitura do nome de arquivos
#define TAM_ARQ_LEITURA 100


// aqui por enquanto
void concatena_chave(registro reg, char* string) {
    strcpy(string, reg.tecnologiaOrigem.nome);
    strcat(string, reg.tecnologiaDestino.nome);
    for(int i = reg.tecnologiaOrigem.tamanho + reg.tecnologiaDestino.tamanho; i < TAM_CHAVE; i++) {
        string[i] = LIXO_ARVORE;
    }
    string[TAM_CHAVE] = '\0';
}

/**
 * @brief Executa a funcionalidade 1, lendo um dado arquivo csv e convertendo seus dados para registros em binario
 */
void funcionalidade5() {
    // recebe arquivos de entrada e saida
    char nome_dados[TAM_ARQ_LEITURA];
    char nome_arvore[TAM_ARQ_LEITURA];
    scanf("%s", nome_dados);
    scanf("%s", nome_arvore);

    // abre arquivo de saida em modo de escrita
    FILE *arq_arvore;
    if(open(&arq_arvore, nome_arvore, "wb+")) {
        // fim da execucao em caso de erros
        return;
    }

    // abre arquivo de entrada em modo de leitura
    FILE *arq_dados;
    if(open(&arq_dados, nome_dados, "rb")) {
        // fim da execucao em caso de erros
        fclose(arq_arvore);
        return;
    }

    // le registro de cabecalho do arquivo de dados e retorna quaisquer erros
    header cabecalho_dados;
    int erro = check_cabecalho(arq_dados, &cabecalho_dados);
    if(erro == 1) {
        // fim da execucao em caso de erros
        return;
    }

    // inicializa registro de cabecalho da arvore com dados generalizados e o escreve
    header_arvore cabecalho_arvore;
    inicializa_cabecalho_arvore(&cabecalho_arvore);
    escrever_header_arvore(arq_arvore, cabecalho_arvore);

    // cria registro de dados a ser empregado na leitura
    registro reg;

    // inicializa registro raiz da arvore com dados generalizados e o escreve
    registro_arvore reg_arvore;
    inicializa_registro_arvore(&reg_arvore);
    escrever_registro_arvore(arq_arvore, reg_arvore);
    cabecalho_arvore.noRaiz = 0;
    cabecalho_arvore.RRNproxNo = 1;

    // inicializa contador de RRN
    int RRNatual = 0;

    // loop de leitura e escrita dos dados
    while(1) {
        // funcao de leitura dos registros, retornando quando encontra EOF
        int end = ler_registro(arq_dados, &reg);
        if(end) {
            // break com fim do arquivo
            break;
        }

        // imprime os dados contidos no registro lido, caso nao removido
        if(reg.removido != '1') {
            dado data;
            concatena_chave(reg, data.chave);
            data.ponteiro_dado = RRNatual;
            insere_raiz(arq_arvore, &cabecalho_arvore, &data);
        }

        // libera as strings alocadas
        free(reg.tecnologiaOrigem.nome);
        free(reg.tecnologiaDestino.nome);

        // aumenta contador de RRN
        RRNatual++;
    }

    // muda status do cabecalho da arvore com o fim da escrita de dados
    cabecalho_arvore.status = '1';

    // escreve registro de cabecalho da arvore atualizado
    fseek(arq_arvore, 0, SEEK_SET);
    escrever_header_arvore(arq_arvore, cabecalho_arvore);

    // fecha arquivos de dados
    fclose(arq_dados);
    fclose(arq_arvore);

    // aplica a funcao binarioNaTela, como solicitado
    binarioNaTela(nome_arvore);
}