//
// Created by alvaro on 2/12/20.
//

#include "animacion.h"
#include "times.h"
#include "gamelogic.h"

#define MOD(x) ((x < 0) ? (-x) : (x))

static void swimming(void* gs);
static void movingSeaweed(void* gs);
static void movingCheepCheep(void* gs);
static void blinkingCoin(void* gs);
static void rotatePlayerAtDie (void* gs);

#if MODOJUEGO == 0

void * animar (void* gs){

    estadoJuego_t *gameState = (estadoJuego_t*) gs;

    createNewTimer(0.1f, swimming, PLAYERSWIMMINGANIM);
    createNewTimer(0.6f, movingSeaweed, SEAWEEDANIM);
    createNewTimer(0.4f, blinkingCoin, BLINKINGCOINANIM);
    createNewTimer(0.5f, movingCheepCheep, CHEEPCHEEPANIM);
    createNewTimer(0.005f, rotatePlayerAtDie, DEATHANIM);

    startInGameAnimations();

    while (gameState->state != GAMECLOSED) {

        while(gameState->state == PAUSE);


        if (MOD(gameState->entidades.jugador.fisica.velx) > 0.01) {
            startTimer(PLAYERSWIMMINGANIM);
        }
        else{
            pauseTimer(PLAYERSWIMMINGANIM);
            gameState->entidades.jugador.sprite = 0;
        }


        /*
        if (gameState->entidades.jugador.estado == ALMOSTDEAD) {
            startTimer(DEATHANIM);
        }
         */

    }

}

void startInGameAnimations(){
    startTimer(SEAWEEDANIM);
    startTimer(BLINKINGCOINANIM);
    startTimer(CHEEPCHEEPANIM);
}

void stopInGameAnimations(){
    pauseTimer(SEAWEEDANIM);
    pauseTimer(BLINKINGCOINANIM);
    pauseTimer(PLAYERSWIMMINGANIM);
    pauseTimer(CHEEPCHEEPANIM);
    pauseTimer(DEATHANIM);
    pauseTimer(LIFEUPANIM);
}


static void rotatePlayerAtDie (void* gs) {
    static int animationCounter = 0;
    estadoJuego_t* gameState = (estadoJuego_t*) gs;

    gameState->entidades.jugador.angleRotation += 4.5 * 3.1416f / 200;
    gameState->entidades.jugador.sprite = 0;
    if(animationCounter >= 200){
        gameState->entidades.jugador.estado = DEAD;
        pauseTimer(DEATHANIM);
        gameState->entidades.jugador.angleRotation = 0;
        animationCounter = 0;
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

#elif MODOJUEGO == 1

void * animar (void* gs){

    pthread_exit(NULL);

}

#endif