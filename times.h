/***************************************************************************//**
  @file     times.h
  @brief    Libreria de timer
 ******************************************************************************/

#ifndef TPFINAL_TIMES_H
#define TPFINAL_TIMES_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "matiasBrosGame.h"
#include "animacion.h" //sacas esto y te tira errores en fisica.c y render.c wtf

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

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
 * @brief TODO: completar descripcion
 * @param param1 Descripcion parametro 1
 * @param param2 Descripcion parametro 2
 * @return Descripcion valor que devuelve
*/
void setCurrentGameState(estadoJuego_t* gs);

/**
 * @brief TODO: completar descripcion
 * @param param1 Descripcion parametro 1
 * @param param2 Descripcion parametro 2
 * @return Descripcion valor que devuelve
*/
int createNewTimer(float _secondsPerTick, void (*funct)(void*), int ID);

/**
 * @brief TODO: completar descripcion
 * @param param1 Descripcion parametro 1
 * @param param2 Descripcion parametro 2
 * @return Descripcion valor que devuelve
*/
void setTimerSecondsPerTick(float newSecondsPerTick, int timerID);

/**
 * @brief TODO: completar descripcion
 * @param param1 Descripcion parametro 1
 * @param param2 Descripcion parametro 2
 * @return Descripcion valor que devuelve
*/
void startTimer(int timerID);

/**
 * @brief TODO: completar descripcion
 * @param param1 Descripcion parametro 1
 * @param param2 Descripcion parametro 2
 * @return Descripcion valor que devuelve
*/
void stopTimer(int timerID);

/**
 * @brief TODO: completar descripcion
 * @param param1 Descripcion parametro 1
 * @param param2 Descripcion parametro 2
 * @return Descripcion valor que devuelve
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
 * @brief TODO: completar descripcion
 * @param param1 Descripcion parametro 1
 * @param param2 Descripcion parametro 2
 * @return Descripcion valor que devuelve
*/
void destroyAllTimers();

/**
 * @brief TODO: completar descripcion
 * @param param1 Descripcion parametro 1
 * @param param2 Descripcion parametro 2
 * @return Descripcion valor que devuelve
*/
int timerAlreadyExist(int ID);

#endif //TPFINAL_TIMES_H
