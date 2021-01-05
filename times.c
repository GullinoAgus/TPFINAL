//
// Created by gonzalo on 26/12/20.
//

#include "times.h"

static eventTimer_t* timers;
static int eventTimerQuant = 0;
static estadoJuego_t* currentGameState;

static void* wait(void* timerPointer);

void setCurrentGameState(estadoJuego_t* gs){
    currentGameState = gs;
}

int createNewTimer(float _secondsPerTick, void (*funct)(void*), int ID){

    int salida = 0;

    eventTimerQuant++;

    for(int i = 0; i < eventTimerQuant-1; i++){
        stopTimer(i);
    }

    if(eventTimerQuant == 1){
        timers = malloc(sizeof(eventTimer_t));
    }
    else{
        realloc(timers, sizeof(eventTimer_t) * eventTimerQuant);
    }

    if(timers == NULL){
        printf("Error al reservar espacio para el timer");
        salida = -1;
    }
    else{

        for(int i = 0; i < eventTimerQuant-1; i++){
            startTimer(i);
        }

        timers[eventTimerQuant-1].ID = ID;
        timers[eventTimerQuant-1].isRunning = 1;
        timers[eventTimerQuant-1].secondsPerTick = _secondsPerTick;
        timers[eventTimerQuant-1].funtionToExecute = funct;
        timers[eventTimerQuant-1].isPaused = 1;
    }

    return salida;
}

static void* wait(void* timerPointer){
    eventTimer_t* actualTimer = timerPointer;

    do {
        while (!actualTimer->isPaused) {
            usleep(actualTimer->secondsPerTick * 1000000);
            actualTimer->funtionToExecute(currentGameState);
        }
    }while(actualTimer->isRunning);

    pthread_exit(NULL);
}

void startTimer(int timerID){
    int found = 0;
    for(int i = 0; i < eventTimerQuant && !found; i++){
        if(timers[i].ID == timerID){
            if(timers[i].isPaused) {
                timers[i].isPaused = 0;
                pthread_create(&timers[i].timer, NULL, wait, &timers[i]);
            }
            found = 1;
        }
    }
}

void stopTimer(int timerID){
    int found = 0;

    for(int i = 0; i < eventTimerQuant && !found; i++){
        if(timers[i].ID == timerID){
            if(timers[i].isPaused == 0) {
                timers[i].isPaused = 0;
            }
            found = 1;
        }
    }
}

void setTimerSecondsPerTick(float newSecondsPerTick, int timerID){
    int found = 0;
    for(int i = 0; i < eventTimerQuant && !found; i++){
        if(timers[i].ID == timerID){
            stopTimer(timerID);
            timers[i].secondsPerTick = newSecondsPerTick;
            startTimer(timerID);
            found = 1;
        }
    }
}

int timerAlreadyExist(int ID){
    int found = 0;
    for(int i = 0; i < eventTimerQuant && !found; i++){
        if(timers[i].ID == ID){
            found = 1;
        }
    }
    return found;
}

void stopAllTimers(){
    for(int i = 0; i < eventTimerQuant; i++){
        timers[i].isRunning = 0;
        timers[i].ID = NULLENTITIE;
        pthread_join(timers[i].timer, NULL);
    }
    eventTimerQuant = 0;
    free(timers);
}

