/***************************************************************************//**
  file      times.c
  @brief    Libreria de timer
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "times.h"
#include <unistd.h>



/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void* wait(void* timerPointer);
static eventTimer_t* findTimer(int timerId);

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static eventTimer_t* timerList = NULL;
static estadoJuego_t* currentGameState;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void setCurrentGameState(void* gs){
    currentGameState = gs;      //Gamestate del juego
}

int createNewTimer(float _secondsPerTick, void (*funct)(void*), int ID){

    int salida = 0;
    eventTimer_t* pTimer;               //Puntero al ultimo timer de la lista
    eventTimer_t* pNewTimer = NULL;     //Puntero para guardar el nuevo timer

    pTimer = findTimer(ID);     //Buscamos si existe el timer con ese ID

    //Si la lista esta vacia o el id ingresado no existe en la lista
    if((pTimer == NULL) || (pTimer->ID != ID)) {

        pNewTimer = calloc(1 , sizeof(eventTimer_t));       //Reservamos espacio para el timer
        if(pNewTimer != NULL){

            //Inicializamos el nuevo reloj
            pNewTimer->ID = ID;
            pNewTimer->next = NULL;
            pNewTimer->isPaused = 1;
            pNewTimer->isRunning = 1;
            pNewTimer->funtionToExecute = funct;
            pNewTimer->secondsPerTick = _secondsPerTick;
            pthread_create(&pNewTimer->timer, NULL, wait, pNewTimer);

            if(pTimer == NULL)      //Si es el primero creado lo guardamos como el primero de la lista
            {
                timerList = pNewTimer;
            }
            else
            {
                pTimer->next = pNewTimer;       //sino como el ultimo de la lista
            }
        }
        else{
            printf("Error al reservar espacio para el timer ID: %d\n", ID);     //Si hubo un error devolvemos un -1
            salida = -1;
        }
    }


    return salida;
}

void startTimer(int timerID){
    eventTimer_t* pTimer;
    pTimer = timerList;

    //Busca el timer con el timerID
    while ((pTimer->next != NULL) && (pTimer->ID != timerID)) {
        pTimer = pTimer->next;
    }

    if (pTimer->ID == timerID) {
        pTimer->isPaused = 0;   //Pausa el timer si se encontro el timer
    }

}

void stopTimer(int timerID) {

    eventTimer_t *pTimer;
    pTimer = timerList;

    //Busca el timer con el timerID
    while ((pTimer->next != NULL) && (pTimer->ID != timerID)) {
        pTimer = pTimer->next;
    }

    if(pTimer->ID == timerID) {
        pTimer->isPaused = 1; //Despausea el timer si se lo encontro
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

    //Busca el timer con el timerID
    while((pTimer->next != NULL) && (pTimer->ID != timerID) ){
        pTimer = pTimer->next;
    }

    if(pTimer->ID == timerID) {
        stateValue = pTimer->isPaused;  //Si encontro el timer, devuelve su estado de pausa
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

    //Recorre cada timer y los pausea y lo cierra
    while(current != NULL){
        current->isPaused = 1;
        current->isRunning = 0;
        current->ID = NULLENTITIE;
        next = current->next;
        current = next;
    }
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static eventTimer_t* findTimer(int timerId){

    eventTimer_t *pTimer;

    pTimer = timerList;

    //Buscamos el timer con el ID pasado por parametro
    if(pTimer != NULL) {
        while ((pTimer->next != NULL) && (pTimer->ID != timerId)) { //Mientras no se haya llegado al final de la lista y no se haya encontrado el timer con el ID buscado, seguimos
            pTimer = pTimer->next;
        }
    }

    return pTimer;
}

static void* wait(void* timerPointer){

    pthread_detach(pthread_self());

    eventTimer_t* actualTimer = timerPointer;

    do {
        while (!actualTimer->isPaused) {                            //Mientras el timer no este pausado
            usleep(actualTimer->secondsPerTick * 1000000);  //Hacemos el delay indicado
            actualTimer->funtionToExecute(currentGameState);        //Lanzamos la funcion asociada
        }
        usleep(300);                                        //Hacemos un pequeño delay para liberar el CPU
    }while(actualTimer->isRunning);                                 //Corremos el timer hasta que su variable corriendo sea falsa

    free(actualTimer);                                              //Liberamos el espacio del timer
    pthread_exit(NULL);
}
