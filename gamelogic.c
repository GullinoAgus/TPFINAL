//
// Created by damian on 30/11/20.
//

#include <pthread.h>
#include "gamelogic.h"
#include "IEvents.h"

static pthread_mutex_t padlock;

void *gamelogic (void *p2GameState) {

    estadoJuego_t *gameState = (estadoJuego_t *) p2GameState;
    char evento = 0;
    char ultimoEvento = 0;

    pthread_mutex_init(&padlock);

    while (gameState->state != GAMECLOSED) {
        if(gameState->threadTurn == GAMELOGIC) {

            pthread_mutex_lock(&padlock);

            /*
            ultimoEvento = evento;

            evento = getInputEvent();
            ultimoEvento = evento;
            */

            switch (gameState->state) {

                case MENU:

                    if (evento == DOWNBOTON) {

                        if (gameState->menuSelection == 1) {
                            gameState->state = 3;
                        } else if (gameState->menuSelection == 2) {

                            gameState->state = 2;
                        } else if (gameState->menuSelection == 3) {

                            gameState->state = 1;
                        }
                    } else if ((evento == DOWNABAJO) || (evento == DOWNDERECHA)) {

                        if (gameState->menuSelection == 1) {

                            gameState->menuSelection = 2;
                        } else if (gameState->menuSelection == 2) {

                            gameState->menuSelection = 3;
                        }
                    } else if ((evento == DOWNARRIBA) || (evento == DOWNIZQUIERDA)) {

                        if (gameState->menuSelection == 2) {

                            gameState->menuSelection = 1;
                        } else if (gameState->menuSelection == 3) {

                            gameState->menuSelection = 2;
                        }
                    }
                    break;

                case LEVELSELECTED: //seleccion de nivel

                    break;

                case SCORETABLE: //tabla de scores

                    if (evento == DOWNBOTON) {

                        gameState->menuSelection = 0;
                    }
                    break;

                case INGAME: //en juego

                    switch (evento) {

                        case DOWNIZQUIERDA:
                            (gameState->entidades).jugador.fisica.velx = -1.0f;
                            break;
                        case DOWNDERECHA:
                            (gameState->entidades).jugador.fisica.velx = 1.0f;
                            break;
                        case DOWNARRIBA:
                            if (ultimoEvento != DOWNARRIBA) {
                                (gameState->entidades).jugador.fisica.vely = -1.5f;
                            }
                            break;
                        case UPIZQUIERDA:
                            (gameState->entidades).jugador.fisica.velx = 0.0f;
                            break;
                        case UPDERECHA:
                            (gameState->entidades).jugador.fisica.velx = 0.0f;
                            break;
                        case DOWNABAJO:
                            (gameState->entidades).jugador.fisica.vely = 1.0f;
                            break;


                            // A continuacion tambien los del joystick, los cuales no se tiene acceso desde las flechitas
                        case DOWNARRIBADERECHA:
                            if (ultimoEvento != DOWNARRIBADERECHA) {
                                (gameState->entidades).jugador.fisica.vely = -1.0f;
                                (gameState->entidades).jugador.fisica.velx = 0.5f;
                            }
                            break;
                        case DOWNARRIBAIZQUIERDA:
                            if (ultimoEvento != DOWNARRIBAIZQUIERDA) {
                                (gameState->entidades).jugador.fisica.vely = -1.0f;
                                (gameState->entidades).jugador.fisica.velx = -0.5f;
                            }
                            break;
                        case DOWNABAJODERECHA:
                            if (ultimoEvento != DOWNABAJODERECHA) {
                                (gameState->entidades).jugador.fisica.vely = 1.0f;
                                (gameState->entidades).jugador.fisica.velx = 0.5f;
                            }
                            break;
                        case DOWNABAJOIZQUIERDA:
                            if (ultimoEvento != DOWNABAJOIZQUIERDA) {
                                (gameState->entidades).jugador.fisica.vely = 1.0f;
                                (gameState->entidades).jugador.fisica.velx = -0.5f;
                            }
                            break;
                    }

            }

            pthread_mutex_unlock(&padlock);
            gameState->threadTurn = INPUT //Pasamos al siguiente thread
        }
    }

    pthread_mutex_destroy(&padlock);
    pthread_exit(NULL);
}
