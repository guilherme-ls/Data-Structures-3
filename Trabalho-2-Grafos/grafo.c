// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#include <stdlib.h>
#include <string.h>
#include "grafo.h"
#include "heap.h"

/**
 * @brief funcao para a inicializacao do grafo com valores padrao
 * @param g ponteiro do grafo a ser inicializado
 * @param num_tecnologias numero de tecnologias contidas no grafo
 */
void inicializa_grafo(grafo *g, int num_tecnologias) {
    g->num_vertices = 0;
    g->num_arestas = 0;
    g->lista_vertices = (vertice_grafo**) malloc((num_tecnologias + 1) * sizeof(vertice_grafo*));
}

/**
 * @brief funcao para criar um vertice do grafo a partir dos dados fornecidos
 * @param vertice ponteiro para o vertice a ser criado
 * @param tecnologia string com a tecnologia origem do vertice
 * @param grupo grupo do vertice
 */
void cria_vertice(vertice_grafo *vertice, string tecnologia, int grupo) {
    vertice->nomeTecOrigem = (char*) malloc((tecnologia.tamanho + 1) * sizeof(char));
    strcpy(vertice->nomeTecOrigem, tecnologia.nome);
    vertice->grupo = grupo;
    vertice->grau_entrada = 0;
    vertice->grau_saida = 0;
    vertice->grau = 0;
    inicializa_lista(&(vertice->lista_arestas));
}

/**
 * @brief funcao para criar uma aresta do grafo a partir do vertice de origem e do registro de dados
 * @param aresta ponteiro para a aresta a ser criada
 * @param vertice_destino vertice de destino a ser indicado pela aresta
 * @param peso peso da aresta
 */
void cria_aresta(aresta_grafo *aresta, vertice_grafo *vertice_destino, int peso) {
    aresta->tecDestino = vertice_destino;
    aresta->peso = peso;
}

/**
 * @brief insere uma aresta em um dado vertice
 * @param vertice vertice onde inserir a aresta
 * @param aresta aresta a ser inserida
 */
void insere_aresta_vertice(vertice_grafo *vertice, aresta_grafo *aresta) {
    inserir(&(vertice->lista_arestas), aresta);
    vertice->grau_saida++;
    vertice->grau++;
    aresta->tecDestino->grau++;
    aresta->tecDestino->grau_entrada++;
}

/**
 * @brief realiza busca binaria na lista de adjacencias para encontrar o vertice com uma dada tecnologia origem
 * @param g grafo onde realizar a busca
 * @param nomeTec tecnologia origem do vertice buscado
 * @return posicao do vertice buscado na lista de adjacencias
 */
int busca_binaria_vertices(grafo g, char* nomeTec) {
    // variaveis de apoio
    int menor = 0;
    int maior = g.num_vertices - 1;
    int meio = 0;

    while (menor <= maior) {
        // calculo da posicao do meio
        meio = menor + (maior - menor) / 2;

        if (strcmp(nomeTec, g.lista_vertices[meio]->nomeTecOrigem) == 0) {
            // Encontrou chave
            return meio;
        }
        else if (strcmp(nomeTec, g.lista_vertices[meio]->nomeTecOrigem) > 0) {
            menor = meio + 1;
        }
        else {
            maior = meio - 1;
        }
    }

    // retorna posicao onde chave foi encontrada (ou deveria ser inserida)
    return menor;
}

/**
 * @brief insere um dado vertice na lista de adjacencias do grafo
 * @param g grafo onde realizar a insercao
 * @param vertice vertice a ser inserido
 * @param pos posicao onde inserir o vertice
 */
void insere_vertice(grafo *g, vertice_grafo *vertice, int pos) {
    for(int i = g->num_vertices; i > pos; i--) {
        g->lista_vertices[i] = g->lista_vertices[i-1];
    }
    g->lista_vertices[pos] = vertice;
    g->num_vertices++;
}

/**
 * @brief insere uma aresta no grafo, definida pelos parametros da funcao
 * @param g grafo onde realizar a insercao
 * @param tecnologiaOrigem string da tecnologia origem
 * @param grupo grupo ao qual as tecnologias pertencem
 * @param tecnologiaDestino string da tecnologia destino
 * @param peso peso da aresta
 */
void insere_aresta(grafo *g, string tecnologiaOrigem, int grupo1, string tecnologiaDestino, int grupo2, int peso) {
    int pos;
    vertice_grafo *verticeOrigem = malloc(sizeof(vertice_grafo));
    vertice_grafo *verticeDestino = malloc(sizeof(vertice_grafo));
    aresta_grafo *aresta = malloc(sizeof(aresta_grafo));


    // Primeira inserção, insere vertice e aresta direto
    if(g->num_vertices == 0) {
        // Cria o vertice a ser inserido no grafo
        cria_vertice(verticeOrigem, tecnologiaOrigem, grupo1);
        cria_vertice(verticeDestino, tecnologiaDestino, grupo2);
        insere_vertice(g, verticeOrigem, 0);
        pos = busca_binaria_vertices(*g, tecnologiaDestino.nome);
        insere_vertice(g, verticeDestino, pos);

        // Cria aresta a ser inserida no grafo
        cria_aresta(aresta, verticeDestino, peso);

        // Adiciona aresta na lista de arestas do vertice
        insere_aresta_vertice(verticeOrigem, aresta);
        g->num_arestas++;
    }
    else {
        // checa se o vertice origem ja existe na lista de adjacencias, criando e inserindo caso necessario
        pos = busca_binaria_vertices(*g, tecnologiaOrigem.nome);
        if(pos < g->num_vertices && strcmp(g->lista_vertices[pos]->nomeTecOrigem, tecnologiaOrigem.nome) == 0) {
            free(verticeOrigem);
            verticeOrigem = g->lista_vertices[pos];
            if(verticeOrigem->grupo == 0) {
                verticeOrigem->grupo = grupo1;
            }
        }
        else {
            cria_vertice(verticeOrigem, tecnologiaOrigem, grupo1);
            insere_vertice(g, verticeOrigem, pos);
        }

        // checa se o vertice destino ja existe na lista de adjacencias, criando e inserindo caso necessario
        pos = busca_binaria_vertices(*g, tecnologiaDestino.nome);
        if(pos < g->num_vertices && strcmp(g->lista_vertices[pos]->nomeTecOrigem, tecnologiaDestino.nome) == 0) {
            free(verticeDestino);
            verticeDestino = g->lista_vertices[pos];
            if(verticeDestino->grupo == 0) {
                verticeDestino->grupo = grupo2;
            }
        }
        else {
            cria_vertice(verticeDestino, tecnologiaDestino, grupo2);
            insere_vertice(g, verticeDestino, pos);
        }

        // cria aresta a ser inserida
        cria_aresta(aresta, verticeDestino, peso);
        
        // insere aresta criada
        insere_aresta_vertice(verticeOrigem, aresta);
        g->num_arestas++;
    }
}

/**
 * @brief busca a tecnologia origem fornecida e imprime todas as tecnologias destino para a qual possui arestas de saida
 * @param g grafo onde buscar
 * @param tecnologia tecnologia origem buscada
 */
void busca_tecnologias_entrada(grafo g, char* tecnologia) {
    // realiza busca binaria na lista de adjacencias para encontrar o vertice buscado
    int pos = busca_binaria_vertices(g, tecnologia);

    // verifica se o vertice foi encontrado
    vertice_grafo vertice = *(g.lista_vertices[pos]);
    if(strcmp(tecnologia, vertice.nomeTecOrigem) != 0) {
        printf("Registro inexistente.\n");
        return;
    }

    // se encontrado, verifica o tamanho de sua lista de arestas
    if(vertice.grau_saida == 0) {
        printf("Registro inexistente.\n");
        return;
    }

    // caso existam elementos, printa a saida desejada
    printf("%s: ", vertice.nomeTecOrigem);
    no *aresta = vertice.lista_arestas.ini;
    for(int i = 0; i < vertice.lista_arestas.tam - 1; i++) {
        printf("%s, ", aresta->info->tecDestino->nomeTecOrigem);
        aresta = aresta->prox;
    }
    printf("%s\n\n", aresta->info->tecDestino->nomeTecOrigem);
}

int verificaNaoDescoberto(grafo *g, vertice_grafo *vertice, int cor[]){
    int pos = busca_binaria_vertices(*g, vertice->nomeTecOrigem);

    if(cor[pos] == 0){
        return 1;
    }
    return 0;
}  


void procedimentoBuscaDFS(grafo *g, vertice_grafo *verticeAtual, int *pilha, int *indicePilha, int *cor, int *tempoDescoberta, int *tempoTermino, int *tempo){
    int posAtual = busca_binaria_vertices(*g, verticeAtual->nomeTecOrigem);
    // Pinta vertice de cinza e marca tempo de descoberta
    cor[posAtual] = 1;
    tempoDescoberta[posAtual] = *tempo;
    *tempo++;

    // Empilha vertices adjacentes nao descobertos
    no *noAdjacente = verticeAtual->lista_arestas.ini;
    aresta_grafo *aresta_adjacente;
    while(noAdjacente != NULL){
        aresta_adjacente = noAdjacente->info;
        if(verificaNaoDescoberto(g, aresta_adjacente->tecDestino, cor)){
            procedimentoBuscaDFS(g, aresta_adjacente->tecDestino, pilha, indicePilha, cor, tempoDescoberta, tempoTermino, tempo);
        }
        noAdjacente = noAdjacente->prox;
    }

    // Adiciona a pilha de ordem
    pilha[(*indicePilha)++] = posAtual;

    // Sem vertices adjacentes pinta de preto e marca tempo de termino
    cor[posAtual] = 2;
    tempoTermino[posAtual] = *tempo;
    *tempo++;
}

void procedimentoBuscaDFSTransposto(grafo *g, vertice_grafo *verticeAtual, int *cor, int *tempoDescoberta, int *tempoTermino, int *tempo){
    int posAtual = busca_binaria_vertices(*g, verticeAtual->nomeTecOrigem);
    // Pinta vertice de cinza e marca tempo de descoberta
    cor[posAtual] = 1;
    tempoDescoberta[posAtual] = *tempo;
    *tempo++;

    // Empilha vertices adjacentes nao descobertos
    no *noAdjacente = verticeAtual->lista_arestas.ini;
    aresta_grafo *aresta_adjacente;
    while(noAdjacente != NULL){
        aresta_adjacente = noAdjacente->info;
        if(verificaNaoDescoberto(g, aresta_adjacente->tecDestino, cor)){
            procedimentoBuscaDFSTransposto(g, aresta_adjacente->tecDestino, cor, tempoDescoberta, tempoTermino, tempo);
        }
        noAdjacente = noAdjacente->prox;
    }

    // Sem vertices adjacentes pinta de preto e marca tempo de termino
    cor[posAtual] = 2;
    tempoTermino[posAtual] = *tempo;
    *tempo++;
}


void ContaCFC(grafo *g, grafo *g_transposto){
    int tempoDescoberta[g->num_vertices]; // Vetor para armazenar tempo de descoberta dos vertices
    int tempoTermino[g->num_vertices]; // Vetor para armazenar tempo de termino dos vertices
    int cor[g->num_vertices]; // 0 (branco) - Nao descoberto; 1 (cinza) - Descoberto; 2 (preto) - Finalizado
    int *pilha = (int*) malloc(g->num_vertices * sizeof(int));  // Pilha para armazenar ordem dos vertices
    int indicePilha = 0;

    // Inicializa temporizador
    int tempo = 1;

    // Inicializa cor branca (= 0) em todos
    for(int i = 0; i < g->num_vertices; i++){
        cor[i] = 0;
    } 

    // Declara variavel para vertice atual
    vertice_grafo *verticeAtual;

    // Loop para busca em profundidadae no grafo normal
    for(int i = 0; i < g->num_vertices; i++){
        // Caso ainda haja vertices nao descobertos
        if(cor[i] == 0){
            verticeAtual = g->lista_vertices[i];
            procedimentoBuscaDFS(g, verticeAtual, pilha, &indicePilha, cor, tempoDescoberta, tempoTermino, &tempo);
        }
    }

    // Reinicializa cor branca (= 0) em todos
    for(int i = 0; i < g->num_vertices; i++){
        cor[i] = 0;
    } 
    
    int cont = 0;
    // Loop para busca em profundidadae no grafo transposto
    for(int i = indicePilha-1; i >= 0; i--){
        // Caso ainda haja vertices nao descobertos
        int idAtual = pilha[i];
        if(cor[idAtual] == 0){
            printf("Percorrendo CFC %d...\n", ++cont);
            verticeAtual = g->lista_vertices[idAtual];
            procedimentoBuscaDFSTransposto(g, verticeAtual, cor, tempoDescoberta, tempoTermino, &tempo);
        }
    }

    // Libera memoria da pilha
    free(pilha);

}

/**
 * @brief verifica se determinado vertice ja foi finalizado no algoritmo de Dijkstra
 * @param finalizados vetor com vertices ja finalizados
 * @param vertice vertice para verificacao
 * @param cont quantidade de vertices finalizados
 * @return 1 caso ja esteja finaliza, 0 caso contrario.
 */
int verifica_vertice_finalizado(vertice_grafo *finalizados, vertice_grafo vertice, int cont){
    // Loop para verificar se vertice ja foi finalizado
    for(int i = 0; i < cont; i++){
        if(strcmp(finalizados[cont].nomeTecOrigem, vertice.nomeTecOrigem) == 0){
            return 1;
        }
    }
    return 0;
}

int finalizaMenor(grafo g, vertice_grafo *finalizados, int distancias[], int cont){
    int menorDist = INFINITO;  // Armazena menor distancia atual
    int menorId = 0;  // Armazena posicao do vertice de menor distancia atual
    vertice_grafo vertice_atual;  // Armazena vertice atual para comparaçao

    for(int i = 0; i < g.num_vertices; i++){
        vertice_atual = *(g.lista_vertices[i]);
        if(!verifica_vertice_finalizado(finalizados, vertice_atual, cont)){ 
            // Se o vertice nao foi finalizado ainda, faz comparacao
            if(distancias[i] < menorDist){
                menorDist = distancias[i];
                menorId = i;
            }
        }
    }

    // Adiciona vertice aos finalizados
    if(menorDist < INFINITO){
        finalizados[cont] = *(g.lista_vertices[menorId]);
        return menorId;
    }

    return -1;
}

/**
 * @brief Busca o menor caminho entre uma tecnologia origem a uma tecnologia destino
 * @param g grafo onde busca e realizada
 * @param tecnologiaInicial nome da tecnologia inicial
 * @param tecnologiaFinal nome da tecnologia final
 * @return distancia para sair do vertice inicial e chegar no final
*/
int dijkstra(grafo g, char* tecnologiaInicial, char* tecnologiaFinal){
    // Realiza busca binaria na lista de adjacencia para encontrar o vertice de origem
    int posInicial = busca_binaria_vertices(g, tecnologiaInicial);  // Posicao vertice origem.
    int posFinal = busca_binaria_vertices(g, tecnologiaFinal);  // Posicao vertice destino.
    int size = g.num_vertices;
    int distancias[g.num_vertices];  // Vetor para armazenar distancias minímas a partir do vertice inicial
    int heap[g.num_vertices];
    
    // Verifica se existem os vertices inicial e final em questao
    vertice_grafo verticeInicial = *(g.lista_vertices[posInicial]);
    if(strcmp(tecnologiaInicial, verticeInicial.nomeTecOrigem) != 0) {
        return INFINITO;
    }
    vertice_grafo verticeFinal = *(g.lista_vertices[posFinal]);
    if(strcmp(tecnologiaFinal, verticeFinal.nomeTecOrigem) != 0) {
        return INFINITO;
    }

    // Inicializacao das distancias
    for(int i = 0; i < g.num_vertices; i++) {
        heap[i] = i;
        distancias[i] = INFINITO;
    }

    // Começa pelo vertice origem
    vertice_grafo vertice_atual = verticeInicial;
    distancias[posInicial] = 0;
    int posAtual = posInicial;
    organiza_heap(heap, size, distancias);

    // Loop para encontrar menor caminho entre os vertices
    while(1) {
        // define lista de arestas a ser analisada
        no *no_atual_lista = vertice_atual.lista_arestas.ini;
        aresta_grafo aresta_adjacente;
        for(int i = 0; i < vertice_atual.lista_arestas.tam; i++) {
            aresta_adjacente = *(no_atual_lista->info);
            
            // Para todo vertice (v) adjacente ao atual (w) D[v] = min(D[v], D[w] + peso aresta(w, v)) 
            int posAdjacente = busca_binaria_vertices(g, aresta_adjacente.tecDestino->nomeTecOrigem);
            if(distancias[posAtual] + aresta_adjacente.peso < distancias[posAdjacente]) {
                distancias[posAdjacente] = distancias[posAtual] + aresta_adjacente.peso;
            }

            // Vai para proxima aresta
            no_atual_lista = no_atual_lista->prox;
        }
        if(size == 0) {
            // Nao ha mais vertices para analisar
            break;
        }

        // reorganiza a heap com base nas novas posicoes e seleciona seu topo para ser o novo vertice a ser analisado
        organiza_heap(heap, size, distancias);
        posAtual = remove_top(heap, &size, distancias);
        size--;
        vertice_atual = *(g.lista_vertices[posAtual]);
    }

    // Retorna tamanho do menor caminho para vertice final.
    return distancias[posFinal];
}

/**
 * @brief libera memoria alocada pelo grafo
 * @param g grafo a ser liberado
 */
void libera_grafo(grafo g) {
    for(int i = 0; i < g.num_vertices; i++) {
        vertice_grafo *vertice = g.lista_vertices[i];
        destruir(&(vertice->lista_arestas));
        free(vertice->nomeTecOrigem);
        free(vertice);
    }

    free(g.lista_vertices);
}