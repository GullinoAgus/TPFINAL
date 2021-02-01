/***************************************************************************//**
  @file     menu.h
  @brief    Funciones relacionadas al dibujado y manejo del menu.
 ******************************************************************************/

#ifndef TPFINAL_MENU_H
#define TPFINAL_MENU_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "render.h"

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

#if MODOJUEGO == ALLEGRO

/**
 * @brief Libera el espacio reservado en memoria para las imagenes y texto del menu.
*/
void destroyMenu(void);

#elif MODOJUEGO == RASPI

/**
 * @brief Dibuja el numero recibido en la parte inferior del display de la raspi. 4 cifras maximo
 * @param numero es el numero a dibujar
 * @param zeroEnabled Si se indica 1, entonces se imprimen las 4 cifras. Si se indica 0, entonces se imprime una sola a la derecha
*/
void imprimirNumero (int numero, int zeroEnabled);

#endif

/**
 * @brief Dibuja el menu
 * @param *gameState puntero a gameState donde se encuentra toda la informacion sobre las imagenes y texto del menu
*/
void drawMenu(estadoJuego_t *gameState);

/**
 * @brief Carga la informacion de los textos y las imagenes del menu dentro del juego (en memoria dinamica) para luego poder dibujar tod0. Para la Raspi esta funcion no hace nada
 * @return Devuelve 0 si la operacion fue correcta y 1 en el caso contrario
*/
int loadMenuData(void);

/**
 * @brief Dibuja la pantalla de seleccion de nivel
 * @param *gameState puntero a gameState donde se encuentra toda la informacion sobre las imagenes y el nivel seleccionado
*/
void drawLevelSelector(estadoJuego_t* gameState);

/**
 * @brief Dibuja la pantalla de highscores
 * @param *gameState puntero a gameState donde se encuentra toda la informacion sobre las imagenes y texto de la pantalla de highscores
*/
void drawTopScores(estadoJuego_t * gameState);

/**
 * @brief Actualiza la flecha del menu segun los limites de la misma
 * @param *seleccion recibe un puntero a la posicion donde debe modificar la seleccion
 * @param evento Recibe el evento realizado, es decir arriba, abajo o izquierda-derecha
*/
void updateMenuArrow (int* seleccion, unsigned char evento);

/**
 * @brief Actualiza la flecha del menu de pausa segun los limites de la misma
 * @param *seleccion recibe un puntero a la posicion donde debe modificar la seleccion
 * @param evento Recibe el evento realizado, es decir arriba, abajo o izquierda-derecha
*/
void updatePauseArrow (int* seleccion, unsigned char evento);


 //TPFINAL_MENU_H
#endif