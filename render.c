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

static bloque_t* lastBlockInMapX = NULL;
static float scrollX = 0.0f;

static void redraw(void* gs);
static void show1UPText(void* gs);

//Si el juego debe renderizarse en la pantalla de la computadora
#if MODOJUEGO == 0

void *render (void *gs) {

    ALLEGRO_DISPLAY* disp;
    estadoJuego_t *gameState = (estadoJuego_t *) gs;
    sem_init(&renderSem, 0, 1);

    disp = al_create_display(SCREENWIDHT, SCREENHEIGHT);

    createNewTimer(1.0f/(FPS), redraw, FPSTIMER);
    createNewTimer( 0.05f, show1UPText, ONEUPANIM);
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
    if( (cameraScroll  < (object1->posx + (float)object1->ancho)) && ((object1->posx - (float)object1->ancho) < SCREENWIDHT + cameraScroll)){
        insideX = 1;
    }
    return insideX;
}

void updateCameraPosition(void* gs){

    int offsetX = 15;
    estadoJuego_t* gameState = (estadoJuego_t*) gs;

    if(lastBlockInMapX == NULL) {
        lastBlockInMapX = &gameState->entidades.bloques[0];
        for (int i = 1; gameState->entidades.bloques[i].identificador != NULLENTITIE; i++) {
            if (gameState->entidades.bloques[i].fisica.posx + (float)gameState->entidades.bloques[i].fisica.ancho >
                lastBlockInMapX->fisica.posx) {
                lastBlockInMapX = &gameState->entidades.bloques[i];
            }
        }
    }

    if (gameState->entidades.jugador.fisica.posx > ((float)SCREENWIDHT/2 + scrollX) && (lastBlockInMapX->fisica.posx + (float)lastBlockInMapX->fisica.ancho > scrollX + SCREENWIDHT + (float)offsetX)) {
        scrollX = gameState->entidades.jugador.fisica.posx - (float)SCREENWIDHT/2;
    }

}


static void show1UPText(void* gs){

    estadoJuego_t* gameState = (estadoJuego_t* ) gs;
    static int timescounter = 0;

    //al_map_rgb(76,25,153)

    if(timescounter <= 30){
        //al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], al_map_rgb(255,255,255), gameState->entidades.jugador.fisica.posx, gameState->entidades.jugador.fisica.posy - 20, 0, "up");
        timescounter++;
    }
    else {
        timescounter = 0;
        stopTimer(ONEUPANIM);
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

                case PAUSE:                 //EN PRINCIPIO NO HABRIA PAUSA PARA EL MODO RASPI, NO TENEMOS NINGUNA TECLA DESIGNADA
                    drawPause(gameState);
                break;

                case NEXTLEVEL:
                    drawNextLevelScreen(gameState);
                break;

                case GAMEOVERSCREEN:
                    drawGameOverScreen(gameState);
                    sleep(2);
                    if (wasNewHighScoreAchieved(gameState)){
                        drawGameOverScreenHighScore(gameState);
                        sleep(2);
                    }
                    else{
                        sleep(2);
                    }
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

int isInsideScreenY(fisica_t* object1){

    int insideY = 0;

    if( (0  < (object1->posy + object1->alto)) && ((object1->posy - object1->alto) < 640) ) {
        insideY = 1;
    }
    return insideY;

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

    if (gameState->entidades.jugador.fisica.posx > (SCREENWIDHT/4 + scrollX) && (lastBlockInMapX->fisica.posx + lastBlockInMapX->fisica.ancho > scrollX + SCREENWIDHT/2 + offsetX)) {
        scrollX = gameState->entidades.jugador.fisica.posx - SCREENWIDHT/4;
    }

}

#endif


static void redraw(void* gs){
    sem_post(&renderSem);
}

void setCameraScrollX(float coordX){
    scrollX = coordX;
}

float getCameraScrollX(){
    return scrollX;
}

void resetLastBlockInMap(){
    lastBlockInMapX = NULL;
}