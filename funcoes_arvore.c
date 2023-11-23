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
    fseek(arquivo, (cabecalho->noRaiz + 1) * TAM_REG_ARVORE, SEEK_SET);
    int* altura = malloc(sizeof(int));
    dado* promove = insert(arquivo, cabecalho, data, altura);

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
 * @return 
 */
dado* insert(FILE* arquivo, header_arvore* cabecalho, dado *data, int* altura) {
    // le registro atual
    registro_arvore reg;
    ler_registro_arvore(arquivo, &reg);
    if(altura != NULL)
        *altura = reg.alturaNo;

    
    // encontra ponteiro adequado para a chave dada
    int num_chave = busca_binaria_reg_arvore(data->chave, reg);
    
    // garantir que estamo na posicao do maior elemento de chave maior que a atual
    if(num_chave < reg.nroChavesNo && strcmp(data->chave, reg.dados[num_chave].chave) > 0){
        num_chave += 1;
    }
    
    // analisa o ponteiro determinado, inserindo o elemento no registro atual caso seja -1, ou continuando a busca caso contrario
    dado* insere;
    if(reg.ponteiro_arvore[num_chave] == -1)
        return insert_here(arquivo, cabecalho, data, &reg, num_chave, -1);
    else {
        fseek(arquivo, (reg.ponteiro_arvore[num_chave] + 1) * TAM_REG_ARVORE, SEEK_SET);
        insere = insert(arquivo, cabecalho, data, NULL);
    }

    // caso a insercao tenha resultado em split e necessite da insercao de elemento na posicao atual
    if(insere != NULL) {
        int num_chave = 0;
        for(; num_chave < reg.nroChavesNo; num_chave++) {
            if(strcmp(insere->chave, reg.dados[num_chave].chave) < 0)
                break;
        }
        dado* retorno = insert_here(arquivo, cabecalho, insere, &reg, num_chave, cabecalho->RRNproxNo - 1);
        free(insere);
        return retorno;
    }
    return NULL;
}

dado* insert_here(FILE* arquivo, header_arvore* cabecalho, dado* data, registro_arvore* reg, int pos, int pont) {
    // insere dado no registro atual quando possui espaco
    if(reg->nroChavesNo < ORDEM-1) {
        for(int i = reg->nroChavesNo; i > pos; i--) {
            reg->dados[i] = reg->dados[i - 1];
            reg->ponteiro_arvore[i + 1] = reg->ponteiro_arvore[i];
        }
        reg->dados[pos] = *data;
        reg->ponteiro_arvore[pos + 1] = pont;

        reg->nroChavesNo++;

        // escreve registro modificado
        fseek(arquivo, (reg->RRNdoNo + 1) * TAM_REG_ARVORE, SEEK_SET);
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
        int meio = ORDEM/2;
        if(pos > meio) {
            // transfere todos os registros depois do meio para o novo registro criado
            for(int i = meio + 1; i < ORDEM-1; i++) {
                new_reg.dados[i - ORDEM/2] = reg->dados[i];
                reg->dados[i] = clear;
                new_reg.ponteiro_arvore[i - meio + 1] = reg->ponteiro_arvore[i + 1];
                reg->ponteiro_arvore[i + 1] = -1;
                
                reg->nroChavesNo--;
                new_reg.nroChavesNo++;
            }
            new_reg.ponteiro_arvore[0] = reg->ponteiro_arvore[meio + 1];
            reg->ponteiro_arvore[meio + 1] = -1;

            // insere dado na posicao escolhida
            int i = new_reg.nroChavesNo;
            for (; i > pos; i--) {
                new_reg.dados[i] = reg->dados[i - 1];
                new_reg.ponteiro_arvore[i + 1] = reg->ponteiro_arvore[i];
            }
            new_reg.dados[i] = *data;
            new_reg.ponteiro_arvore[i + 1] = pont;
            new_reg.nroChavesNo++;

            reg->nroChavesNo--;
        }
        else {
            // transfere todos os registros a partir do meio para o novo registro criado
            for(int i = meio; i < ORDEM-1; i++) {
                new_reg.dados[i - ORDEM/2] = reg->dados[i];
                reg->dados[i] = clear;
                new_reg.ponteiro_arvore[i - meio + 1] = reg->ponteiro_arvore[i + 1];
                reg->ponteiro_arvore[i + 1] = -1;
                
                reg->nroChavesNo--;
                new_reg.nroChavesNo++;
            }

            // insere dado na posicao escolhida
            for (int i = meio; i > pos; i--) {
                reg->dados[i] = reg->dados[i - 1];
                reg->ponteiro_arvore[i + 1] = reg->ponteiro_arvore[i];
            }
            reg->dados[pos] = *data;
            reg->ponteiro_arvore[pos + 1] = pont;
            
            // define ultimo ponteiro do segundo registro
            new_reg.ponteiro_arvore[0] = reg->ponteiro_arvore[meio + 1];
            reg->ponteiro_arvore[meio + 1] = -1;
        }
        // promove dado central
        *promove = reg->dados[meio];
        reg->dados[meio] = clear;

        // escreve registro modificado
        fseek(arquivo, (reg->RRNdoNo + 1) * TAM_REG_ARVORE, SEEK_SET);
        escrever_registro_arvore(arquivo, *reg);
        // escreve novo registro
        fseek(arquivo, (new_reg.RRNdoNo + 1) * TAM_REG_ARVORE, SEEK_SET);
        escrever_registro_arvore(arquivo, new_reg);

        //free(data);
        // retorna dado a ser promovido na split
        return promove;
    }
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
