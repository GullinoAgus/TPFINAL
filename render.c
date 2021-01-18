//
// Created by damian on 30/11/20.
//

#include <pthread.h>
#include <semaphore.h>
#include "render.h"
#include "configuracion.h"
#include "menu.h"
#include "level.h"
#include "gamelogic.h"
#include "times.h"
#include <unistd.h>

sem_t renderSem;

static float scrollX = 0.0f;

static void redraw(void* gs);

//Si el juego debe renderizarse en la pantalla de la computadora
#if MODOJUEGO == 0

void *render (void *gs) {

    ALLEGRO_DISPLAY* disp;
    estadoJuego_t *gameState = (estadoJuego_t *) gs;
    sem_init(&renderSem, 0, 1);

    disp = al_create_display(SCREENWIDHT, SCREENHEIGHT);

    createNewTimer(1.0f/FPS, redraw, FPSTIMER);
    startTimer(FPSTIMER);

    while (gameState->state != GAMECLOSED) {

        sem_wait(&renderSem);

        switch (gameState->state) {

            case MENU: //menu
                drawMenu(gameState);
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

            case GAMEOVERSCREEN:
                drawGameOverScreen(gameState);
                break;

            case RETRYSCREEN:
                drawRetryScreen(gameState);
                break;

            case PAUSE:
                drawPause(gameState);
                break;

            case NEXTLEVEL:
                drawNextLevelScreen(gameState);
                break;
        }
    }


    stopTimer(FPSTIMER);
    al_destroy_display(disp);
    pthread_exit(NULL);

}

int isInsideScreenX(fisica_t* object1){
    int insideX = 0;

    float cameraScroll = getCameraScrollX();
    if( (cameraScroll  < (object1->posx + object1->ancho)) && ((object1->posx - object1->ancho) < SCREENWIDHT + cameraScroll)){
        insideX = 1;
    }
    return insideX;
}

void updateCameraPosition(void* gs){

    static bloque_t* lastBlockInMapX = NULL;
    int offsetX = 15;
    estadoJuego_t* gameState = (estadoJuego_t*) gs;


    lastBlockInMapX = &gameState->entidades.bloques[0];
    for(int i = 1; gameState->entidades.bloques[i].identificador != NULLENTITIE; i++){
        if(gameState->entidades.bloques[i].fisica.posx + gameState->entidades.bloques[i].fisica.ancho > lastBlockInMapX->fisica.posx){
            lastBlockInMapX = &gameState->entidades.bloques[i];
        }
    }


    if (gameState->entidades.jugador.fisica.posx > (SCREENWIDHT/2 + scrollX) && (lastBlockInMapX->fisica.posx + lastBlockInMapX->fisica.ancho > scrollX + SCREENWIDHT + offsetX)) {
        scrollX = gameState->entidades.jugador.fisica.posx - SCREENWIDHT/2;
    }

}

#elif MODOJUEGO == 1

    void *render (void *gs) {

        estadoJuego_t *gameState = (estadoJuego_t *) gs;

        createNewTimer(1.0f/FPS, redraw, FPSTIMER);
        startTimer(FPSTIMER);

        while (gameState->state != GAMECLOSED) {

            sem_wait(&renderSem);

            switch (gameState->state) {

                case MENU: //menu
                    drawMenu(gameState);
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
                    gameState->gameUI.time = MAXLEVELTIME;
                    startTimer(INGAMETIMER);
                break;

                case PAUSE://EN PRINCIPIO NO HABRIA PAUSA PARA EL MODO RASPI, NO TENEMOS TECLA
                    drawPause(gameState);
                break;

                case NEXTLEVEL:
                    drawNextLevelScreen(gameState);
                break;
            }
        }

        stopTimer(FPSTIMER);
        pthread_exit(NULL);

    }

    void writeDisplay(char matriz[16][16]){

        dcoord_t myPoint = {};		//inicializa myPoint en (0,0). Recordemos que está arriba a la izquierda.
        int y,x;

        for (x = DISP_MIN; x <= DISP_MAX_Y; x++)	//para cada coordenada en y...
        {
            for ( y = DISP_MIN; y <= DISP_MAX_X ; y++)	//para cada coordenada en x...
            {
                myPoint.x = y;
                myPoint.y = x;
                if (matriz[x][y] == 1) {
                    disp_write(myPoint, D_ON);                //prende el LED en el buffer
                }
                else if (matriz[x][y] == 0){
                    disp_write(myPoint, D_OFF);                //prende el LED en el buffer
                }
            }
        }

        disp_update();
    }

int isInsideScreenX(fisica_t* object1){
    int insideX = 0;

    float cameraScroll = getCameraScrollX();
    if( (cameraScroll  < (object1->posx + object1->ancho)) && ( (object1->posx - object1->ancho) < ( SCREENWIDHT/2 + cameraScroll) )){
        insideX = 1;
    }
    return insideX;
}


void updateCameraPosition(void* gs){

    static bloque_t* lastBlockInMapX = NULL;
    static int actualLevel = 0;
    int offsetX = 15;
    estadoJuego_t* gameState = (estadoJuego_t*) gs;


    if(lastBlockInMapX == NULL || actualLevel != gameState->gameUI.level){
        actualLevel = gameState->gameUI.level;
        lastBlockInMapX = &gameState->entidades.bloques[0];
        for(int i = 1; gameState->entidades.bloques[i].identificador != NULLENTITIE; i++){
            if(gameState->entidades.bloques[i].fisica.posx + gameState->entidades.bloques[i].fisica.ancho > lastBlockInMapX->fisica.posx){
                lastBlockInMapX = &gameState->entidades.bloques[i];
            }
        }
    }

    if (gameState->entidades.jugador.fisica.posx > (SCREENWIDHT/4 + scrollX) && (lastBlockInMapX->fisica.posx + lastBlockInMapX->fisica.ancho > scrollX + SCREENWIDHT + offsetX)) {
        scrollX = gameState->entidades.jugador.fisica.posx - SCREENWIDHT/4;
    }

}

#endif



void setCameraScrollX(float coordX){
    scrollX = coordX;
}

float getCameraScrollX(){
    return scrollX;
}

static void redraw(void* gs){
    sem_post(&renderSem);
}