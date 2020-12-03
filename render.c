//
// Created by damian on 30/11/20.
//

#include <pthread.h>
#include "render.h"
#include "allegro.h"
#include "level.h"
#include "menu.h"

_Noreturn void * renderizar (void * entrada){

    estadoJuego_t *gameState = (estadoJuego_t*) entrada;

    int closed_game = 0;

    while(!closed_game){

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