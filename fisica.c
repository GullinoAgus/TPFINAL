//
// Created by agus on 23/11/20.
//
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include "fisica.h"
#include "matiasBrosGame.h"

void* fisica(void* entrada){

    estadoJuego_t *gameState = entrada;

    while(gameState->state != GAMECLOSED) {
        if (gameState->threadTurn == PHYSICS) {

            usleep(UTIEMPOREFRESCO);

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

            gameState->entidades.jugador.fisica.vely += GRAVEDAD;
            gameState->entidades.jugador.fisica.velx *= INERCIA;

            //if(a.max.x < b.min.x or a.min.x > b.max.x) return false;
            //if(a.max.y < b.min.y or a.min.y > b.max.y) return false;
            for (int i = 0; gameState->entidades.enemigos[i].identificador != NULLENTITIE; ++i) {
                if ((gameState->entidades.jugador.fisica.posx + gameState->entidades.jugador.fisica.ancho) >
                    gameState->entidades.enemigos[i].fisica.posx &&
                    gameState->entidades.jugador.fisica.posx <
                    (gameState->entidades.enemigos[i].fisica.posx + gameState->entidades.enemigos[i].fisica.ancho) &&
                    (gameState->entidades.jugador.fisica.posy + gameState->entidades.jugador.fisica.alto) >
                    gameState->entidades.enemigos[i].fisica.posy &&
                    gameState->entidades.jugador.fisica.posy <
                    (gameState->entidades.enemigos[i].fisica.posy + gameState->entidades.enemigos[i].fisica.alto)) {

                }
            }

            // COLISIONES
            for (int i = 0; gameState->entidades.bloques[i].identificador != NULLENTITIE; ++i) {
                if ((gameState->entidades.jugador.fisica.posx + gameState->entidades.jugador.fisica.ancho) >
                    gameState->entidades.bloques[i].fisica.posx &&
                    gameState->entidades.jugador.fisica.posx <
                    (gameState->entidades.bloques[i].fisica.posx + gameState->entidades.bloques[i].fisica.ancho) &&
                    (gameState->entidades.jugador.fisica.posy + gameState->entidades.jugador.fisica.alto) >
                    gameState->entidades.bloques[i].fisica.posy &&
                    gameState->entidades.jugador.fisica.posy <
                    (gameState->entidades.bloques[i].fisica.posy + gameState->entidades.bloques[i].fisica.alto)) {

                    if ((gameState->entidades.jugador.fisica.posx + gameState->entidades.jugador.fisica.ancho -
                         gameState->entidades.bloques[i].fisica.posx <= VELOCIDADXMAX) !=
                        (VELOCIDADXMAX >=
                         (gameState->entidades.bloques[i].fisica.posx + gameState->entidades.bloques[i].fisica.ancho) -
                         gameState->entidades.jugador.fisica.posx)) {

                        if (gameState->entidades.jugador.fisica.posx <
                            gameState->entidades.bloques[i].fisica.posx) { //Choque por izquierda
                            gameState->entidades.jugador.fisica.posx =
                                    gameState->entidades.bloques[i].fisica.posx -
                                    gameState->entidades.jugador.fisica.ancho;
                        } else if ((
                                gameState->entidades.jugador.fisica.posx - gameState->entidades.jugador.fisica.ancho <
                                gameState->entidades.bloques[i].fisica.posx +
                                gameState->entidades.bloques[i].fisica.ancho)) {
                            gameState->entidades.jugador.fisica.posx =
                                    gameState->entidades.bloques[i].fisica.posx +
                                    gameState->entidades.bloques[i].fisica.ancho;
                        }
                    } else if (((gameState->entidades.jugador.fisica.posy + gameState->entidades.jugador.fisica.alto) >
                                gameState->entidades.bloques[i].fisica.posy) !=
                               ((gameState->entidades.jugador.fisica.posy) >
                                (gameState->entidades.bloques[i].fisica.posy +
                                 gameState->entidades.bloques[i].fisica.alto))) {
                        gameState->entidades.jugador.fisica.vely = 0;
                        if (gameState->entidades.jugador.fisica.posy <
                            gameState->entidades.bloques[i].fisica.posy) { //las patas
                            gameState->entidades.jugador.fisica.posy =
                                    gameState->entidades.bloques[i].fisica.posy -
                                    gameState->entidades.jugador.fisica.alto;
                            gameState->entidades.jugador.sobreBloque = true;
                        } else {
                            gameState->entidades.jugador.fisica.posy =
                                    gameState->entidades.bloques[i].fisica.posy +
                                    gameState->entidades.bloques[i].fisica.alto;
                        }
                    }
                }
            }
            gameState->threadTurn = GAMELOGIC;
        }
    }

    pthread_exit(NULL);
}



