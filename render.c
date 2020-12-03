//
// Created by damian on 30/11/20.
//

#include <pthread.h>
#include "render.h"
#include "allegro.h"
#include "configuracion.h"
#include "menu.h"

//Si el juego debe renderizarse en la pantalla de la computdora
#ifdef MODOJUEGO == 0

    void *render (void *gs){

        estadoJuego_t *gameState = (estadoJuego_t*) gs;

        while(gameState->state != GAMECLOSED){

            switch (gameState->state) {

                case 0: //MENU

                    break;

                case 1: //Seleccion de niveles

                    break;

                case 2: //Tabla de scores

                    break;

                case 3: //En un nivel

                    break;
            }
        }

        pthread_exit(NULL);
    }

#elif MODOJUEGO == 1



#endif