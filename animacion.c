/***************************************************************************//**
  file     animacion.c
  @brief    Control de animaciones y timers correspondientes
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "animacion.h"
#include <unistd.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define MOD(x) ((x < 0) ? (-x) : (x))

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void animacion(void* gs);
static void swimming(void* gs);
static void movingSeaweed(void* gs);
static void movingCheepCheep(void* gs);
static void blinkingCoin(void* gs);
static void rotatePlayerAtDeath (void* gs);
static void blinkingMushroom(void* gs);
static void blinkingPipe(void* gs);

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static sem_t* animacionSem;
static sem_t* fisicaSem;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void * animar (void* gs){

    pthread_detach(pthread_self());

    fisicaSem = getPhysicsSem();
    animacionSem = getAnimeSem();

    estadoJuego_t *gameState = (estadoJuego_t*) gs;

    createNewTimer( 1.0f / FPS, animacion, ANIMETIMER);
    //Se crean timers para cada tipo de animación
    createNewTimer(1.0f, blinkingCoin, BLINKINGCOINANIM);
    createNewTimer(0.25f, movingCheepCheep, CHEEPCHEEPANIM);
    createNewTimer(2.0f, blinkingMushroom, MUSHROOMANIM);
    createNewTimer(0.1f, blinkingPipe, PIPEANIM);

    createNewTimer(0.04f, rotatePlayerAtDeath, DEATHANIM);
    createNewTimer(0.1f, swimming, PLAYERSWIMMINGANIM);
    createNewTimer(0.6f, movingSeaweed, SEAWEEDANIM);

    startInGameAnimations();

    startTimer(ANIMETIMER);

    sem_post(fisicaSem);
    while (gameState->state != GAMECLOSED) {

        sem_wait(animacionSem);
        while(gameState->state == PAUSE){
            usleep(300);
        }

        if (gameState->entidades.jugador.estado == ALMOSTDEAD) {
            stopTimer(PLAYERSWIMMINGANIM);
            gameState->entidades.jugador.sprite = 0;
            startTimer(DEATHANIM);
        }
        else{ // En caso de que el personaje se este desplazando a velocidad considerable en el eje x, se activa la animación de nadar
            if (MOD(gameState->entidades.jugador.fisica.velx) > 0.01) {
                startTimer(PLAYERSWIMMINGANIM);
            }
            else{
                stopTimer(PLAYERSWIMMINGANIM);
                gameState->entidades.jugador.sprite = 0;
            }
        }
        

    }

    return NULL;
}

void startInGameAnimations(){
    startTimer(SEAWEEDANIM);
    startTimer(BLINKINGCOINANIM);
    startTimer(CHEEPCHEEPANIM);
    startTimer(MUSHROOMANIM);
    startTimer(PIPEANIM);
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void animacion(void* gs){
    sem_post(animacionSem);
}

static void rotatePlayerAtDeath (void* gs) {

    static int animationCounter = 0;
    int rotationCounter = 80;
    estadoJuego_t* gameState = (estadoJuego_t*) gs;

    // Se va incrementando la posición angular del personaje
    gameState->entidades.jugador.angleRotation += 4.5 * 3.1416f / rotationCounter;

    if (animationCounter >= rotationCounter) {
        animationCounter = 0;
        gameState->entidades.jugador.angleRotation = 0.0;
        stopTimer(DEATHANIM);
    } else if (animationCounter == 0) {
        movePlayer(DOWNARRIBA, &gameState->entidades.jugador);
        animationCounter++;
    } else {
        animationCounter++;
    }


}

static void movingCheepCheep(void* gs){ //Control de animación de los CheepCheeps
    estadoJuego_t* gameState = (estadoJuego_t*) gs;

    if(wasLevelInitialized()) {
        for (int i = 0; gameState->entidades.enemigos[i].identificador != NULLENTITIE; i++) {
            if (gameState->entidades.enemigos[i].identificador == SLOWCHEEPCHEEP ||
                gameState->entidades.enemigos[i].identificador == FASTCHEEPCHEEP) {
                if (gameState->entidades.enemigos[i].sprite == 0) {
                    gameState->entidades.enemigos[i].sprite = 1;
                } else {
                    gameState->entidades.enemigos[i].sprite = 0;
                }
            }
        }
    }
}

static void movingSeaweed(void* gs){ //Control de animación de algas
    estadoJuego_t* gameState = (estadoJuego_t*) gs;

    if(wasLevelInitialized()) {
        for (int i = 0; gameState->entidades.bloques[i].identificador != NULLENTITIE;
        i++){
            if (gameState->entidades.bloques[i].identificador == ALGA) {
                if (gameState->entidades.bloques[i].sprite == 0) {
                    gameState->entidades.bloques[i].sprite = 1;
                } else {
                    gameState->entidades.bloques[i].sprite = 0;
                }
            }
        }
    }
}

static void blinkingCoin(void* gs){ //Control de animación de monedas
    estadoJuego_t* gameState = (estadoJuego_t*) gs;

    if(wasLevelInitialized()) {
        for (int i = 0; gameState->entidades.bloques[i].identificador != NULLENTITIE; i++) {
            if (gameState->entidades.bloques[i].identificador == MONEDA) {
                if (gameState->entidades.bloques[i].sprite == 0) {
                    gameState->entidades.bloques[i].sprite = 1;
                } else {
                    gameState->entidades.bloques[i].sprite = 0;
                }
            }
        }
    }
}

static void blinkingMushroom(void* gs){ //Control de animación de los hongos
    estadoJuego_t* gameState = (estadoJuego_t*) gs;

    if(wasLevelInitialized()) {
        for (int i = 0; gameState->entidades.bloques[i].identificador != NULLENTITIE; i++) {
            if (gameState->entidades.bloques[i].identificador == MUSHROOM) {
                if (gameState->entidades.bloques[i].sprite == 0) {
                    gameState->entidades.bloques[i].sprite = 1;
                } else {
                    gameState->entidades.bloques[i].sprite = 0;
                }
            }
        }
    }
}

static void blinkingPipe(void* gs){ //Control de animación de las tuberías
    estadoJuego_t* gameState = (estadoJuego_t*) gs;

    if(wasLevelInitialized()) {
        for (int i = 0; gameState->entidades.bloques[i].identificador != NULLENTITIE; i++) {
            if (gameState->entidades.bloques[i].identificador == TOPPIPE || gameState->entidades.bloques[i].identificador == MIDDLEPIPE) {
                if (gameState->entidades.bloques[i].sprite == 0) {
                    gameState->entidades.bloques[i].sprite = 1;
                } else {
                    gameState->entidades.bloques[i].sprite = 0;
                }
            }
        }
    }
}

static void swimming(void* gs) {
    estadoJuego_t *gameState = gs;

    // Se van alternando las imágenes del personaje
    if (gameState->entidades.jugador.sprite < 4) {
        gameState->entidades.jugador.sprite++;
    } else {
        gameState->entidades.jugador.sprite = 1;
    }
}
