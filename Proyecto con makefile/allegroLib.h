/***************************************************************************//**
  @file     allegroLib.h
  @brief    Funciones de carga de datos e inicializacion relacionadas a Allegro. Tambien carga de highscores.
 ******************************************************************************/

#ifndef TPFINAL_ALLEGROLIB_H
#define TPFINAL_ALLEGROLIB_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "matiasBrosGame.h"

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Carga todos los sonidos conociendo la cantidad y su path desde soundsData.txt
 * @param **sonido puntero a la estructura sonido_t dentro de gameState
 * @return Devuelve 0 si se cargo bien, sino -1
*/
int cargarSonidosMenu(sonido_t **sonido);

/**
 * @brief Libera el espacio reservado para las imagenes, el sonido y las fuentes
 * @param *resourcesBuffer puntero a la estructura bufferRecursos_t la cual contiene todos los datos que destruira
*/
void destroyResources(bufferRecursos_t *resourcesBuffer);

#if MODOJUEGO == ALLEGRO

/**
 * @brief Inicializa tod0 lo necesario para luego utilizar allegro en el programa
 * @return Devuelve 0 si se cargo bien, sino -1
*/
int inicializarAllegro();

/**
 * @brief Carga todas las texturas conociendo la cantidad y su path desde texturesData.txt
 * @param **textura puntero a la estructura image_t dentro de gameState
 * @return Devuelve 0 si se cargo bien, sino -1
*/
int cargarTexturasMenu(image_t **textura);

/**
 * @brief Carga todas las fuentes conociendo la cantidad y su path desde fontsData.txt
 * @param **fuente puntero a la estructura fuente_t dentro de gameState
 * @return Devuelve 0 si se cargo bien, sino -1
*/
int cargarFuentesMenu(fuente_t **fuente);

#endif

/**
 * @brief Se carga la lista de high scores al programa
 * @param gameState Puntero a gameState
 * @return Devuelve 0 si se cargo bien, sino -1
*/
int loadGameState(estadoJuego_t *gameState);

#endif //TPFINAL_ALLEGROLIB_H
