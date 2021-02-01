/***************************************************************************//**
  @file     animacion.h
  @brief    Control de animaciones y timers correspondientes.
 ******************************************************************************/

#ifndef TPFINAL_ANIMACION_H
#define TPFINAL_ANIMACION_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "matiasBrosGame.h"
#include "times.h"
#include "fisica.h"
#include <semaphore.h>
#include <stdlib.h>

#if MODOJUEGO == 0

#include "allegro.h"

#endif //MODOJUEGO == 0


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Se encarga de controlar las animaciones de la mayor parte del programa
 * @param gs Puntero que debe referenciar a la estructura principal del programa
*/
void * animar (void* gs);

/**
 * @brief Se encarga de frenar todos los timers correspondientes a las animaciones
*/
void stopInGameAnimations();

/**
 * @brief Se encarga de inicializar todos los timers necesarios para las animaciones
*/
void startInGameAnimations();

#endif //TPFINAL_ANIMACION_H
