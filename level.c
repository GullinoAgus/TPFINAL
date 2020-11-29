//
// Created by gonzalo on 27/11/20.
//

#include "level.h"
#include "data.h"
#include <stdlib.h>
#include "allegro.h"

// FIXME: Checkear lo de rows asterisco
int cargarMapa(int ***mapa, int id, int* rows, int* columns){

    FILE *mapData;
    int error = 0;

    if(openLevelData(&mapData, id) == 1 ){
        return -1;
    }
    else {
        fscanf(mapData, "%d %d", rows, columns);
        *mapa = (int **) malloc(*rows * sizeof(int *)); //PUSE UN ASTERISCO ANTES DE ROWS PARA SACAR UN PROBLEMA
        for (int i = 0; i < *rows; i++){
            *(mapa[i]) = (int *) malloc(*columns * sizeof(int));
        }
    }

    fclose(mapData);
    return 0;
}

void drawLevel(estadoJuego_t *gameState){

    al_clear_to_color(al_map_rgb(0, 0, 0));

    for(int i = 0; i < gameState->entidades.enemigos; i++){

    }
}