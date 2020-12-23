//
// Created by agus on 23/11/20.
//
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include "fisica.h"
#include "matiasBrosGame.h"
#include "IEvents.h"



static int isColliding(fisica_t* object1, fisica_t* object2);

void* fisica(void* entrada){

    estadoJuego_t *gameState = entrada;

    while(gameState->state != GAMECLOSED) {

        while(gameState->state == PAUSE);

        usleep(UTIEMPOREFRESCO);

        if (gameState->entidades.jugador.fisica.velx > VELOCIDADXMAX) {
            gameState->entidades.jugador.fisica.velx = VELOCIDADXMAX;
        }
        if (gameState->entidades.jugador.fisica.vely > VELOCIDADYMAX) {
            gameState->entidades.jugador.fisica.vely = VELOCIDADYMAX;
        }

        // ACTUALIZACION DE POSICIONES
        gameState->entidades.jugador.fisica.posx += gameState->entidades.jugador.fisica.velx*UTIEMPOREFRESCO/1000;
        gameState->entidades.jugador.fisica.posy += gameState->entidades.jugador.fisica.vely*UTIEMPOREFRESCO/1000;

        if (gameState->entidades.jugador.sobreBloque && gameState->entidades.jugador.fisica.vely != 0) {
            gameState->entidades.jugador.sobreBloque = false;
        }

        for (int i = 0; gameState->entidades.enemigos[i].identificador != NULLENTITIE; ++i) {
            gameState->entidades.enemigos[i].fisica.posx += gameState->entidades.enemigos[i].fisica.velx*UTIEMPOREFRESCO/1000;
            gameState->entidades.enemigos[i].fisica.posy += gameState->entidades.enemigos[i].fisica.vely*UTIEMPOREFRESCO/1000;
        }

        gameState->entidades.jugador.fisica.vely += GRAVEDAD;
        if (!gameState->entidades.jugador.isMoving) {
            gameState->entidades.jugador.fisica.velx *= INERCIA;
        }

        if (gameState->entidades.jugador.fisica.posy < 32){ //MANTIENE QUE MARIO NO SE ZARPE DEL TECHO

            gameState->entidades.jugador.fisica.posy += (32 - gameState->entidades.jugador.fisica.posy);
            gameState->entidades.jugador.fisica.vely = 0.0f;
        }
        if (gameState->entidades.jugador.fisica.posx < 2){  //MANTIENE QUE MARIO NO SE ZARPE DE LA IZQUIERDA

            if (gameState->entidades.jugador.fisica.posx >0){

                gameState->entidades.jugador.fisica.posx += (gameState->entidades.jugador.fisica.posx);
            }
            else{

                gameState->entidades.jugador.fisica.posx += -(gameState->entidades.jugador.fisica.posx);
            }

            gameState->entidades.jugador.fisica.velx = 0.0f;
        }


            //if(a.max.x < b.min.x or a.min.x > b.max.x) return false;
            //if(a.max.y < b.min.y or a.min.y > b.max.y) return false;

            //Si colisiona con algun enemigo
        for (int i = 0; gameState->entidades.enemigos[i].identificador != NULLENTITIE; ++i) {
            if (isColliding(&gameState->entidades.jugador.fisica, &gameState->entidades.enemigos[i].fisica)){
                if(gameState->entidades.jugador.estado != INVULNERABLE){        //Si puede ser dañado
                    if(gameState->entidades.jugador.powerUpsState == SMALL){    //Si es chiquito
                        gameState->entidades.jugador.estado = DEAD;
                    }
                    else if(gameState->entidades.jugador.powerUpsState == BIG){ //Si es grande
                        gameState->entidades.jugador.powerUpsState = SMALL;     //Lo hacemos chiquito
                    }
                    break;
                }
            }
        }

            // COLISIONES
            for (int i = 0; gameState->entidades.bloques[i].identificador != NULLENTITIE; ++i) {
                if (isColliding(&gameState->entidades.jugador.fisica, &gameState->entidades.bloques[i].fisica)) {

                    if ((gameState->entidades.jugador.fisica.posx + gameState->entidades.jugador.fisica.ancho -
                         gameState->entidades.bloques[i].fisica.posx <= VELOCIDADXMAX) !=
                        (VELOCIDADXMAX >=
                         (gameState->entidades.bloques[i].fisica.posx + gameState->entidades.bloques[i].fisica.ancho) -
                         gameState->entidades.jugador.fisica.posx)) {

                            if (gameState->entidades.jugador.fisica.posx < gameState->entidades.bloques[i].fisica.posx) { //Choque por izquierda
                                gameState->entidades.jugador.fisica.posx = gameState->entidades.bloques[i].fisica.posx - gameState->entidades.jugador.fisica.ancho;
                            } else if ((
                                    gameState->entidades.jugador.fisica.posx - gameState->entidades.jugador.fisica.ancho <
                                    gameState->entidades.bloques[i].fisica.posx +
                                    gameState->entidades.bloques[i].fisica.ancho)) {
                                        gameState->entidades.jugador.fisica.posx = gameState->entidades.bloques[i].fisica.posx + gameState->entidades.bloques[i].fisica.ancho;
                            }
                    } else if (((gameState->entidades.jugador.fisica.posy + gameState->entidades.jugador.fisica.alto) >
                                gameState->entidades.bloques[i].fisica.posy) !=
                               ((gameState->entidades.jugador.fisica.posy) >
                                (gameState->entidades.bloques[i].fisica.posy +
                                 gameState->entidades.bloques[i].fisica.alto))) {

                                    gameState->entidades.jugador.fisica.vely = 0;
                                    if (gameState->entidades.jugador.fisica.posy < gameState->entidades.bloques[i].fisica.posy) { //las patas
                                        gameState->entidades.jugador.fisica.posy = gameState->entidades.bloques[i].fisica.posy - gameState->entidades.jugador.fisica.alto;
                                        gameState->entidades.jugador.sobreBloque = true;
                                    } else {
                                        gameState->entidades.jugador.fisica.posy = gameState->entidades.bloques[i].fisica.posy + gameState->entidades.bloques[i].fisica.alto;
                                    }
                    }
                }
            }
    }

    pthread_exit(NULL);
}

void movePlayer(int direction, void* player){

    jugador_t* matias = player;
    static int ultimoEvento;

    switch (direction) {

        case DOWNIZQUIERDA:
            matias->isMoving = true;
            matias->fisica.velx = -0.6f;
            break;

        case DOWNDERECHA:
            matias->isMoving = true;
            matias->fisica.velx = 0.6f;
            break;

        case UPDERECHA:
        case UPIZQUIERDA:
            matias->isMoving = false;
            break;

        case DOWNARRIBA:
            matias->fisica.vely = -1.0f;
            break;

            // A continuacion tambien los del joystick, los cuales no se tiene acceso desde las flechitas
        case DOWNARRIBADERECHA:
            if (ultimoEvento != DOWNARRIBADERECHA) {
                matias->fisica.vely = -1.0f;
                matias->isMoving = true;
                matias->fisica.velx = 0.6f;
            }
            break;
        case DOWNARRIBAIZQUIERDA:
            if (ultimoEvento != DOWNARRIBAIZQUIERDA) {
                matias->fisica.vely = -1.0f;
                matias->isMoving = true;
                matias->fisica.velx = -0.6f;
            }
            break;
    }
    ultimoEvento = direction;
}

static int isColliding(fisica_t* object1, fisica_t* object2){
    int collision = 0;
    if( ((object1->posx + object1->ancho) > object2->posx) && (object1->posx < (object2->posx + object2->ancho)) &&
        ((object1->posy + object1->alto) > object2->posy) && (object1->posy < (object2->posy + object2->alto))){
        collision = 1;
    }

    return collision;
}



