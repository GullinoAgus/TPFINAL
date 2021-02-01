/***************************************************************************//**
  @file     times.h
  @brief    Libreria de timer.
 ******************************************************************************/

#ifndef TPFINAL_TIMES_H
#define TPFINAL_TIMES_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <pthread.h>
#include "matiasBrosGame.h"

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

enum {FPSTIMER, INGAMETIMER, PHYSICSTIMER, ANIMETIMER, DOVULNERABLETIMER, DEATHANIM, PLAYERSWIMMINGANIM, SEAWEEDANIM, BLINKINGCOINANIM, CHEEPCHEEPANIM, LIFEUPANIM, MUSHROOMANIM, PIPEANIM,NUMOFDEFAULTTIMERS};

typedef struct timer{
    int ID;
    float secondsPerTick;
    int isRunning;
    int isPaused;
    pthread_t timer;
    void (*funtionToExecute)(void* param);
    struct timer* next;
}eventTimer_t;

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief La funcion recibe el puntero al gameState del juego para que lo usen los enemigos
 * @param Puntero al estadoJuego del juego
*/
void setCurrentGameState(void* gs);

/**
 * @brief Reserva espacio para una nueva estructura eventTimer_t que dispara cada _secondsPerTick la funcion funct y se guarda con el ID ingresado.
 *        En caso de que el ID ya exista, no hace nada.
 * @param _secondsPerTick cantidad de segundos que deben transcurrir para que el reloj dispare la funcion pasada por parametro
 * @param funct puntero a la funcion que se disparara
 * @param ID identificador unico para acceder al reloj
 * @return Devuelve un 0 si se pudo crear el reloj exitosamente, sino devuelve un -1
*/
int createNewTimer(float _secondsPerTick, void (*funct)(void*), int ID);

/**
 * @brief TODO: completar descripcion
 * @param param1 Descripcion parametro 1
 * @param param2 Descripcion parametro 2
*/
void setTimerSecondsPerTick(float newSecondsPerTick, int timerID);

/**
 * @brief Comienza a correr el timer con el ID que se le paso por parametro. Si el timer no existe, no hace nada.
 * @param timerID identificador del timer que se quiere lanzar
*/
void startTimer(int timerID);

/**
 * @brief Para de correr el timer con el ID que se le paso por parametro. Si el timer no existe, no hace nada.
 * @param timerID identificador del timer que se quiere parar.
*/
void stopTimer(int timerID);

/**
 * @brief La funcion indica el estado de pausa del reloj con ID timerID
 * @param timerID identificador del timer que se quiere evaluar.
 * @return Devuelve 0 si el reloj NO esta pausado o no existe el reloj, 1 si esta pausado
*/
int isPaused(int timerID);

/**
 * @brief TODO: completar descripcion
 * @param param1 Descripcion parametro 1
 * @param param2 Descripcion parametro 2
 * @return Descripcion valor que devuelve
*/
void destroyTimer(int timerID);

/**
 * @brief Libera la memoria de todos los timers que se crearon
*/
void destroyAllTimers();

#endif //TPFINAL_TIMES_H
