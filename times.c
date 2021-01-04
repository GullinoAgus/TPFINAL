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
        timers[eventTimerQuant-1].isRunning = 0;
        timers[eventTimerQuant-1].secondsPerTick = _secondsPerTick;
        timers[eventTimerQuant-1].funtionToExecute = funct;
    }

    return salida;
}

static void* wait(void* timerPointer){
    eventTimer_t* toLauch = timerPointer;

    while(toLauch->isRunning) {
        usleep(toLauch->secondsPerTick * 1000000);
        toLauch->funtionToExecute(currentGameState);
    }

    pthread_exit(NULL);
}


void startTimer(int timerID){
    int found = 0;
    for(int i = 0; i < eventTimerQuant && !found; i++){
        if(timers[i].ID == timerID){
            eventTimer_t* timerToStart = &timers[i];
            if(!timerToStart->isRunning) {
                timerToStart->isRunning = 1;
                pthread_create(&timerToStart->timer, NULL, wait, timerToStart);
            }
        }
    }
}

void stopTimer(int timerID){
    int found = 0;

    for(int i = 0; i < eventTimerQuant && !found; i++){
        if(timers[i].ID == timerID){
            if(timers[i].isRunning == 1) {
                timers[i].isRunning = 0;
                pthread_join(timers[i].timer, NULL);
            }
            found = 1;
        }
    }
}

void setTimerSecondsPerTick(float newSecondsPerTick, int timerID){
    int found = 0;
    for(int i = 0; i < eventTimerQuant && !found; i++){
        if(timers[i].ID == timerID){
            timers[i].secondsPerTick = newSecondsPerTick;
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
        pthread_join(timers[i].timer, NULL);
    }
    eventTimerQuant = 0;
    free(timers);
}

