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


void * animar (void* gs){

    pthread_detach(pthread_self());

    estadoJuego_t *gameState = (estadoJuego_t*) gs;

    createNewTimer(0.1f, swimming, PLAYERSWIMMINGANIM);
    createNewTimer(0.6f, movingSeaweed, SEAWEEDANIM);
    createNewTimer(1.0f, blinkingCoin, BLINKINGCOINANIM);
    createNewTimer(0.25f, movingCheepCheep, CHEEPCHEEPANIM);
    createNewTimer(0.05f, rotatePlayerAtDeath, DEATHANIM);

    startInGameAnimations();

    while (gameState->state != GAMECLOSED) {

        while(gameState->state == PAUSE);


        if (MOD(gameState->entidades.jugador.fisica.velx) > 0.01) {
            startTimer(PLAYERSWIMMINGANIM);
        }
        else{
            stopTimer(PLAYERSWIMMINGANIM);
            gameState->entidades.jugador.sprite = 0;
        }


        
        if (gameState->entidades.jugador.estado == ALMOSTDEAD) {
            startTimer(DEATHANIM);
        }
        

    }
    return NULL;
}

void startInGameAnimations(){
    startTimer(SEAWEEDANIM);
    startTimer(BLINKINGCOINANIM);
    startTimer(CHEEPCHEEPANIM);
}

void stopInGameAnimations(){
    stopTimer(SEAWEEDANIM);
    stopTimer(BLINKINGCOINANIM);
    stopTimer(PLAYERSWIMMINGANIM);
    stopTimer(CHEEPCHEEPANIM);
    stopTimer(DEATHANIM);
    stopTimer(LIFEUPANIM);
}


static void rotatePlayerAtDeath (void* gs) {
    static int animationCounter = 0;
    estadoJuego_t* gameState = (estadoJuego_t*) gs;

    gameState->entidades.jugador.angleRotation += 4.5 * 3.1416f / 50;
    gameState->entidades.jugador.sprite = 0;
    if(animationCounter >= 50){
        gameState->entidades.jugador.estado = DEAD;
        stopTimer(DEATHANIM);
        gameState->entidades.jugador.angleRotation = 0;
        animationCounter = 0;
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

static void swimming(void* gs) {
    estadoJuego_t *gameState = gs;

    if (gameState->entidades.jugador.sprite < 4) {
        (gameState->entidades.jugador.sprite)++;
    } else {
        gameState->entidades.jugador.sprite = 1;
    }
}
