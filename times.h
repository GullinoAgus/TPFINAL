//
// Created by gonzalo on 26/12/20.
//

#ifndef TPFINAL_TIMES_H
#define TPFINAL_TIMES_H

#include <pthread.h>
#include "matiasBrosGame.h"

enum {FPSTIMER, INGAMETIMER, DEATHTIMER, PLAYERSWIMMINGANIM, NUMOFDEFAULTTIMERS};

typedef struct{
    int ID;
    float secondsPerTick;
    int isRunning;
    int isPaused;
    pthread_t timer;
    void (*funtionToExecute)(void* param);
}eventTimer_t;

void setCurrentGameState(estadoJuego_t* gs);
int createNewTimer(float _secondsPerTick, void (*funct)(), int ID);
void setTimerSecondsPerTick(float newSecondsPerTick, int timerID);
void startTimer(int timerID);
void stopTimer(int timerID);
void stopAllTimers();
int timerAlreadyExist(int ID);

#endif //TPFINAL_TIMES_H
