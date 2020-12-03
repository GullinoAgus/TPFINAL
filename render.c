//
// Created by damian on 30/11/20.
//

#include <pthread.h>
#include "render.h"
#include "configuracion.h"
#include "menu.h"
#include "level.h"

static pthread_mutex_t padlock;

void *render (void *gs) {

    //Si el juego debe renderizarse en la pantalla de la computadora
    #if MODOJUEGO == 0

        estadoJuego_t *gameState = (estadoJuego_t *) gs;

        pthread_mutex_init(&padlock);


    while (gameState->state != GAMECLOSED) {
            if (gameState->threadTurn == RENDER) {
                switch (gameState->state) {

                    case MENU: //menu
                        drawMenu(&(gameState->buffer));
                        break;

                    case LEVELSELECTED: //seleccion de nivel


                        break;

                    case SCORETABLE: //tabla de scores


                        break;

                    case INGAME: //en juego
                        drawLevel(gameState, &(gameState->buffer));
                        break;
                }
            }
        }

        pthread_exit(NULL);

    #elif MODOJUEGO == 1



    #endif

}