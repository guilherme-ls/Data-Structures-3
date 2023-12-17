// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#ifndef HEAP
#define HEAP

void heapify(int *vertice, int size, int pos, int* distancias);
void organiza_heap(int *vertice, int size, int* distancias);
int remove_top(int *vertice, int *size, int* distancias);

#endif