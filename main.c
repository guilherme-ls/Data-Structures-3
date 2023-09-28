// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registros.h"
#include "funcionalidades.h"

int main() {
    // detecta o comando numerico para selecionar a funcionalidade
    int comando;
    scanf("%d", &comando);

    // executa a funcionalidade selecionada
    switch(comando) {
        case 1:
            funcionalidade1();
            break;
        case 2:
            funcionalidade2();
            break;
        case 3:
            funcionalidade3();
            break;
        case 4:
            funcionalidade4();
            break;
        default:
            printf("comando nao encontrado.\n");
    }

    return 0;
}

