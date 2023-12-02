// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#include "funcionalidades_arvores.h"

// define tamanho de strings para leitura do nome de arquivos
#define TAM_ARQ_LEITURA 100

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
    if (open(&arq_arvore, nome_arvore, "wb+")) {
        // fim da execucao em caso de erros
        return;
    }

    // abre arquivo de entrada em modo de leitura
    FILE *arq_dados;
    if (open(&arq_dados, nome_dados, "rb")) {
        // fim da execucao em caso de erros
        fclose(arq_arvore);
        return;
    }

    // le registro de cabecalho do arquivo de dados e retorna quaisquer erros
    header cabecalho_dados;
    int erro = check_cabecalho(arq_dados, &cabecalho_dados);
    if (erro == 1) {
        // fim da execucao em caso de erros
        return;
    }

    // inicializa registro de cabecalho da arvore com dados generalizados e o escreve
    header_arvore cabecalho_arvore;
    inicializa_cabecalho_arvore(&cabecalho_arvore);
    escrever_header_arvore(arq_arvore, cabecalho_arvore);

    // cria registro de dados a ser empregado na leitura
    registro reg;

    // inicializa registro raiz da arvore com dados generalizados e o escreve (sempre o deixa em memoria primaria)
    registro_arvore reg_arvore;
    inicializa_registro_arvore(&reg_arvore);
    escrever_registro_arvore(arq_arvore, reg_arvore);
    cabecalho_arvore.noRaiz = 0;
    cabecalho_arvore.RRNproxNo = 1;

    // inicializa contador de RRN
    int RRNatual = 0;

    // loop de leitura e escrita dos dados
    while (1) {
        // funcao de leitura dos registros, retornando 1 quando encontra EOF
        int end = ler_registro(arq_dados, &reg);
        if (end) {
            // break com fim do arquivo
            break;
        }

        // insere na arvore a chave do registro lido, caso nao removido e caso a chave seja valida
        if (reg.removido != '1' && reg.tecnologiaDestino.tamanho != 0 && reg.tecnologiaOrigem.tamanho != 0) {
            dado data;
            concatena_chave(reg, data.chave);
            data.ponteiro_dado = RRNatual;
            insere_raiz(arq_arvore, &cabecalho_arvore, &data, &reg_arvore);
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

/**
 * @brief Executa a funcionalidade 6, lendo dois arquivos (dados e indice), realizando n buscas solicitadas, sendo no arquivo de indice, quando possível, ou no de dados.
 */
void funcionalidade6()
{
    // Inicialização de variáveis
    char nome_dados[TAM_ARQ_LEITURA];   // Nome do arquivo binario de dados
    char nome_arvore[TAM_ARQ_LEITURA];  // Nome do arquivo binario do indice
    char nomeCampo[TAM_ARQ_LEITURA];    // nome do campo a ser buscado
    int n;                              // Quantidade de buscas;
    int primeiraBuscaArvore = 1;        // variavel de controle para pôr no raiz em memoria primaria
    int primeiraBuscaDados = 1;         // variavel de controle para posicionar cursos no inicio do arquivo de dados;
    registro_arvore raiz_arvore;        // armazena em memoria primaria a raiz da arvore

    // Recebe o nome do arquivo de entrada e a quantidade de valores para buscar
    scanf("%s %s %d", nome_dados, nome_arvore, &n);

    // abre o arquivo de dados
    FILE *arq_dados;
    if (open(&arq_dados, nome_dados, "rb")) {
        // fim da execucao em caso de erros
        return;
    }

    // le registro de cabecalho dos dados e retorna quaisquer erros
    header cabecalho_dados;
    int erro;
    erro = check_cabecalho(arq_dados, &cabecalho_dados);
    if (erro == 1) {
        // fim da execucao em caso de erros
        return;
    }

    // abre o arquivo de indice (arvore B)
    FILE *arq_arvore;
    if (open(&arq_arvore, nome_arvore, "rb")) {
        // fim da execucao em caso de erros
        fclose(arq_dados);
        return;
    }

    header_arvore cabecalho_arvore;
    erro = check_cabecalho_arvore(arq_arvore, &cabecalho_arvore);
    if (erro == 1) {
        // fim da execucao em caso de erro
        fclose(arq_dados);
        return;
    }

    // declara variáveis a serem empregadas no loop de leitura
    char *temp = malloc(160 * sizeof(char)); // valor temporario do campo a ser buscado
    char *valorCampoBuscado;        // ponteiro para armazenamento do campo buscado apos

    for (int i = 0; i < n; i++) {
        // recebe o nome e valor do campo a serem buscados
        scanf("%s", nomeCampo);
        scanf("%s", temp);
        valorCampoBuscado = strtok(temp, "\"");

        // Verifica se a busca deve ser feita no arquivo de indice ou diretamente no
        // arquivo de dados
        if (strcmp(nomeCampo, "nomeTecnologiaOrigemDestino") == 0) {
            // Busca em arquivo de indice
            busca_em_indice(arq_arvore, arq_dados, valorCampoBuscado, cabecalho_arvore, &primeiraBuscaArvore, &raiz_arvore);
        }
        else {
            // busca direta em arquivo de dados
            if (primeiraBuscaDados == 0 || primeiraBuscaArvore == 0) {
                // Leva cursor para inicio do arquivo de dados
                fseek(arq_dados, TAM_HEADER, SEEK_SET);
            }
            if (primeiraBuscaDados == 1)
                primeiraBuscaDados = 0;

            int end = busca_em_arq_dados(arq_dados, nomeCampo, valorCampoBuscado);
            if (end == 2) {
                printf("Falha no processamento do arquivo.");
                free(temp);
                fclose(arq_dados);
                fclose(arq_arvore);
                return;
            }
        }
    }
    // Fim da funcionalidade. Libera memória e fecha arquivos.
    free(temp);
    fclose(arq_dados);
    fclose(arq_arvore);
}

void funcionalidade7()
{
    char nome_dados[TAM_ARQ_LEITURA];  // Nome do arquivo binario de dados
    char nome_arvore[TAM_ARQ_LEITURA]; // Nome do arquivo binario do indice

    int n; // Quantidade de inserções;

    // Recebe o nome do arquivo de entrada e a quantidade de valores para inserir
    scanf("%s %s %d", nome_dados, nome_arvore, &n);

    // abre o arquivo de dados
    FILE* arq_dados;
    if(open(&arq_dados, nome_dados, "rb+")) {
        // fim da execucao em caso de erros
        return;
    }

    // le registro de cabecalho dos dados e retorna quaisquer erros
    header cabecalho_dados;
    int erro;
    erro = check_cabecalho(arq_dados, &cabecalho_dados);
    if (erro == 1) {
        // fim da execucao em caso de erros
        return;
    }

    // abre o arquivo de indice (arvore B)
    FILE* arq_arvore;
    if(open(&arq_arvore, nome_arvore, "rb+")) {
        // fim da execucao em caso de erros
        fclose(arq_dados);
        return;
    }

    header_arvore cabecalho_arvore;
    erro = check_cabecalho_arvore(arq_arvore, &cabecalho_arvore);
    if (erro == 1) {
        // fim da execucao em caso de erro
        fclose(arq_dados);
        return;
    }

    // Muda o status dos cabeçalhos para incio da escrita
    cabecalho_dados.status = '0';
    cabecalho_arvore.status = '0';

    // escreve os registros de cabecalho atualizados
    fseek(arq_arvore, 0, SEEK_SET);
    escrever_header_arvore(arq_arvore, cabecalho_arvore);
    fseek(arq_dados, 0, SEEK_SET);
    escrever_header(arq_dados, cabecalho_dados);

    // cria lista para serem inseridas as tecnologias lidas
    lista l;
    inicializa_lista(&l);

    // Leitura completa do arquivo de dados para extrair quantidade de tecnologias
    le_arquivo_de_dados(arq_dados, &l);

    // busca registro raiz da arvore em memoria primaria para insercao
    registro_arvore reg_arvore;
    pega_raiz(arq_arvore, cabecalho_arvore, &reg_arvore);

    // Loop para efetuar inserção.
    for (int i = 0; i < n; i++) {
        // Inserção no arquivo de dados:

        // lê a entrada e atualiza registro
        registro reg;
        inicializa_registro(&reg);
        entrada_para_registro(&reg);

        // escreve o registro
        escrever_registro(arq_dados, reg);

        // salva o nome de cada tecnologia em uma lista sem repeticoes
        if(reg.tecnologiaOrigem.tamanho != 0)
            inserir(&l, reg.tecnologiaOrigem.nome);
        if(reg.tecnologiaDestino.tamanho != 0)
            inserir(&l, reg.tecnologiaDestino.nome);

        // atualiza informacoes do cabecalho
        if (reg.tecnologiaOrigem.tamanho != 0 && reg.tecnologiaDestino.tamanho != 0)
            cabecalho_dados.nroParesTecnologias++;

        cabecalho_dados.proxRRN++;

        // Inserção no arquivo de indice:
        if(reg.tecnologiaDestino.tamanho != 0 && reg.tecnologiaOrigem.tamanho != 0) {
            dado data;
            concatena_chave(reg, data.chave);
            data.ponteiro_dado = cabecalho_dados.proxRRN - 1;
            insere_raiz(arq_arvore, &cabecalho_arvore, &data, &reg_arvore);
        }

        // libera as strings alocadas
        if(reg.tecnologiaOrigem.tamanho != 0)
            free(reg.tecnologiaOrigem.nome);
        if(reg.tecnologiaDestino.tamanho != 0)
            free(reg.tecnologiaDestino.nome);
    }

    // define numero de tecnologias distintas com base no tamanho da lista criada (e destroi lista)
    cabecalho_dados.nroTecnologias = l.tam;
    destruir(&l);

    // Muda o status dos cabeçalhos com o fim da escrita
    cabecalho_dados.status = '1';
    cabecalho_arvore.status = '1';

    // escreve os registros de cabecalho atualizados
    fseek(arq_arvore, 0, SEEK_SET);
    escrever_header_arvore(arq_arvore, cabecalho_arvore);
    fseek(arq_dados, 0, SEEK_SET);
    escrever_header(arq_dados, cabecalho_dados);

    // fecha arquivos
    fclose(arq_dados);
    fclose(arq_arvore);

    // aplica a funcao binarioNaTela, como solicitado
    binarioNaTela(nome_dados);
    binarioNaTela(nome_arvore);
}