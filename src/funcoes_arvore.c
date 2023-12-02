// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#include "funcoes_arvore.h"

/**
 * @brief insere o dado desejado na arvore-B, comecando pela raiz
 * @param arquivo arquivo da arvore-B
 * @param cabecalho cabecalho da arvore-B
 * @param data dado a ser inserido na arvore
 */
void insere_raiz(FILE *arquivo, header_arvore *cabecalho, dado *data, registro_arvore* raiz) {
    // inicia a insercao do dado na raiz
    int *altura = malloc(sizeof(int));
    dado *promove = insere_loop(arquivo, cabecalho, data, altura, raiz);

    // se nao necessitar de nova raiz, retorna
    if (promove == NULL) {
        free(altura);
        return;
    }

    // cria nova raiz resultante do split e atribui seu RRN
    registro_arvore new_reg;
    inicializa_registro_preparado_arvore(&new_reg, cabecalho, *altura + 1);

    // insere dados e ponteiros na raiz
    insere_chave_reg(&new_reg, *promove, cabecalho->RRNproxNo - 2, 0);
    new_reg.ponteiro_arvore[0] = cabecalho->noRaiz;

    // muda noRaiz no cabecalho
    cabecalho->noRaiz = new_reg.RRNdoNo;

    // troca raiz externa
    *raiz = new_reg;

    // escreve registro modificado
    fseek(arquivo, (new_reg.RRNdoNo + 1) * TAM_REG_ARVORE, SEEK_SET);
    escrever_registro_arvore(arquivo, new_reg);

    free(promove);
    free(altura);
}

/**
 * @brief procura a chave fornecida na arvore (o byte offset atual deve se encontrar na raiz)
 * @param arquivo arquivo da arvore-B a ser lida
 * @param chave chave a ser buscada
 * @return retorna dado promovido
 */
dado *insere_loop(FILE *arquivo, header_arvore *cabecalho, dado *data, int *altura, registro_arvore* registro_carregado) {
    // le registro atual caso necessario, ou carrega o da raiz
    registro_arvore reg;
    if(registro_carregado == NULL)
        ler_registro_arvore(arquivo, &reg);
    else
        reg = *registro_carregado;

    // preenche altura atual, caso solicitado (nao nulo)
    if (altura != NULL)
        *altura = reg.alturaNo;

    // encontra ponteiro adequado para a chave dada
    int num_chave = busca_binaria_reg_arvore(data->chave, reg);

    // analisa o ponteiro determinado, inserindo o elemento no registro atual caso seja -1, ou continuando a busca caso contrario
    dado *insere;
    if (reg.ponteiro_arvore[num_chave] == -1) {
        dado *retorno = insere_reg(arquivo, cabecalho, data, &reg, num_chave, -1);
        // atualiza registro precarregado fornecido
        if(registro_carregado != NULL)
            *registro_carregado = reg;

        return retorno;
    }
    else {
        // segue para o proximo ponteiro e chama recursivamente a busca
        fseek(arquivo, calcula_byte_off_arvore(reg.ponteiro_arvore[num_chave]), SEEK_SET);
        insere = insere_loop(arquivo, cabecalho, data, NULL, NULL);
    }

    // caso a insercao tenha resultado em split e necessite da insercao de elemento na posicao atual
    if (insere != NULL) {
        // calcula posicao ideal para insercao com a busca binaria e insere
        int num_chave = busca_binaria_reg_arvore(insere->chave, reg);
        dado *retorno = insere_reg(arquivo, cabecalho, insere, &reg, num_chave, cabecalho->RRNproxNo - 1);
        free(insere);
        // atualiza registro precarregado fornecido
        if(registro_carregado != NULL)
            *registro_carregado = reg;

        return retorno;
    }
    return NULL;
}

/**
 * @brief Funcao para insercao de um dado em um registro fornecido
 * @param arquivo arquivo de indices
 * @param cabecalho cabecalho da arvore
 * @param data dado a ser inserido
 * @param reg registro no qual o dado deve ser inserido
 * @param pos posicao do registro na qual o dado deve ser inserido
 * @param pont ponteiro a direita do dado inserido (relevante para promocoes)
 * @return retorna dado a ser promovido
 */
dado *insere_reg(FILE *arquivo, header_arvore *cabecalho, dado *data, registro_arvore *reg, int pos, int pont) {
    // insere dado no registro atual quando possui espaco
    if (reg->nroChavesNo < ORDEM - 1) {
        insere_chave_reg(reg, *data, pont, pos);

        // escreve registro modificado (necessario fseek pois ponteiro atual encontra-se no registro seguinte apos leitura)
        fseek(arquivo, calcula_byte_off_arvore(reg->RRNdoNo), SEEK_SET);
        escrever_registro_arvore(arquivo, *reg);

        return NULL;
    }
    // caso nao haja espaco, faz split
    else {
        // cria dado vazio
        dado clear;
        clear_dado(&clear);

        // cria novo registro resultante do split e atribui seu RRN e altura
        registro_arvore new_reg;
        inicializa_registro_preparado_arvore(&new_reg, cabecalho, reg->alturaNo);

        // reorganiza os registros para o split
        dado *promove = malloc(sizeof(dado));
        const int meio = ORDEM / 2;

        // insere chave no novo registro criado
        if (pos > meio) {
            // transfere todos os registros depois do meio para o novo registro criado
            divide_chaves(reg, &new_reg, meio + 1);

            // insere dado na posicao escolhida
            insere_chave_reg(&new_reg, *data, pont, pos - reg->nroChavesNo);
        }
        // insere chave no registro antigo
        else {
            // transfere todos os registros a partir do meio para o novo registro criado
            divide_chaves(reg, &new_reg, meio);

            // insere dado na posicao escolhida
            insere_chave_reg(reg, *data, pont, pos);
        }
        // define ultimo ponteiro do novo registro
        new_reg.ponteiro_arvore[0] = reg->ponteiro_arvore[meio + 1];
        reg->ponteiro_arvore[meio + 1] = -1;

        // promove dado central
        *promove = reg->dados[meio];
        reg->dados[meio] = clear;
        reg->nroChavesNo--;

        // escreve registro modificado (necessario fseek pois ponteiro atual encontra-se no registro seguinte apos leitura)
        fseek(arquivo, calcula_byte_off_arvore(reg->RRNdoNo), SEEK_SET);
        escrever_registro_arvore(arquivo, *reg);
        // escreve novo registro
        fseek(arquivo, calcula_byte_off_arvore(new_reg.RRNdoNo), SEEK_SET);
        escrever_registro_arvore(arquivo, new_reg);

        // retorna dado a ser promovido no split
        return promove;
    }
}

/**
 * @brief realiza a busca de uma chave fornecida em um arquivo de indices e imprime o registro encontrado
 * @param arq_arvore arquivo de indices onde realizar a busca
 * @param arq_dados arquivo de dados onde buscar o registro determinado no arquivo de indices
 * @param valorCampoBuscado chave buscada no aquivo de indices
 * @param cabecalho_arvore cabecalho do arvore B no arquivo de indices
 * @param primeiraBuscaArvore variavel para determinar a necessidade de buscar a raiz da arvore
 * @return 1 em caso de erros, 0 caso executado com sucesso
 */
int busca_em_indice(FILE* arq_arvore,FILE *arq_dados, char *valorCampoBuscado, header_arvore cabecalho_arvore, int *primeiraBuscaArvore, registro_arvore* reg_raiz_arvore) {
    registro_arvore reg_no_atual; // variavel para armazenar no atual da busca na arvore

    if (cabecalho_arvore.noRaiz == -1) {
        // Arquivo de arvore vazio.
        printf("Registro inexistente.\n");
        return 0;
    }

    // trazer raiz da arvore para memória primária caso seja o primeiro acesso
    if (*primeiraBuscaArvore) {
        pega_raiz(arq_arvore, cabecalho_arvore, reg_raiz_arvore);
        *primeiraBuscaArvore = 0;
    }

    reg_no_atual = *reg_raiz_arvore; // começa busca pela raiz

    // Busca na arvore B
    int iBuscaAtual = busca_em_arvore(arq_arvore, valorCampoBuscado, &reg_no_atual);

    if (iBuscaAtual != -1) {
        // O registro foi encontrado
        registro regBuscado;
        int end = ler_reg_dados_do_indice(arq_dados, reg_no_atual, &regBuscado, iBuscaAtual); // ler registro a partir da referencia no arquivo de indice
        if (end == 1) {
            // o registro nao existe no arquivo de dados
            printf("Registro inexistente.\n");
            return 1;
        }
        else if (regBuscado.removido != '1') {
            // Por garantia, checar se registro não está removido.
            imprime_registro(regBuscado);
        }
        else
            printf("Registro inexistente.\n");

        // libera as strings alocadas
        free(regBuscado.tecnologiaOrigem.nome);
        free(regBuscado.tecnologiaDestino.nome);
    }
    return 0;
}

/**
 * @brief realiza a busca de uma dada chave na arvora
 * @param arq_arvore arquivo de indices onde buscar a chave
 * @param valorCampoBuscado chave buscada na arvore
 * @param reg_no_atual registro atual da arvore
 * @return -1 em caso de erros, RRN quando concluido com sucesso
 */
int busca_em_arvore(FILE *arq_arvore, char *valorCampoBuscado, registro_arvore *reg_no_atual) {
    char* chaveAtual = malloc((TAM_CHAVE + 1) * sizeof(char)); // Valor para chave do indice sem o lixo
    int iBuscaAtual = busca_binaria_reg_arvore(valorCampoBuscado, *reg_no_atual); // Variavel para indice retornado na busca binaria
    if(iBuscaAtual >= reg_no_atual->nroChavesNo)
        strcpy(chaveAtual, ""); // caso a posicao para insercao seja a 3, ultimo ponteiro do registro, sem chaves
    else
        ler_chave_sem_lixo(reg_no_atual->dados[iBuscaAtual].chave, chaveAtual); // Lê a chave retornada na busca.
    
    if(strcmp(valorCampoBuscado, chaveAtual) != 0) {
        free(chaveAtual);

        if (reg_no_atual->ponteiro_arvore[iBuscaAtual] == -1) {
            // Não existe subárvore, fim da busca.
            printf("Registro inexistente.\n");
            return -1;
        }
        fseek(arq_arvore, calcula_byte_off_arvore(reg_no_atual->ponteiro_arvore[iBuscaAtual]), SEEK_SET); // Posiciona cursor no proximo registro da arvore.
        int check = ler_registro_arvore(arq_arvore, reg_no_atual); // leitura do próximo registro de indice para busca.
        // checa existencia do registro
        if (check == 1) {
            printf("Registro inexistente.\n");
            return -1;
        }
        // reinicia a busca recursivamente no proximo registro
        return busca_em_arvore(arq_arvore, valorCampoBuscado, reg_no_atual);
    }
    free(chaveAtual);
    return iBuscaAtual;
}