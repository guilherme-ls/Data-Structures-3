// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#include "heap.h"

// organiza heaps
void heapify(int *vertice, int size, int pos, int* distancias) {
    // define a posição do menor elemento como a atual
    int smaller = pos;
    // define as posições dos elementos à esquerda e à direita do atual
    int left = 2 * pos + 1;
    int right = 2 * pos + 2;

    // se o filho esquerdo pertence à heap e é menor que o pai, define ele como menor
    if (left < size && distancias[vertice[left]] < distancias[vertice[smaller]]) {
        smaller = left;
    }
    
    // se o filho direito pertence à heap e é menor que o pai, define ele como menor
    if (right < size && distancias[vertice[right]] < distancias[vertice[smaller]]) {
        smaller = right;
    }

    // se o menor elemento não é o pai, reorganiza a heap
    if (smaller != pos) {
        // troca o pai e o menor filho de posição
        int aux = vertice[pos];
        vertice[pos] = vertice[smaller];
        vertice[smaller] = aux;

        // reorganiza a heap a partir do filho trocado
        heapify(vertice, size, smaller, distancias);
    }
}

void organiza_heap(int *vertice, int size, int* distancias) {
    // organiza a heap inicialmente
    // inicia em size/2 - 1 e vai até 0 para contemplar apenas nós pais, reorganizando a heap em cada um deles
    for(int i = (size / 2) - 1; i >= 0; i--) {
        heapify(vertice, size, i, distancias);
    }
}

int remove_top(int *vertice, int *size, int* distancias) {
    if(size == 0)
        return -1;

    int aux = vertice[0];
    vertice[0] = vertice[*size - 1];

    return aux;
}