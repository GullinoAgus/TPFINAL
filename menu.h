//
// Created by gonzalo on 23/11/20.
//

#ifndef TPFINAL_MENU_H
#define TPFINAL_MENU_H

#include "allegroLib.h"
#include "matiasBrosGame.h"

//Funcion que dibuja el menu y comienza la musica de fondo
int drawMenu(bufferRecursos *buffer, int posYflecha);
int actualizarMenu (bufferRecursos *buffer);
void destroyMenu();

int verTopScores(estadoJuego_t * gameState,bufferRecursos *buffer);

// void actualizarMenu (void);


#endif //TPFINAL_MENU_H
