// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#include "subfuncoes_internas_arvore.h"

/**
 * @brief funcao para divisao de chaves e ponteiros entre dois registros durante o split
 * @param reg registro original
 * @param new_reg novo registro resultante do split
 * @param comeco ponto do registro original a partir do qual serao transferidas as chaves
 */
void divide_chaves(registro_arvore *reg, registro_arvore *new_reg, int comeco) {
    dado clear;
    clear_dado(&clear);

    // transfere chaves e ponteiros entre registros no intervalo dado
    for (int i = comeco; i < ORDEM - 1; i++) {
        new_reg->dados[i - ORDEM / 2] = reg->dados[i];
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
void insere_chave_reg(registro_arvore *reg, dado chave, int ponteiro, int pos) {
    for (int i = reg->nroChavesNo; i > pos; i--)
    {
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
void clear_dado(dado *data) {
    for (int i = 0; i < TAM_CHAVE; i++)
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
int pega_raiz(FILE *arq_arvore, header_arvore header, registro_arvore *reg) {
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
int ler_reg_dados_do_indice(FILE *arq_dados, registro_arvore reg_arvore, registro *reg_dados, int iBuscaBinaria) {
    int RRNDado = reg_arvore.dados[iBuscaBinaria].ponteiro_dado; // RRN do registro buscado no arquivo de dados
    fseek(arq_dados, calcula_byte_off(RRNDado), SEEK_SET);       // Cursor para o registro encontrado no arquivo de dados
    int end = ler_registro(arq_dados, reg_dados);
    return end;
}

/**
 * @brief realiza a busca binaria nas chaves de um registro do arquivo de indice
 * @param chaveBuscada string da chave buscada
 * @param reg registro onde buscar a chave
 * @return posicao onde a chave deve ser inserida (ou onde foi encontrada)
 */
int busca_binaria_reg_arvore(char *chaveBuscada, registro_arvore reg) {
    // variaveis de apoio
    int menor = 0;
    int maior = reg.nroChavesNo - 1;
    int meio = 0;

    while (menor <= maior) {
        // calculo da posicao do meio
        meio = menor + (maior - menor) / 2;

        if (strcmp(chaveBuscada, reg.dados[meio].chave) == 0) {
            // Encontrou chave
            return meio;
        }
        else if (strcmp(chaveBuscada, reg.dados[meio].chave) > 0) {
            menor = meio + 1;
        }
        else {
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
void ler_chave_sem_lixo(char *chave_original, char *chave_limpa) {
    int j = 0;
    for (j = 0; j < TAM_CHAVE + 1; j++) {
        if(chave_original[j] == '$')
            break;
        chave_limpa[j] = chave_original[j];
    }
    chave_limpa[j] = '\0';
}
