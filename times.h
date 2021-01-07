//
// Created by gonzalo on 26/12/20.
//

#ifndef TPFINAL_TIMES_H
#define TPFINAL_TIMES_H

#include <pthread.h>
#include "matiasBrosGame.h"
#include "animacion.h"

typedef struct timer{
    int ID;
    float secondsPerTick;
    int isRunning;
    int isPaused;
    pthread_t timer;
    void (*funtionToExecute)(void* param);
    struct timer* next;
}eventTimer_t;

void setCurrentGameState(estadoJuego_t* gs);
int createNewTimer(float _secondsPerTick, void (*funct)(void*), int ID);
void setTimerSecondsPerTick(float newSecondsPerTick, int timerID);
void startTimer(int timerID);
void stopTimer(int timerID);
int isPaused(int timerID);
void destroyTimer(int timerID);
void destroyAllTimers();
int timerAlreadyExist(int ID);

#endif //TPFINAL_TIMES_H
