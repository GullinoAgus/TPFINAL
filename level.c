//
// Created by gonzalo on 27/11/20.
//

#include "level.h"
#include "data.h"
#include <stdlib.h>
#include "allegro.h"

#include "allegroLib.h"

#define TOWORLDPOS(v) (v*PIXELSPERUNIT)
#define LECTURAVALIDA(x) ((x) == JUGADOR || (x) == BORDE || (x) == NADA || (x) == LADRILLO || (x) == ALGA || (x) == CHEEPCHEEP || (x) == PULPITO)


int cargarMapa(level_t* level , int id) {

    FILE *mapData;
    int i = 0;
    int j = 0;
    char read;
    int auxCont;
    int borderCount = 0;


    int error = openLevelData(&mapData, id);

    if (error != 1){
        countColumns(level, mapData);
        level->level = (int **) calloc( level->levelHeight, sizeof(int *));
        for (i = 0; i < level->levelHeight; i++) {
            level->level[i] = (int *) malloc(level->levelWidht * sizeof(int));
        }
        i = 0;
        do {
            read =fgetc(mapData);
            switch (read) {
                case CHEEPCHEEP:
                case PULPITO:
                case ALGA:
                case LADRILLO:
                case JUGADOR:
                case NADA:
                    level->level[i][j] = read;
                    j++;
                    auxCont = 0;
                    break;
                case ';':
                    auxCont++;
                    break;
                case BORDE:
                    borderCount++;
                    if (borderCount == 2){
                        i++;
                        j = 0;
                        borderCount = 0;
                    }
                    auxCont = 0;
                    break;
                default:
                    break;

            }
            if (auxCont == 2){
                level->level[i][j] = NADA;
                j++;
                auxCont--;
            }
        }while (read != EOF);
    }

    fclose(mapData);
    return error;
}


int countColumns(level_t* level, FILE* mapData){

    int error = 0;
    int colNum = 0;
    char read = 0;
    int borderCount = 0;
    int auxCont;

    do {
        read = fgetc(mapData);
        switch (read) {
            case CHEEPCHEEP:
            case PULPITO:
            case ALGA:
            case LADRILLO:
            case JUGADOR:
            case NADA:
                colNum++;
                auxCont = 0;
                break;
            case ';':
                auxCont++;
                break;
            case BORDE:
                borderCount++;
                auxCont = 0;
                break;
            default:
                break;

        }
        if (auxCont == 2){
            colNum++;
            auxCont--;
        }
    }while (read != EOF);

    if (borderCount%2 == 1){
        error = 1;
    } else{
        level->levelHeight = borderCount/2;
        level->levelWidht = colNum/level->levelHeight;
    }
    fseek( mapData, 0, SEEK_SET );
    return error;
}

void drawLevel(estadoJuego_t *gameState, bufferRecursos_t *resourceBuffer){

    int i = 0;
    al_clear_to_color(al_map_rgb(0, 0, 0));

    //Mientras no se hayan leido todos los bloques, dibujamos el siguiente
    while(gameState->entidades.bloques[i].identificador != NULLENTITIE){

        switch (gameState->entidades.bloques[i].identificador){
            case ALGA:
                al_draw_bitmap(resourceBuffer->image[6], gameState->entidades.bloques[i].fisica.posx, gameState->entidades.bloques[i].fisica.posy, 0);
                break;

            case LADRILLO:
                al_draw_bitmap(resourceBuffer->image[7], gameState->entidades.bloques[i].fisica.posx, gameState->entidades.bloques[i].fisica.posy, 0);
                break;
        }

        i++;
    }

    while(gameState->entidades.enemigos[i].identificador != NULLENTITIE){

        switch (gameState->entidades.enemigos[i].identificador){
            case PULPITO:
                al_draw_bitmap(resourceBuffer->image[5], gameState->entidades.bloques[i].fisica.posx, gameState->entidades.bloques[i].fisica.posy, 0);
                break;

            case CHEEPCHEEP:
                al_draw_bitmap(resourceBuffer->image[4], gameState->entidades.bloques[i].fisica.posx, gameState->entidades.bloques[i].fisica.posy, 0);
                break;
        }

        i++;
    }

    al_flip_display();

}


int initEntities(estadoJuego_t* gameState){

    int blocksCounter = 0;
    int enemiesCounter = 0;
    int blocksIndex = 0;
    int enemiesIndex = 0;

    //Calculamos la cantidad de enemigos y de bloques que hay en el mapa
    for(int i = 0; i < gameState->level.levelHeight; i++){
        for(int j = 0; j < gameState->level.levelWidht; j++) {
            switch(gameState->level.level[i][j]) {
                case LADRILLO:
                    blocksCounter++;
                    break;

                case CHEEPCHEEP:
                case PULPITO:
                    enemiesCounter++;
                    break;
            }
        }
    }

    //Reservamos el espacio para los bloques
    gameState->entidades.bloques = (bloque_t*) malloc(sizeof(bloque_t) * (blocksCounter+1));
    if(gameState->entidades.bloques == NULL){
        printf("Error al reservar espacio para los bloques");
        return 1;
    }
    gameState->entidades.bloques[blocksCounter].identificador = NULLENTITIE;         //Inicializamos el ultimo elemento en nulo

    //Reservamos el espacio para los enemigos
    gameState->entidades.enemigos = (enemigo_t*) malloc(sizeof(enemigo_t) * (enemiesCounter+1));
    if(gameState->entidades.enemigos == NULL){
        printf("Error al reservar espacio para los enemigos");
        return 1;
    }
    gameState->entidades.enemigos[enemiesCounter].identificador = NULLENTITIE;       //Inicializamos el ultimo elemento en nulo


    for(int i = 0; i < gameState->level.levelWidht; i++){
        for(int j = 0; j < gameState->level.levelHeight; j++){
            switch (gameState->level.level[j][i]) {

                case LADRILLO:
                    gameState->entidades.bloques[blocksIndex].sprite = 0;
                    gameState->entidades.bloques[blocksIndex].identificador = LADRILLO;
                    gameState->entidades.bloques[blocksIndex].fisica.posx = TOWORLDPOS(i);
                    gameState->entidades.bloques[blocksIndex].fisica.posy = TOWORLDPOS(j);
                    gameState->entidades.bloques[blocksIndex].fisica.ancho = PIXELSPERUNIT;
                    gameState->entidades.bloques[blocksIndex].fisica.alto = PIXELSPERUNIT;
                    gameState->entidades.bloques[blocksIndex].fisica.velx = 0;
                    gameState->entidades.bloques[blocksIndex].fisica.vely = 0;
                    blocksIndex++;
                    break;

                case JUGADOR:
                    gameState->entidades.jugador.vidas = 3;
                    gameState->entidades.jugador.sobreBloque = 0;
                    gameState->entidades.jugador.estado = 1;                                //VIVO?
                    gameState->entidades.jugador.sprite = 0;
                    gameState->entidades.jugador.fisica.posx = TOWORLDPOS(i);
                    gameState->entidades.jugador.fisica.posx = TOWORLDPOS(j);
                    gameState->entidades.jugador.fisica.ancho = 0;
                    gameState->entidades.jugador.fisica.alto = 0;
                    gameState->entidades.jugador.fisica.velx = 0;
                    gameState->entidades.jugador.fisica.vely = 0;
                    break;

                case CHEEPCHEEP:
                    gameState->entidades.enemigos[enemiesIndex].sprite = 0;
                    gameState->entidades.enemigos[enemiesIndex].identificador = CHEEPCHEEP;
                    gameState->entidades.enemigos[enemiesIndex].fisica.posx = TOWORLDPOS(i);
                    gameState->entidades.enemigos[enemiesIndex].fisica.posy = TOWORLDPOS(j);
                    gameState->entidades.enemigos[enemiesIndex].fisica.ancho = PIXELSPERUNIT;
                    gameState->entidades.enemigos[enemiesIndex].fisica.alto = PIXELSPERUNIT;
                    gameState->entidades.enemigos[enemiesIndex].fisica.velx = -10;             //Le puse una velocidad al cheep cheep para la izquierda
                    gameState->entidades.enemigos[enemiesIndex].fisica.vely = 0;
                    enemiesIndex++;
                    break;

                case PULPITO:
                    gameState->entidades.enemigos[enemiesIndex].sprite = 0;
                    gameState->entidades.enemigos[enemiesIndex].identificador = PULPITO;
                    gameState->entidades.enemigos[enemiesIndex].fisica.posx = TOWORLDPOS(i);
                    gameState->entidades.enemigos[enemiesIndex].fisica.posy = TOWORLDPOS(j);
                    gameState->entidades.enemigos[enemiesIndex].fisica.ancho = PIXELSPERUNIT*2;
                    gameState->entidades.enemigos[enemiesIndex].fisica.alto = PIXELSPERUNIT;
                    gameState->entidades.enemigos[enemiesIndex].fisica.velx = -10;             //Le puse una velocidad al blooper para la izquierda
                    gameState->entidades.enemigos[enemiesIndex].fisica.vely = 0;
                    enemiesIndex++;
                    break;

                case ALGA:
                    gameState->entidades.bloques[blocksIndex].sprite = 0;
                    gameState->entidades.bloques[blocksIndex].identificador = ALGA;
                    gameState->entidades.bloques[blocksIndex].fisica.posx = TOWORLDPOS(i);
                    gameState->entidades.bloques[blocksIndex].fisica.posy = TOWORLDPOS(j);
                    gameState->entidades.bloques[blocksIndex].fisica.ancho = PIXELSPERUNIT;
                    gameState->entidades.bloques[blocksIndex].fisica.alto = PIXELSPERUNIT * 2;
                    gameState->entidades.bloques[blocksIndex].fisica.velx = 0;
                    gameState->entidades.bloques[blocksIndex].fisica.vely = 0;
                    blocksIndex++;
                    break;
            }
        }
    }

    return 0;
}