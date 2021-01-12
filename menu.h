//
// Created by gonzalo on 23/11/20.
//

#ifndef TPFINAL_MENU_H
#define TPFINAL_MENU_H


#include "matiasBrosGame.h"
#include "allegroLib.h"

//Funcion que dibuja el menu y comienza la musica de fondo
int loadMenuData(void);
void drawMenu(estadoJuego_t *gameState);
void updateMenuArrow (int* seleccion, char evento);
void destroyMenu(void); //TODO: Esta funcion la usamos en algun momento?
void drawLevelSelector(estadoJuego_t* gameState);
void drawTopScores(estadoJuego_t * gameState);
void updatePauseArrow (int* seleccion, char evento);
void imprimirHighScore (int numero);


#endif //TPFINAL_MENU_H
