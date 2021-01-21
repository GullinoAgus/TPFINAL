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

static char mapLevel[16][32];

static bloque_t* lastBlockInMapX = NULL;
static float scrollX = 0.0f;
static void testGraphicsAsRaspi(estadoJuego_t* gameState);

static void redraw(void* gs);

//Si el juego debe renderizarse en la pantalla de la computadora
#if MODOJUEGO == 0

void *render (void *gs) {

    ALLEGRO_DISPLAY* disp;
    estadoJuego_t *gameState = (estadoJuego_t *) gs;
    sem_init(&renderSem, 0, 1);

    disp = al_create_display(SCREENWIDHT, SCREENHEIGHT);

    createNewTimer(1.0f/(FPS), redraw, FPSTIMER);
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
                    //testGraphicsAsRaspi(gameState);
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

    int offsetX = 15;
    estadoJuego_t* gameState = (estadoJuego_t*) gs;

    if(lastBlockInMapX == NULL) {
        lastBlockInMapX = &gameState->entidades.bloques[0];
        for (int i = 1; gameState->entidades.bloques[i].identificador != NULLENTITIE; i++) {
            if (gameState->entidades.bloques[i].fisica.posx + gameState->entidades.bloques[i].fisica.ancho >
                lastBlockInMapX->fisica.posx) {
                lastBlockInMapX = &gameState->entidades.bloques[i];
            }
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

static void testGraphicsAsRaspi(estadoJuego_t* gameState){

    int i;

    for(i = 0; i < 16; i++){
        for(int j = 0; j < 32; j++){
            mapLevel[i][j] = 0;
        }
    }

    updateCameraPosition(gameState);

    float cameraScrollX = getCameraScrollX();
    int posX = 0, posY = 0, actualSprite;


    //Mientras no se haya llegado al final de la lista
    i = 0;
    while (gameState->entidades.enemigos[i].identificador != NULLENTITIE) {

        //Si el enemigo esta dentro de la pantalla
        if (isInsideScreenX(&gameState->entidades.enemigos[i].fisica)) {

            //Si es alguno de estos dos enemigos, leemos su sprite para hacerlo parpadear
            if (gameState->entidades.enemigos[i].identificador == FASTCHEEPCHEEP ||
                gameState->entidades.enemigos[i].identificador == SLOWCHEEPCHEEP) {
                actualSprite = gameState->entidades.enemigos[i].sprite;
            } else {
                actualSprite = 0;
            }

            //Calculamos la posicion inicial del enemigo
            posX = ((int) (gameState->entidades.enemigos[i].fisica.posx - cameraScrollX)) / PIXELSPERUNIT;
            posY = ((int) gameState->entidades.enemigos[i].fisica.posy) / PIXELSPERUNIT;

            for (int j = 0; j < ((int) (gameState->entidades.enemigos[i].fisica.alto / PIXELSPERUNIT)); j++) {
                if((posX < 16) && ((posY+j) < 16)) {
                    mapLevel[posY + j][posX] = 1 - actualSprite;        //Mientras no se haya recorrido el objeto en toda su altura, dibujamos un punto mas segun el sprite
                    printf("posX: %d - posY: %d\n", posX, posY+j);
                }
            }
        }

        i++;
    }


    //Mientras no se haya llegado al final de la lista
    i = 0;
    while (gameState->entidades.bloques[i].identificador != NULLENTITIE) {

        //Si el bloque esta dentro de la pantalla
        if (isInsideScreenX(&gameState->entidades.bloques[i].fisica)) {

            //Si es una moneda, leemos su sprite para hacerla parpadear
            if (gameState->entidades.bloques[i].identificador == MONEDA) {
                actualSprite = gameState->entidades.bloques[i].sprite;
            } else {
                actualSprite = 0;
            }

            posX = ((int) (gameState->entidades.bloques[i].fisica.posx - cameraScrollX)) / PIXELSPERUNIT;
            posY = ((int) gameState->entidades.bloques[i].fisica.posy) / PIXELSPERUNIT;

            for (int j = 0; j < ((int) (gameState->entidades.bloques[i].fisica.alto)) / PIXELSPERUNIT; j++) {

                if((posX < 16) && ((posY+j) < 16)) {
                    mapLevel[posY + j][posX] = 1 - actualSprite;

                    printf("posX: %d - posY: %d\n", posX, posY+j);
                }
            }

            for (int j = 0; j < ((int) (gameState->entidades.bloques[i].fisica.ancho)) / PIXELSPERUNIT; j++) {

                if(((posX+j) < 16) && (posY < 16)) {
                    mapLevel[posY][posX + j] = 1 - actualSprite;

                    printf("posX: %d - posY: %d\n", posX, posY+j);
                }
            }
        }

        i++;
    }

    posX = (int) ((gameState->entidades.jugador.fisica.posx - cameraScrollX) / PIXELSPERUNIT);
    posY = (int) (gameState->entidades.jugador.fisica.posy / PIXELSPERUNIT);
    mapLevel[posY][posX] = 1;

    /*
    for (int j = 0; j < SCREENHEIGHT/PIXELSPERUNIT; j++) {
        for (int k = 0; k < SCREENWIDHT/PIXELSPERUNIT; k++) {
            printf("%d", mapLevel[j][k]);
        }
        printf("\n");
    }
    printf("\n");*/
}