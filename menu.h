//
// Created by gonzalo on 23/11/20.
//

#ifndef TPFINAL_MENU_H
#define TPFINAL_MENU_H


#if MODOJUEGO == 0

#include "matiasBrosGame.h"
#include "allegroLib.h"

//Funcion que dibuja el menu y comienza la musica de fondo
int loadMenuData();
int drawMenu(estadoJuego_t *gameState);
void updateMenuArrow (int* seleccion, char evento);
void destroyMenu();
void drawLevelSelector(estadoJuego_t* gameState);
void drawTopScores(estadoJuego_t * gameState);

#elif MODOJUEGO == 1

int actualizarMenu (void);
int verTopScores (void);
int TopScore(void);
void imprimirHighScore (int numero);

#endif


#endif //TPFINAL_MENU_H
