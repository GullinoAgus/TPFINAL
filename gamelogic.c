//
// Created by damian on 30/11/20.
//

#include <pthread.h>
#include "gamelogic.h"
#include "IEvents.h"

void * gamelogic (void *p2GameState) {

    estadoJuego_t *gameState = (estadoJuego_t *) p2GameState;

    while (gameState->state != GAMECLOSED) {

        char evento = getInputEvent();

        switch (gameState->state) {

            case 0:

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

            case 1:

                break;

            case 2:

                break;

            case 3:

                break;
        }
    }

    pthread_exit(NULL);
}