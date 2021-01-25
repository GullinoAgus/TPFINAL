//
// Created by alvaro on 2/12/20.
//

#include "animacion.h"
#include "times.h"
#include "gamelogic.h"
#include "IEvents.h"

#define MOD(x) ((x < 0) ? (-x) : (x))

static void swimming(void* gs);
static void movingSeaweed(void* gs);
static void movingCheepCheep(void* gs);
static void blinkingCoin(void* gs);
static void rotatePlayerAtDeath (void* gs);
static void blinkingMushroom(void* gs);
static void blinkingPipe(void* gs);


void * animar (void* gs){

    pthread_detach(pthread_self());

    estadoJuego_t *gameState = (estadoJuego_t*) gs;


    createNewTimer(1.0f, blinkingCoin, BLINKINGCOINANIM);
    createNewTimer(0.25f, movingCheepCheep, CHEEPCHEEPANIM);
    createNewTimer(2.0f, blinkingMushroom, MUSHROOMANIM);
    createNewTimer(0.1f, blinkingPipe, PIPEANIM);

    createNewTimer(0.05f, rotatePlayerAtDeath, DEATHANIM);
    createNewTimer(0.1f, swimming, PLAYERSWIMMINGANIM);
    createNewTimer(0.6f, movingSeaweed, SEAWEEDANIM);

    startInGameAnimations();

    while (gameState->state != GAMECLOSED) {

        while(gameState->state == PAUSE);


        if (gameState->entidades.jugador.estado == ALMOSTDEAD) {
            stopTimer(PLAYERSWIMMINGANIM);
            gameState->entidades.jugador.sprite = 0;
            startTimer(DEATHANIM);
        }
        else{
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

void stopInGameAnimations(){
    stopTimer(SEAWEEDANIM);
    stopTimer(BLINKINGCOINANIM);
    stopTimer(PLAYERSWIMMINGANIM);
    stopTimer(CHEEPCHEEPANIM);
    stopTimer(DEATHANIM);
    stopTimer(LIFEUPANIM);
    stopTimer(MUSHROOMANIM);
    stopTimer(PIPEANIM);
}


static void rotatePlayerAtDeath (void* gs) {

    static int animationCounter = 0;
    int rotationCounter = 50;
    estadoJuego_t* gameState = (estadoJuego_t*) gs;

    gameState->entidades.jugador.angleRotation += 4.5 * 3.1416f / rotationCounter;

    if(animationCounter >= rotationCounter){
        gameState->entidades.jugador.angleRotation = 0;
        animationCounter = 0;
        stopTimer(DEATHANIM);
    } else if (animationCounter == 0){
        movePlayer(DOWNARRIBA, &gameState->entidades.jugador);
        animationCounter++;
    }
    else{
        animationCounter++;
    }
}

static void movingCheepCheep(void* gs){
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

static void movingSeaweed(void* gs){
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

static void blinkingCoin(void* gs){
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

static void blinkingMushroom(void* gs){
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

static void blinkingPipe(void* gs){
    estadoJuego_t* gameState = (estadoJuego_t*) gs;

    if(wasLevelInitialized()) {
        for (int i = 0; gameState->entidades.bloques[i].identificador != NULLENTITIE; i++) {
            if (gameState->entidades.bloques[i].identificador == PIPETOPSPRITE || gameState->entidades.bloques[i].identificador == PIPEMIDDLESPRITE) {
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

    if (gameState->entidades.jugador.sprite < 4) {
        (gameState->entidades.jugador.sprite)++;
    } else {
        gameState->entidades.jugador.sprite = 1;
    }
}
