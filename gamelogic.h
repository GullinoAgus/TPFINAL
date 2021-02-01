/***************************************************************************//**
  @file     gamelogic.h
  @brief    Uno de los threads principales donde se maneja la logica del videojuego.
 ******************************************************************************/

#ifndef TPFINAL_GAMELOGIC_H
#define TPFINAL_GAMELOGIC_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "menu.h"
#include "animacion.h"
#include "level.h"

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Thread principal donde se maneja la logica del videojuego
 * @param p2GameState puntero al estado del juego
*/
void *gamelogic (void *p2GameState);

/**
 * @brief Checkea si el nivel esta inicializado
 * @return Devuelve 1 si esta inicializado y 0 en el caso contrario
*/
char wasLevelInitialized();

/**
 * @brief Getter para semaforo para fisicas
 * @return Devuelve un puntero al semaforo para el thread de fisicas
*/
sem_t* getPhysicsSem();

/**
 * @brief Getter para semaforo para animaciones
 * @return Devuelve un puntero al semaforo para el thread de animaciones
*/
sem_t* getAnimeSem();

/**
 * @brief Getter para semaforo para render
 * @return Devuelve un puntero al semaforo para el thread de render
*/
sem_t* getRenderSem();

#endif //TPFINAL_GAMELOGIC_H
