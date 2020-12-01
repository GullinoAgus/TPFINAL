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


int cargarMapa(int ***mapa, int id, int* rows, int* columns) {

    FILE *mapData;
    int error = 0;

    if (openLevelData(&mapData, id) == 1) {
        error = -1;
    } else {
        fscanf(mapData, "%d %d", rows, columns);
        *mapa = (int **) malloc(*rows * sizeof(int *));
        for (int i = 0; i < *rows; i++) {
            (*mapa)[i] = (int *) malloc(*columns * sizeof(int));
        }


        do {
            char lectura = fgetc(mapData);
            if (LECTURAVALIDA(lectura)){
                *mapa++ = lectura;
                contadorChequeo++;
            }
        }while (lectura != EOF);

        for (int i = 0; i < *rows; i++) {
            for (int j = 0; j < *columns; j++) {
                if (j == *columns -
                         1) {        //Si la posicion donde se esta leyendo es la ultima columna, no tiene un punto y coma al final
                    fscanf(mapData, "%c", &((*mapa)[i][j]));
                } else {
                    fscanf(mapData, "%c;", &((*mapa)[i][j]));
                }
                printf("%d ", (*mapa)[i][j]);
            }
            printf("\n");
        }
    }

    fclose(mapData);
    return error;
}


int* leerNivel(const char* direccion){

    int* mapa;
    int cantBloques = verificarTamanioDeNivel(direccion);
    int contadorChequeo = 0;

    if (cantBloques <= 0){
        return NULL;
    }else {
        mapa = calloc(cantBloques, sizeof(int));
        FILE* nivel = fopen(direccion,"r");
        int lectura;

        do {
            lectura = fgetc(nivel);
            if (LECTURAVALIDA(lectura)){
                *mapa++ = lectura;
                contadorChequeo++;
            }
        }while (lectura != EOF);

        if (cantBloques != contadorChequeo){
            free(mapa);
            mapa = NULL;
        }
        fclose(nivel);
    }
    return (mapa - cantBloques);
}

/*
int verificarTamanioDeNivel(const  char* direccion){

    FILE* nivel = fopen(direccion, "r");
    if (nivel == NULL){
        fclose(nivel);
        return 0;
    }
    char car = 0;
    int celdaVacia = 0;
    int contadorCar = 0;
    int contadorLineas = 0;
    int contadorBordes = 0;
    int hayJugador = 0;
    do {
        car = fgetc(nivel);
        celdaVacia++;
        if (car != ';' && car != '\n' && car != EOF){
            celdaVacia = 0;
            contadorCar++;
            if (car == JUGADOR){
                hayJugador++;
            }else if (car == BORDE){
                contadorBordes++;
            }
        }
        if (celdaVacia == 2){
            celdaVacia = 1;
            contadorCar++;
        }
        if (car == '\n' || car == EOF){
            contadorLineas++;
        }
    }while (car != EOF);
    if (contadorLineas != 16){
        contadorCar = 0;
    } else if( hayJugador != 1){
        contadorCar = -1;
    } else if (((contadorBordes/2) != contadorLineas) || contadorBordes%2 !=0) {
        contadorCar = -2;
    }
    fclose(nivel);
    return contadorCar;

}*/

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
    for(int i = 0; i < gameState->levelWidht; i++){
        for(int j = 0; j < gameState->levelHeight; j++) {
            switch(gameState->level[i][j]) {
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


    for(int i = 0; i < gameState->levelWidht; i++){
        for(int j = 0; j < gameState->levelHeight; j++){
            switch ((gameState->level)[i][j]) {

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