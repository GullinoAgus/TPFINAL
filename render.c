//
// Created by damian on 30/11/20.
//

#include "render.h"
#include "allegro.h"

_Noreturn void * render (void *p2GameState){

    estadoJuego_t * gameState = (estadoJuego_t*) p2GameState;
    int closed_game = 0;

    while(!closed_game){

        switch (gameState->state) {

            case 0: //MENU



                break;

            case 1:

                break;

            case 2:

                break;

            case 3:

                break;
        }
    }
}