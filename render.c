//
// Created by damian on 30/11/20.
//

#include <pthread.h>
#include "render.h"
#include "configuracion.h"
#include "menu.h"
#include "level.h"
#include "semaphore.h"
#include "gamelogic.h"

static float scrollX = 0.0f;

//Si el juego debe renderizarse en la pantalla de la computadora
#if MODOJUEGO == 0

void *render (void *gs) {

    ALLEGRO_DISPLAY* disp;
    estadoJuego_t *gameState = (estadoJuego_t *) gs;
    int salida = 0;

    disp = al_create_display(SCREENWIDHT, SCREENHEIGHT);

    while (gameState->state != GAMECLOSED) {

        usleep(UTIEMPOREFRESCO);

        switch (gameState->state) {

            case MENU: //menu
                salida = drawMenu(gameState);
                break;

            case CHOOSINGLEVEL: //seleccion de nivel
                drawLevelSelector(gameState);
                break;

            case INSCORETABLE: //tabla de scores
                drawTopScores(gameState);
                break;

            case INGAME: //en juego
                if(wasLevelInitialized()){
                    drawLevel(gameState);
                }
                break;

            case RETRYSCREEN:
                drawRetryScreen(gameState);
                sleep(2);
                gameState->state = INGAME;
                break;

            case PAUSE:
                drawPause(gameState);
                break;
            }
    }

    pthread_exit(NULL);

}

void updateCameraPosition(jugador_t* player){
    if (player->fisica.posx > (SCREENWIDHT/2 + scrollX)) {
        scrollX = player->fisica.posx - SCREENWIDHT/2;
    }
}

void setCameraScrollX(float coordX){
    scrollX = coordX;
}

float getCameraScrollX(){
    return scrollX;
}

#elif MODOJUEGO == 1



#endif