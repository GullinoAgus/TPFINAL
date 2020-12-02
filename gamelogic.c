//
// Created by damian on 30/11/20.
//

#include <pthread.h>
#include "gamelogic.h"
#include "IEvents.h"

_Noreturn void * gamelogic (void *p2GameState) {

    estadoJuego_t *gameState = (estadoJuego_t *) p2GameState;
    int closed_game = 0;

    while (!closed_game) {

        while ( esBufferVacio() );
        char evento = getInputEvent();

        switch (gameState->state) {

            case 0: //menu

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

            case 1: //seleccion de nivel

                break;

            case 2: //tabla de scores

                if (evento == DOWNBOTON){

                    gameState->menuSelection = 0;
                }
                break;

            case 3: //en juego

                switch (evento) {

                    case DOWNIZQUIERDA:
                        (gameState->entidades).jugador.fisica.velx = -1.0f;
                        break;
                    case DOWNDERECHA:
                        (gameState->entidades).jugador.fisica.velx = 1.0f;
                        break;
                    case DOWNARRIBA:
                        (gameState->entidades).jugador.fisica.vely = -1.0f;
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
                }

        }
    }
    pthread_exit(NULL);
}