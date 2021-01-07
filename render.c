//
// Created by damian on 30/11/20.
//

#include <pthread.h>
#include "render.h"
#include "configuracion.h"
#include "menu.h"
#include "level.h"
#include "gamelogic.h"
#include "times.h"

static float scrollX = 0.0f;
static int redrawNow = 0;

static void redraw(void* gs);

//Si el juego debe renderizarse en la pantalla de la computadora
#if MODOJUEGO == 0

void *render (void *gs) {

    ALLEGRO_DISPLAY* disp;
    estadoJuego_t *gameState = (estadoJuego_t *) gs;
    int salida = 0;

    disp = al_create_display(SCREENWIDHT, SCREENHEIGHT);

    createNewTimer(1.0f/FPS, redraw, FPSTIMER);
    startTimer(FPSTIMER);

    while (gameState->state != GAMECLOSED) {

        if(redrawNow) {
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
                    if (wasLevelInitialized()) {
                        drawLevel(gameState);
                    }
                    break;

                case RETRYSCREEN:
                    drawRetryScreen(gameState);
                    sleep(2);
                    gameState->state = INGAME;
                    gameState->gameUI.time = 400;
                    startTimer(INGAMETIMER);
                    break;

                case PAUSE:
                    drawPause(gameState);
                    break;

                case NEXTLEVEL:
                    drawNextLevel(gameState);
                    break;
            }

            redrawNow = 0;
        }
    }


    stopTimer(FPSTIMER);
    free(disp);
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

static void redraw(void* gs){
    redrawNow = 1;
}

#elif MODOJUEGO == 1



#endif