//
// Created by gonzalo on 23/11/20.
//

#ifndef TPFINAL_MENU_H
#define TPFINAL_MENU_H


#if MODOJUEGO == 0

#include "matiasBrosGame.h"

//Funcion que dibuja el menu y comienza la musica de fondo
int drawMenu(bufferRecursos_t *buffer);
void updateMenu (int *seleccion, char evento);
void destroyMenu();

int verTopScores(estadoJuego_t * gameState, bufferRecursos_t *buffer);

#elif MODOJUEGO == 1

int actualizarMenu (void);
int verTopScores (void);
int TopScore(void);

#endif

void imprimirHighScore (int numero);

#endif //TPFINAL_MENU_H
