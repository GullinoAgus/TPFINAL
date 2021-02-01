/***************************************************************************//**
  file      render.c
  @brief    Controla el refresco de pantalla y lo que se muestra en ella
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <pthread.h>
#include <semaphore.h>
#include "render.h"
#include <unistd.h>

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

static bloque_t* lastBlockInMapX = NULL;

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static float scrollX = 0.0f;
static sem_t* renderSem;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

#if MODOJUEGO == ALLEGRO
static void show1UPText(void* gs);
#endif

static void redraw(void* gs);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void *render (void *gs) { // Se encarga de refrescar la pantalla cada cierto tiempo indicado por timer

    renderSem = getRenderSem();
    estadoJuego_t *gameState = (estadoJuego_t *) gs;
    sem_init(renderSem, 0, 1);

#if MODOJUEGO == ALLEGRO
    // Creación de la pantalla del juego y el timer que rige los FPS
    ALLEGRO_DISPLAY* disp = al_create_display(SCREENWIDHT, SCREENHEIGHT);
    createNewTimer( 0.05f, show1UPText, LIFEUPANIM);
#elif MODOJUEGO == RASPI
    int lastGameState = gameState->state;
#endif

    createNewTimer(1.0f/(FPS), redraw, FPSTIMER);
    startTimer(FPSTIMER);

    while (gameState->state != GAMECLOSED) {

        sem_wait(renderSem);

#if MODOJUEGO == RASPI
        if(lastGameState != gameState->state) {
            lastGameState = gameState->state;
            disp_clear();
        }
#endif

        // Dependiendo del estado del juego se muestra en la pantalla la información correspondiente
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

            case GAMEOVERSCREEN: //fin de juego
                drawGameOverScreen(gameState);
                break;

            case RETRYSCREEN: //pérdida de 1 vida
                drawRetryScreen(gameState);
                break;

            case PAUSE: //pantalla de pausa
                drawPause(gameState);
                break;

            case NEXTLEVEL: //transición a próximo nivel
                drawNextLevelScreen(gameState);
                break;
            default:
                break;
        }
    }

    // Destrucción de los recursos al terminar la ejecución
    stopTimer(FPSTIMER);

#if MODOJUEGO == ALLEGRO
    al_destroy_display(disp);
#endif

    pthread_exit(NULL);

}

#if MODOJUEGO == ALLEGRO

int isInsideScreenX(fisica_t* object1){ // Determina si cierta entidad se encuentra o no en la pantalla (eje X)
    int insideX = 0;

    float cameraScroll = getCameraScrollX();
    if( (cameraScroll  < (object1->posx + (float)object1->ancho)) && ((object1->posx - (float)object1->ancho) < SCREENWIDHT + cameraScroll)){
        insideX = 1;
    }
    return insideX;
}

void updateCameraPosition(void* gs){ // Se actualiza la posición de la pantalla sobre el nivel

    int offsetX = 15;
    estadoJuego_t* gameState = (estadoJuego_t*) gs;

    if(lastBlockInMapX == NULL) { // Determina donde se encuentra el último bloque del nivel, para frenar el avance de la pantalla
        lastBlockInMapX = &gameState->entidades.bloques[0];
        for (int i = 1; gameState->entidades.bloques[i].identificador != NULLENTITIE; i++) {
            if (gameState->entidades.bloques[i].fisica.posx + (float)gameState->entidades.bloques[i].fisica.ancho >
                lastBlockInMapX->fisica.posx) {
                lastBlockInMapX = &gameState->entidades.bloques[i];
            }
        }
    }
    // Se determina si la pantalla tiene que avanzar por el nivel o no
    if (gameState->entidades.jugador.fisica.posx > ((float)SCREENWIDHT/2 + scrollX) && (lastBlockInMapX->fisica.posx + (float)lastBlockInMapX->fisica.ancho > scrollX + SCREENWIDHT + (float)offsetX)) {
        scrollX = gameState->entidades.jugador.fisica.posx - (float)SCREENWIDHT/2;
    }

}


static void show1UPText(void* gs){

    static int timescounter = 0;

    //al_map_rgb(76,25,153)

    if(timescounter <= 30){
        timescounter++;
    }
    else {
        timescounter = 0;
        stopTimer(LIFEUPANIM);
    }
}

#elif MODOJUEGO == RASPI

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

int isInsideScreenX(fisica_t* object1){ //Determina si cierta entidad se encuentra o no en la pantalla (eje X)
    int insideX = 0;

    float cameraScroll = getCameraScrollX();
    if( (cameraScroll  < (object1->posx + object1->ancho)) && ( (object1->posx - object1->ancho) < ( SCREENWIDHT/2 + cameraScroll) )){
        insideX = 1;
    }
    return insideX;
}

int isInsideScreenY(fisica_t* object1){ //Determina si cierta entidad se encuentra o no en la pantalla (eje Y)

    int insideY = 0;

    if( (0  < (object1->posy + object1->alto)) && ((object1->posy - object1->alto) < 640) ) {
        insideY = 1;
    }
    return insideY;

}

void updateCameraPosition(void* gs){ // Se actualiza la posición de la pantalla sobre el nivel

    static bloque_t* lastBlockInMapX = NULL;
    int offsetX = 15;
    estadoJuego_t* gameState = (estadoJuego_t*) gs;

    // Determina donde se encuentra el último bloque del nivel, para frenar el avance de la pantalla
    lastBlockInMapX = &gameState->entidades.bloques[0];
    for(int i = 1; gameState->entidades.bloques[i].identificador != NULLENTITIE; i++){
        if(gameState->entidades.bloques[i].fisica.posx + gameState->entidades.bloques[i].fisica.ancho > lastBlockInMapX->fisica.posx){
            lastBlockInMapX = &gameState->entidades.bloques[i];
        }
    }

    // Se determina si la pantalla tiene que avanzar por el nivel o no
    if (gameState->entidades.jugador.fisica.posx > (SCREENWIDHT/4 + scrollX) && (lastBlockInMapX->fisica.posx + lastBlockInMapX->fisica.ancho > scrollX + SCREENWIDHT/2 + offsetX)) {
        scrollX = gameState->entidades.jugador.fisica.posx - SCREENWIDHT/4;
    }

}

#endif



void setCameraScrollX(float coordX){ // Asigna el primer parámetro a la posición actual de la pantalla en el nivel
    scrollX = coordX;
}

float getCameraScrollX(){ // Devuelve la posición actual de la pantalla en el nivel
    return scrollX;
}

void resetLastBlockInMap(){ // Asigna NULL a la variable LastBlockInMap
    lastBlockInMapX = NULL;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void redraw(void* gs){ // Función que ejecuta el timer de render
    sem_post(renderSem);
}