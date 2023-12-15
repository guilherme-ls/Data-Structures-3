#include <stdlib.h>
#include <stdio.h>
#include "grafo.h"


int main() {
    // detecta o comando numerico para selecionar a funcionalidade
    int comando;
    scanf("%d", &comando);

    // executa a funcionalidade selecionada
    switch(comando) {
        /*
        case 8:
            funcionalidade8();
            break;
        case 9:
            funcionalidade9();
            break;
        case 10:
            funcionalidade10();
            break;
        case 11:
            funcionalidade11();
            break;
        case 12:
            funcionalidade12();
            break;
        */
        default:
            printf("comando nao encontrado.\n");
    }

    return 0;
}
