//
// Created by gonzalo on 27/11/20.
//

#ifndef TPFINAL_LEVEL_H
#define TPFINAL_LEVEL_H

#include "matiasBrosGame.h"
#include "allegroLib.h"

enum levels{ONE, TWO}; //enum de los niveles para cargar

int cargarMapa(int ***mapa, int id, int* rows, int* columns);
void drawLevel(estadoJuego_t *gameState, bufferRecursos_t *resourceBuffer);
int initEntities(estadoJuego_t* gameState);

#endif //TPFINAL_LEVEL_H
