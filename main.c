

#include <unistd.h>
#include <stdio.h>
#include "mapa.h"

int main() {
    int* prueba;
    prueba = leerNivel("texto.txt");
    int contlineas = 0;
    for (int i = 0; contlineas < 16*2 ; ++i) {
        printf(" %c ", prueba[i]);
        if (prueba[i] == BORDE){
            contlineas++;
            if (contlineas%2 == 0){
                printf("\n");
            }
        }
    }
    return 0;
}

