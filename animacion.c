//
// Created by alvaro on 2/12/20.
//

#include "animacion.h"
#include "matiasBrosGame.h"
#include "times.h"

#define MOD(x) ((x < 0) ? (-x) : (x))

enum playerAnim{SWIMMING1, SWIMMING2, SWIMMING3, SWIMMING4};
enum cheepcheepAnim{CHONE, CHTWO};

static void swimming(void* p1);

void * animar (void* gs){

    estadoJuego_t *gameState = (estadoJuego_t*) gs;

    while (gameState->state != GAMECLOSED) {

        while(gameState->state == PAUSE);

        /*
        if(timerAlreadyExist(PLAYERSWIMMINGANIM)) {
            if (MOD(gameState->entidades.jugador.fisica.velx) > 0) {
                stopTimer(PLAYERSWIMMINGANIM);
                setTimerSecondsPerTick(gameState->entidades.jugador.fisica.velx * 2, PLAYERSWIMMINGANIM);
                startTimer(PLAYERSWIMMINGANIM);
            }
            else {
                stopTimer(PLAYERSWIMMINGANIM);
            }
        }
        else{
            createNewTimer(MOD(gameState->entidades.jugador.fisica.velx) * 2, swimming, PLAYERSWIMMINGANIM);
        }

        if (gameState->entidades.jugador.estado == ALMOSTDEAD) {
            //hacer animacion de morir jugador
        }
        */
    }
}

static void swimming(void* p1){
    jugador_t* player = p1;
    if(player->sprite != SWIMMING4){
        player->sprite++;
    }
    else{
        player->sprite = SWIMMING1;
    }
}