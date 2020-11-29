//
// Created by gonzalo on 27/11/20.
//

#include "level.h"
#include "data.h"
#include <stdlib.h>
#include "allegro.h"

#define TOWORLDPOS(v) (v*PIXELSPERUNIT)

// FIXME: Checkear lo de rows asterisco. no se usa error
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


// FIXME: Los arreglos de enemigos y bloques tienen q terminar con NULL
void initEntities(estadoJuego_t* gameState){

    int blocksIndex = 0;
    int enemiesIndex = 0;

    for(int i = 0; i < gameState->levelWidht; i++){
        for(int j = 0; j < gameState->levelHeight; j++){
            switch ((gameState->level)[i][j]) {

                case LADRILLO:
                    gameState->entidades.bloques[blocksIndex]->sprite = 0;
                    gameState->entidades.bloques[blocksIndex]->identificador = LADRILLO;
                    gameState->entidades.bloques[blocksIndex]->fisica.posx = TOWORLDPOS(i);
                    gameState->entidades.bloques[blocksIndex]->fisica.posy = TOWORLDPOS(j);
                    gameState->entidades.bloques[blocksIndex]->fisica.ancho = PIXELSPERUNIT;
                    gameState->entidades.bloques[blocksIndex]->fisica.alto = PIXELSPERUNIT;
                    gameState->entidades.bloques[blocksIndex]->fisica.velx = 0;
                    gameState->entidades.bloques[blocksIndex]->fisica.vely = 0;
                    blocksIndex++;
                    break;

                case JUGADOR:
                    gameState->entidades.jugador.vidas = 3;
                    gameState->entidades.jugador.sobreBloque = 0;
                    gameState->entidades.jugador.estado = 1;        //VIVO?
                    gameState->entidades.jugador.sprite = 0;
                    gameState->entidades.jugador.fisica.posx = TOWORLDPOS(i);
                    gameState->entidades.jugador.fisica.posx = TOWORLDPOS(j);
                    gameState->entidades.jugador.fisica.ancho = 0;
                    gameState->entidades.jugador.fisica.alto = 0;
                    gameState->entidades.jugador.fisica.velx = 0;
                    gameState->entidades.jugador.fisica.vely = 0;
                    break;

                case CHEEPCHEEP:
                    gameState->entidades.enemigos[enemiesIndex]->sprite = 0;
                    gameState->entidades.enemigos[enemiesIndex]->identificador = CHEEPCHEEP;
                    gameState->entidades.enemigos[enemiesIndex]->fisica.posx = TOWORLDPOS(i);
                    gameState->entidades.enemigos[enemiesIndex]->fisica.posy = TOWORLDPOS(j);
                    gameState->entidades.enemigos[enemiesIndex]->fisica.ancho = PIXELSPERUNIT;
                    gameState->entidades.enemigos[enemiesIndex]->fisica.alto = PIXELSPERUNIT;
                    gameState->entidades.enemigos[enemiesIndex]->fisica.velx = -10;             //Le puse una velocidad al cheep cheep para la izquierda
                    gameState->entidades.enemigos[enemiesIndex]->fisica.vely = 0;
                    enemiesIndex++;
                    break;

                case PULPITO:
                    gameState->entidades.enemigos[enemiesIndex]->sprite = 0;
                    gameState->entidades.enemigos[enemiesIndex]->identificador = PULPITO;
                    gameState->entidades.enemigos[enemiesIndex]->fisica.posx = TOWORLDPOS(i);
                    gameState->entidades.enemigos[enemiesIndex]->fisica.posy = TOWORLDPOS(j);
                    gameState->entidades.enemigos[enemiesIndex]->fisica.ancho = PIXELSPERUNIT*2;
                    gameState->entidades.enemigos[enemiesIndex]->fisica.alto = PIXELSPERUNIT;
                    gameState->entidades.enemigos[enemiesIndex]->fisica.velx = -10;             //Le puse una velocidad al cheep cheep para la izquierda
                    gameState->entidades.enemigos[enemiesIndex]->fisica.vely = 0;
                    enemiesIndex++;
                    break;
            }
        }
    }
}