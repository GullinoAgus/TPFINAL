//
// Created by gonzalo on 27/11/20.
//

#ifndef TPFINAL_LEVEL_H
#define TPFINAL_LEVEL_H

#include "matiasBrosGame.h"
#include "allegroLib.h"

enum levels{ONE, TWO}; //enum de los niveles para cargar
enum worlds{WONE, WTWO}; //enum de los mundos para cargar


int cargarMapa(level_t* level , int id);
void drawLevel(estadoJuego_t *gameState);
void initUI(gameUI_t* gameUI);
int initEntities(estadoJuego_t* gameState);

#endif //TPFINAL_LEVEL_H
