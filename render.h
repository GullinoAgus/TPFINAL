/***************************************************************************//**
  @file     render.h
  @brief    Controla el refresco de pantalla y lo que se muestra en ella
 ******************************************************************************/

#ifndef TPFINAL_RENDER_H
#define TPFINAL_RENDER_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "matiasBrosGame.h"
#include "fisica.h"

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Se encarga de refrescar la pantalla cada cierto tiempo indicado por timer
 * @param gs Se debe pasar un puntero a la estructura principal del programa
*/
void *render (void *gs);

/**
 * @brief Actualiza la posición de la pantalla sobre el nivel
 * @param gs Se debe pasar un puntero a la estructura principal del programa
*/
void updateCameraPosition(void* gs);

/**
 * @brief Determina si cierta entidad se encuentra o no en la pantalla (eje X)
 * @param object1 puntero a estructura fisica_t de la entidad a analizar
 * @return Devuelve 1 si la entidad se encuentra en la pantalla, en caso contrario devuelve 0
*/
int isInsideScreenX(fisica_t* object1);

/**
 * @brief Asigna el primer parámetro a la posición actual de la pantalla en el nivel
 * @param coordX Valor que se desea asignar
*/
void setCameraScrollX(float coordX);

/**
 * @brief Devuelve la posición actual de la pantalla en el nivel
 * @return Devuelve el valor correspondiente en tipo flotante
*/
float getCameraScrollX();

/**
 * @brief Asigna NULL a la variable LastBlockInMap
*/
void resetLastBlockInMap();

#if MODOJUEGO == 1

/**
 * @brief Esta función controla el buffer del display a partir de la matriz dada, si un elemento vale 0 se apaga el led correspondiente, si es 1 se prende, no hay cambios en cualquier otro caso
 * @param matriz Matriz con datos para el control de leds
*/
void writeDisplay(char matriz[16][16]);

/**
 * @brief Determina si cierta entidad se encuentra o no en la pantalla (eje Y)
 * @param object1 puntero a estructura fisica_t de la entidad a analizar
 * @return Devuelve 1 si la entidad se encuentra en la pantalla, en caso contrario devuelve 0
*/
int isInsideScreenY(fisica_t* object1);

#endif

#endif //TPFINAL_RENDER_H
