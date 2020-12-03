//
// Created by damian on 30/11/20.
//

#include <pthread.h>
#include "render.h"
#include "configuracion.h"
#include "menu.h"
#include "level.h"


//Si el juego debe renderizarse en la pantalla de la computadora
#if MODOJUEGO == 0

void *render (void *gs) {

    ALLEGRO_DISPLAY* disp;
    estadoJuego_t *gameState = (estadoJuego_t *) gs;
    int salida = 0;

    disp = al_create_display(SCREENWIDHT, SCREENHEIGHT);

    while (gameState->state != GAMECLOSED) {
        if (gameState->threadTurn == RENDER) {

            switch (gameState->state) {

                case MENU: //menu
                    salida = drawMenu(gameState);
                    break;

                case LEVELSELECTOR: //seleccion de nivel


                    break;

                case SCORETABLE: //tabla de scores


                    break;

                case INGAME: //en juego
                    drawLevel(gameState);
                    break;
            }
        }

        gameState->threadTurn = ANIMATION;
    }

    pthread_exit(NULL);

}

#elif MODOJUEGO == 1



#endif