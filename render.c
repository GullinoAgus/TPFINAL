//
// Created by damian on 30/11/20.
//

#include <pthread.h>
#include "render.h"
#include "allegro.h"
#include "configuracion.h"
#include "menu.h"
#include "level.h"

//Si el juego debe renderizarse en la pantalla de la computdora
#ifdef MODOJUEGO == 0

    void *render (void *gs){
        estadoJuego_t *gameState = (estadoJuego_t*) gs;

        while(gameState->state != GAMECLOSED){

            switch (gameState->state) {

                case 0: //menu
                    drawMenu( &(gameState->buffer) );
                    break;

                case 1: //seleccion de nivel


                    break;

                case 2: //tabla de scores


                    break;

                case 3: //en juego
                    drawLevel(gameState, &(gameState->buffer));
                    break;
            }
        }

        pthread_exit(NULL);
    }

#elif MODOJUEGO == 1



#endif