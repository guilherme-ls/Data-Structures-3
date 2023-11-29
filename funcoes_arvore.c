// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#include "funcoes_arvore.h"

/**
 * @brief insere o dado desejado na arvore-B, comecando pela raiz
 * @param arquivo arquivo da arvore-B
 * @param cabecalho cabecalho da arvore-B
 * @param data dado a ser inserido na arvore
 */
void insere_raiz(FILE* arquivo, header_arvore* cabecalho, dado *data) {
    // inicia a insercao do dado na raiz
    fseek(arquivo, calcula_byte_off_arvore(cabecalho->noRaiz), SEEK_SET);
    int* altura = malloc(sizeof(int));
    dado* promove = insere_loop(arquivo, cabecalho, data, altura);

    // se nao necessitar de nova raiz, retorna
    if(promove == NULL) {
        free(altura);
        return;
    }

    // cria nova raiz resultante do split e atribui seu RRN
    registro_arvore new_reg;
    inicializa_registro_arvore(&new_reg);
    new_reg.RRNdoNo = cabecalho->RRNproxNo;
    new_reg.nroChavesNo = 1;
    cabecalho->RRNproxNo++;
    new_reg.alturaNo = *altura + 1;

    // insere dados e ponteiros na raiz
    new_reg.ponteiro_arvore[0] = cabecalho->noRaiz;
    new_reg.ponteiro_arvore[1] = cabecalho->RRNproxNo - 2;
    new_reg.dados[0] = *promove;

    // muda noRaiz no cabecalho
    cabecalho->noRaiz = new_reg.RRNdoNo;

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
dado* insere_loop(FILE* arquivo, header_arvore* cabecalho, dado *data, int* altura) {
    // le registro atual
    registro_arvore reg;
    ler_registro_arvore(arquivo, &reg);

    // preenche altura atual, caso solicitado (nao nulo)
    if(altura != NULL)
        *altura = reg.alturaNo;

    // encontra ponteiro adequado para a chave dada
    int num_chave = busca_binaria_reg_arvore(data->chave, reg);

    // analisa o ponteiro determinado, inserindo o elemento no registro atual caso seja -1, ou continuando a busca caso contrario
    dado* insere;
    if(reg.ponteiro_arvore[num_chave] == -1)
        return insere_reg(arquivo, cabecalho, data, &reg, num_chave, -1);
    else {
        // segue para o proximo ponteiro e chama recursivamente a busca
        fseek(arquivo, calcula_byte_off_arvore(reg.ponteiro_arvore[num_chave]), SEEK_SET);
        insere = insere_loop(arquivo, cabecalho, data, NULL);
    }

    // caso a insercao tenha resultado em split e necessite da insercao de elemento na posicao atual
    if(insere != NULL) {
        // calcula posicao ideal para insercao com a busca binaria e insere
        int num_chave = busca_binaria_reg_arvore(insere->chave, reg);
        dado* retorno = insere_reg(arquivo, cabecalho, insere, &reg, num_chave, cabecalho->RRNproxNo - 1);
        free(insere);
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
dado* insere_reg(FILE* arquivo, header_arvore* cabecalho, dado* data, registro_arvore* reg, int pos, int pont) {
    // insere dado no registro atual quando possui espaco
    if(reg->nroChavesNo < ORDEM-1) {
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

        // cria novo registro resultante do split e atribui seu RRN
        registro_arvore new_reg;
        inicializa_registro_arvore(&new_reg);
        new_reg.RRNdoNo = cabecalho->RRNproxNo;
        cabecalho->RRNproxNo++;
        new_reg.alturaNo = reg->alturaNo;

        // reorganiza os registros para o split
        dado *promove = malloc(sizeof(dado));
        const int meio = ORDEM/2;

        // insere chave no novo registro criado
        if(pos > meio) {
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

        // retorna dado a ser promovido na split
        return promove;
    }
}

/**
 * @brief funcao para divisao de chaves e ponteiros entre dois registros durante o split
 * @param reg registro original
 * @param new_reg novo registro resultante do split
 * @param comeco ponto do registro original a partir do qual serao transferidas as chaves
 */
void divide_chaves(registro_arvore* reg, registro_arvore* new_reg, int comeco) {
    dado clear;
    clear_dado(&clear);

    // transfere chaves e ponteiros entre registros no intervalo dado
    for(int i = comeco; i < ORDEM-1; i++) {
        new_reg->dados[i - ORDEM/2] = reg->dados[i];
        reg->dados[i] = clear;
        new_reg->ponteiro_arvore[i - comeco + 1] = reg->ponteiro_arvore[i + 1];
        reg->ponteiro_arvore[i + 1] = -1;
        
        reg->nroChavesNo--;
        new_reg->nroChavesNo++;
    }
}

/**
 * @brief insere a chave fornecida na posicao desejada dentro do registro
 * @param reg registro na qual sera realizada a insercao
 * @param chave chave a ser inserida
 * @param ponteiro ponteiro a direita da chave
 * @param pos posicao na qual a chave deve ser inserida
 */
void insere_chave_reg(registro_arvore* reg, dado chave, int ponteiro, int pos) {
    for (int i = reg->nroChavesNo; i > pos; i--) {
        reg->dados[i] = reg->dados[i - 1];
        reg->ponteiro_arvore[i + 1] = reg->ponteiro_arvore[i];
    }
    reg->dados[pos] = chave;
    reg->ponteiro_arvore[pos + 1] = ponteiro;
    reg->nroChavesNo++;
}


/**
 * @brief apaga o dado fornecido, colocando LIXO na chave e -1 no ponteiro
 * @param data dado a ser apagado
 */
void clear_dado(dado* data) {
    for(int i = 0; i < TAM_CHAVE; i++)
        data->chave[i] = LIXO_ARVORE;
    data->chave[TAM_CHAVE] = '\0';
    data->ponteiro_dado = -1;
}

/**
 * @brief le registro raiz da arvore e armazena na variavel fornecida
 * @param arq_arvore arquivo de indices
 * @param header header da arvore
 * @param reg struct de registro no qual armazenar a raiz
 * @return 1, quando encontrar fim do arquivo, ou 0, quando le com sucesso 
 */
int pega_raiz(FILE* arq_arvore, header_arvore header, registro_arvore* reg){
    fseek(arq_arvore, calcula_byte_off_arvore(header.noRaiz), SEEK_SET); // posiciona cursor para no raiz
    int check = ler_registro_arvore(arq_arvore, reg);
    return check;
}

/**
 * @brief le registro de dados proveniente de uma chave especificada no registro da arvore
 * @param arq_dados arquivo de dados a ser lido
 * @param reg_arvore registro da arvore contendo a chave a ser buscada
 * @param reg_dados registro de dados, preenchido pela funcao
 * @param iBuscaBinaria indice da chave buscada
 * @return 1, quando encontrar fim do arquivo, ou 0, quando le com sucesso 
 */
int ler_reg_dados_do_indice(FILE* arq_dados, registro_arvore reg_arvore, registro* reg_dados, int iBuscaBinaria){
    int RRNDado = reg_arvore.dados[iBuscaBinaria].ponteiro_dado; // RRN do registro buscado no arquivo de dados
    fseek(arq_dados, calcula_byte_off(RRNDado), SEEK_SET); // Cursor para o registro encontrado no arquivo de dados
    int end = ler_registro(arq_dados, reg_dados);
    return end;
}

/**
 * @brief realiza a busca binaria nas chaves de um registro do arquivo de indice
 * @param chaveBuscada string da chave buscada
 * @param reg registro onde buscar a chave
 * @return posicao onde a chave deve ser inserida (ou onde foi encontrada)
 */
int busca_binaria_reg_arvore(char* chaveBuscada, registro_arvore reg){
    // variaveis de apoio
    int menor = 0;
    int maior = reg.nroChavesNo - 1;
    int meio = 0;

    while(menor <= maior) {
        // calculo da posicao do meio
        meio = menor + (maior - menor) / 2;

        if(strcmp(chaveBuscada, reg.dados[meio].chave) == 0){
            // Encontrou chave
            return meio;
        }else if(strcmp(chaveBuscada, reg.dados[meio].chave) > 0){
            menor = meio + 1;
        }else{
            maior = meio - 1;
        }
    }

    // retorna posicao onde chave deveria ser inserida
    return menor;
}

/**
 * @brief funcao para remover os caracteres lixo de uma chave lida a partir da arvore
 * @param chave_original chave lida, ainda com lixo
 * @param chave_limpa nova chave, preenchida pela funcao
 */
void ler_chave_sem_lixo(char* chave_original, char* chave_limpa){
    int j = 0; 
    for(j = 0; j < TAM_CHAVE+1; j++){
        if(chave_original[j] == '$'){
            break;
        }
        chave_limpa[j] = chave_original[j];
    }
    chave_limpa[j] = '\0';
}