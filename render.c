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
static float cameraPosX = SCREENWIDHT/2;


//Si el juego debe renderizarse en la pantalla de la computadora
#if MODOJUEGO == 0

void *render (void *gs) {

    ALLEGRO_DISPLAY* disp;
    estadoJuego_t *gameState = (estadoJuego_t *) gs;
    int salida = 0;

    disp = al_create_display(SCREENWIDHT, SCREENHEIGHT);

    while (gameState->state != GAMECLOSED) {

        usleep(UTIEMPOREFRESCO*4);

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
            }
    }

    pthread_exit(NULL);

}

void updateCameraPosition(jugador_t* player){
    if (player->fisica.posx > cameraPosX) {
        scrollX += player->fisica.posx - cameraPosX;
        cameraPosX += scrollX;
    }
}

float getCameraScrollX(){
    return scrollX;
}

#elif MODOJUEGO == 1



#endif