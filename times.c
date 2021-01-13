//
// Created by gonzalo on 26/12/20.
//

#include "times.h"

static eventTimer_t* timerList = NULL;
static estadoJuego_t* currentGameState;

static void* wait(void* timerPointer);
static eventTimer_t* findTimer(int timerId);

void setCurrentGameState(estadoJuego_t* gs){
    currentGameState = gs;
}

int createNewTimer(float _secondsPerTick, void (*funct)(void*), int ID){

    int salida = 0;
    eventTimer_t* pTimer;
    eventTimer_t* pNewTimer = NULL;

    pTimer = findTimer(ID);

    //Si la lista esta vacia o el id ingresado no existe en la lista
    if((pTimer == NULL) || (pTimer->ID != ID)) {

        pNewTimer = calloc(1 , sizeof(eventTimer_t));
        if(pNewTimer != NULL){

            //Inicializamos el nuevo reloj
            pNewTimer->ID = ID;
            pNewTimer->next = NULL;
            pNewTimer->isPaused = 1;
            pNewTimer->isRunning = 1;
            pNewTimer->funtionToExecute = funct;
            pNewTimer->secondsPerTick = _secondsPerTick;
            pthread_create(&pNewTimer->timer, NULL, wait, pNewTimer);

            if(pTimer == NULL)
            {
                timerList = pNewTimer;
            }
            else
            {
                pTimer->next = pNewTimer;
            }
        }
        else{
            printf("Error al reservar espacio para el timer ID: %d\n", ID);
            salida = -1;
        }
    }


    return salida;
}

static eventTimer_t* findTimer(int timerId){

    eventTimer_t *pTimer;

    pTimer = timerList;

    if(pTimer != NULL) {
        while ((pTimer->next != NULL) && (pTimer->ID != timerId)) {
            pTimer = pTimer->next;
        }
    }

    return pTimer;
}

static void* wait(void* timerPointer){
    eventTimer_t* actualTimer = timerPointer;

    do {
        while (!actualTimer->isPaused) {
            usleep(actualTimer->secondsPerTick * 1000000);
            actualTimer->funtionToExecute(currentGameState);
        }
    }while(actualTimer->isRunning);
    free(actualTimer);
    pthread_exit(NULL);
}

void startTimer(int timerID){
    eventTimer_t* pTimer;
    pTimer = timerList;

    while((pTimer->next != NULL) && (pTimer->ID != timerID) ){
        pTimer = pTimer->next;
    }

    if(pTimer->ID == timerID) {
        pTimer->isPaused = 0;
    }

}

void stopTimer(int timerID){

    eventTimer_t* pTimer;
    pTimer = timerList;

    while((pTimer->next != NULL) && (pTimer->ID != timerID) ){
        pTimer = pTimer->next;
    }

    if(pTimer->ID == timerID) {
        pTimer->isPaused = 1;
    }
}

void setTimerSecondsPerTick(float newSecondsPerTick, int timerID){
    eventTimer_t* pTimer;
    pTimer = timerList;

    while((pTimer->next != NULL) && (pTimer->ID != timerID) ){
        pTimer = pTimer->next;
    }

    if(pTimer->ID == timerID) {
        stopTimer(timerID);
        pTimer->secondsPerTick = newSecondsPerTick;
        startTimer(timerID);
    }
}

int isPaused(int timerID){
    eventTimer_t* pTimer;
    pTimer = timerList;
    int stateValue = 0;

    while((pTimer->next != NULL) && (pTimer->ID != timerID) ){
        pTimer = pTimer->next;
    }

    if(pTimer->ID == timerID) {
        stateValue = pTimer->isPaused;
    }
    return stateValue;
}

void destroyTimer(int timerID){

    eventTimer_t* pTimer, *pNextTimer, *pPreviusTimer;
    pTimer = timerList;

    while((pTimer->next != NULL) && (pTimer->ID != timerID) ){
        pTimer = pTimer->next;
    }

    if(pTimer->ID == timerID){

        pNextTimer = pTimer->next;
        pPreviusTimer = findTimer(timerID-1);
        pPreviusTimer->next = pNextTimer;

        pTimer->isPaused = 1;
        pTimer->isRunning = 0;
        pTimer->ID = NULLENTITIE;
    }

}

void destroyAllTimers(){
    eventTimer_t* next;
    eventTimer_t* current = timerList;

    while(current != NULL){
        current->isPaused = 1;
        current->isRunning = 0;
        current->ID = NULLENTITIE;
        next = current->next;
        current = next;
    }
}

