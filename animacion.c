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
static void incrementarTiempo (void* gs);

void * animar (void* gs){

    estadoJuego_t *gameState = (estadoJuego_t*) gs;
    gameState->entidades.jugador.animation_counter = -1;

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

        */

        if (gameState->entidades.jugador.estado == ALMOSTDEAD) {
            if (gameState->entidades.jugador.animation_counter == -1) {
                gameState->entidades.jugador.angleRotation = 0;
                (gameState->entidades.jugador.animation_counter)++;
            }

            if(!timerAlreadyExist(DEATHTIMER)) {
                if ((createNewTimer(0.01f, incrementarTiempo, DEATHTIMER)) != -1) {
                    startTimer(DEATHTIMER);
                }
            }
            else{
                if (gameState->entidades.jugador.animation_counter >= 100) {
                    stopTimer(DEATHTIMER);
                    gameState->entidades.jugador.estado = DEAD;
                    gameState->entidades.jugador.animation_counter = -1;
                    gameState->entidades.jugador.angleRotation = 0;
                }
            }
        }

    }
}

static void incrementarTiempo (void* gs) {
    estadoJuego_t* gameState = (estadoJuego_t*) gs;

    (gameState->entidades.jugador.angleRotation) += 4.5 * 3.1416f / 100;
    (gameState->entidades.jugador.animation_counter)++;
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