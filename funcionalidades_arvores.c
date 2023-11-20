// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#include "funcionalidades_arvores.h"
#include "funcoesAuxiliares.h"

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

void funcionalidade6(){
    char nome_dados[TAM_ARQ_LEITURA];  // Nome do arquivo binario de dados
    char nome_arvore[TAM_ARQ_LEITURA];  // Nome do arquivo binario do indice
    char nomeCampo[TAM_ARQ_LEITURA];  // nome do campo a ser buscado
    int n;  // Quantidade de busca;
    int primeiraBuscaArvore = 1;  // variavel de controle para pôr no raiz em memoria primaria
    int primeiraBuscaDados = 1;  // variavel de controle para posicionar cursos no inicio do arquivo de dados;
    registro_arvore reg_raiz_arvore;  // variavel para armazenar no raiz da arvore
    registro_arvore reg_no_atual;  // variavel para armazenar no atual da busca na arvore

    // Recebe o nome do arquivo de entrada e a quantidade de valores para buscar
    scanf("%s %s %d", nome_dados, nome_arvore, &n);  

    // abre o arquivo de dados
    FILE* arq_dados;
    if(open(&arq_dados, nome_dados, "rb")) {
        // fim da execucao em caso de erros
        return;
    }

    // le registro de cabecalho dos dados e retorna quaisquer erros
    header cabecalho_dados;
    int erro;
    erro = check_cabecalho(arq_dados, &cabecalho_dados);
    if(erro == 1) {
        // fim da execucao em caso de erros
        return;
    }

    // abre o arquivo de indice (arvore B)
    FILE* arq_arvore;
    if(open(&arq_arvore, nome_arvore, "rb")) {
        // fim da execucao em caso de erros
        fclose(arq_dados);
        return;
    }

    header_arvore cabecalho_arvore;
    erro = check_cabecalho_arvore(arq_arvore, &cabecalho_arvore);
    if(erro == 1){
        // fim da execucao em caso de erro
        fclose(arq_dados);
        return;
    }

    // declara variáveis a serem empregadas no loop de leitura
    char* temp = malloc(160 *sizeof(char)); // valor temporario do campo a ser buscado
    char* valorCampoBuscado; // ponteiro para armazenamento do campo buscado apos remocao de ""
    char* chaveAtual;  // Valor para chave do indice sem o lixo

    for(int i = 0; i < n; i++){
        // recebe o nome e valor do campo a serem buscados
        scanf("%s", nomeCampo);
        scanf("%s", temp);

        valorCampoBuscado = strtok(temp, "\"");

        // Verifica se a busca deve ser feita no arquivo de indice ou diretamente no arquivo de dados
        if(strcmp(nomeCampo, "nomeTecnologiaOrigemDestino") == 0){

            // busca em arquivo de indice (arvore)
            int regEncontrado = 1; // controle para saber se registro foi encontrado
            chaveAtual = malloc(TAM_CHAVE+1 * sizeof(char)); // Valor para chave do indice sem o lixo

            if(primeiraBuscaArvore){
                // trazer raiz da arvore para memória primária
                fseek(arq_arvore, calcula_byte_off_arvore(cabecalho_arvore.noRaiz), SEEK_SET); // posiciona cursor para no raiz
                int check = ler_registro_arvore(arq_arvore, &reg_raiz_arvore);
                primeiraBuscaArvore = 0;
                if(check == 1){
                    // arquivo vazio de registro de dados.
                    printf("Registro inexistente.\n");
                    break;
                }
            }else{
                // Leva cursor para o primeiro registro do arquivo de indice
                //fseek(arq_arvore, TAM_HEADER_ARVORE, SEEK_SET);
            }

            reg_no_atual = reg_raiz_arvore; // começa busca pela raiz

            int iBuscaAtual = busca_binaria_reg_arvore(valorCampoBuscado, reg_no_atual); // Variavel para indice retornado na busca binaria

            ler_chave_sem_lixo(reg_no_atual.dados[iBuscaAtual].chave, chaveAtual);
        
            //Busca no arquivo de indice (arvore B)
            while(strcmp(valorCampoBuscado, chaveAtual) != 0){
                free(chaveAtual);
                chaveAtual = malloc(TAM_CHAVE+1 * sizeof(char)); // Valor para chave do indice sem o lixo

                // garantir que estamo na posicao do maior elemento de chave maior que a atual
                if(iBuscaAtual < reg_no_atual.nroChavesNo && strcmp(valorCampoBuscado, reg_no_atual.dados[iBuscaAtual].chave) > 0){
                    iBuscaAtual += 1;
                }

                if(reg_no_atual.ponteiro_arvore[iBuscaAtual] == -1){
                    // Não existe subárvore, fim da busca.
                    printf("Registro inexistente.\n");
                    regEncontrado = 0;
                    break;
                }
                fseek(arq_arvore, (reg_no_atual.ponteiro_arvore[iBuscaAtual] + 1) * TAM_REG_ARVORE, SEEK_SET);
                int check = ler_registro_arvore(arq_arvore, &reg_no_atual);
                if(check == 1){
                    // fim do arquivo
                    printf("Registro inexistente.\n");
                    regEncontrado = 0; 
                    break;
                }
                iBuscaAtual = busca_binaria_reg_arvore(valorCampoBuscado, reg_no_atual);
                ler_chave_sem_lixo(reg_no_atual.dados[iBuscaAtual].chave, chaveAtual);
            }
            if(regEncontrado){
                registro regBuscado;
                int RRNDado = reg_no_atual.dados[iBuscaAtual].ponteiro_dado; // RRN do registro buscado no arquivo de dados
                fseek(arq_dados, calcula_byte_off(RRNDado), SEEK_SET);
                int end = ler_registro(arq_dados, &regBuscado);
                if(end == 2){
                    printf("Falha no processamento do arquivo.");
                    free(temp);
                    free(chaveAtual);
                    fclose(arq_dados);
                    fclose(arq_arvore);
                    return;
                }else if(regBuscado.removido != '1'){
                    imprime_registro(regBuscado);
                }else{
                    printf("Registro inexistente.\n");
                }
                // libera as strings alocadas
                free(regBuscado.tecnologiaOrigem.nome);
                free(regBuscado.tecnologiaDestino.nome);
            }
            free(chaveAtual);
        }else{
            // busca direta em arquivo de dados
            if(primeiraBuscaDados == 0 || primeiraBuscaArvore == 0){
                // Leva cursor para inicio do arquivo de dados
                fseek(arq_dados, TAM_HEADER, SEEK_SET);
            }
            if(primeiraBuscaDados == 1){
                primeiraBuscaDados = 0;
            }

            int end = busca_em_arq_dados(arq_dados, nomeCampo, valorCampoBuscado);
            if(end == 2){
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