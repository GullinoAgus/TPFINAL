//
// Created by agus on 23/11/20.
//
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include "fisica.h"
#include "matiasBrosGame.h"
#include "semaphore.h"
#include "gamelogic.h"

static int isColliding(fisica_t* object1, fisica_t* object2);

void* fisica(void* entrada){

    estadoJuego_t *gameState = entrada;

    while(gameState->state != GAMECLOSED) {

        usleep(UTIEMPOREFRESCO*2);

        if (gameState->entidades.jugador.fisica.velx > VELOCIDADXMAX) {
            gameState->entidades.jugador.fisica.velx = VELOCIDADXMAX;
        }
        if (gameState->entidades.jugador.fisica.vely > VELOCIDADYMAX) {
            gameState->entidades.jugador.fisica.vely = VELOCIDADYMAX;
        }

        // ACTUALIZACION DE POSICIONES
        gameState->entidades.jugador.fisica.posx += gameState->entidades.jugador.fisica.velx;
        gameState->entidades.jugador.fisica.posy += gameState->entidades.jugador.fisica.vely;

        if (gameState->entidades.jugador.sobreBloque && gameState->entidades.jugador.fisica.vely != 0) {
            gameState->entidades.jugador.sobreBloque = false;
        }

        for (int i = 0; gameState->entidades.enemigos[i].identificador != NULLENTITIE; ++i) {
            gameState->entidades.enemigos[i].fisica.posx += gameState->entidades.enemigos[i].fisica.velx;
            gameState->entidades.enemigos[i].fisica.posy += gameState->entidades.enemigos[i].fisica.vely;
        }

        for (int i = 0; gameState->entidades.bloques[i].identificador != NULLENTITIE; ++i) {    //Añado esto par que los bloques tambien puedan desplazarse
            gameState->entidades.bloques[i].fisica.posx += gameState->entidades.bloques[i].fisica.velx;
        }

        gameState->entidades.jugador.fisica.vely += GRAVEDAD;
        gameState->entidades.jugador.fisica.velx *= INERCIA;

            //if(a.max.x < b.min.x or a.min.x > b.max.x) return false;
            //if(a.max.y < b.min.y or a.min.y > b.max.y) return false;
        for (int i = 0; gameState->entidades.enemigos[i].identificador != NULLENTITIE; ++i) {
            if (isColliding(&gameState->entidades.jugador.fisica, &gameState->entidades.enemigos[i].fisica)){
                //TODO: Solo si el jugador es vulnerable bajarle vidas
                if(1){
                    gameState->entidades.jugador.vidas--;
                    if(gameState->entidades.jugador.vidas <= 0) {
                        gameState->entidades.jugador.estado = ALMOST_DEAD;
                        break;  //FIXME: No se si prefieren ponerle un variable para salir de esto o lo dejamos asi
                    }
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

static int isColliding(fisica_t* object1, fisica_t* object2){
    int collision = 0;
    if( ((object1->posx + object1->ancho) > object2->posx) && (object1->posx < (object2->posx + object2->ancho)) &&
        ((object1->posy + object1->alto) > object2->posy) && (object1->posy < (object2->posy + object2->alto))){
        collision = 1;
    }

    return collision;
}



