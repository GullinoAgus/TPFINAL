//
// Created by gonzalo on 27/11/20.
//

#ifndef TPFINAL_LEVEL_H
#define TPFINAL_LEVEL_H

#include "matiasBrosGame.h"
#include "allegroLib.h"

enum levels{ONE = 1, TWO}; //enum de los niveles para cargar

int cargarMapa(level_t* level , int id);
void destroyMap(estadoJuego_t* gameState);
void drawLevel(estadoJuego_t *gameState);
void initUI(gameUI_t* gameUI);
int initEntities(estadoJuego_t* gameState);
void drawRetryScreen(estadoJuego_t *gameState);
void drawPause(estadoJuego_t *gameState);
void destroyEntities(estadoJuego_t * gameState);
void resetEntitiesPosition(estadoJuego_t* gameState);
void drawNextLevel(estadoJuego_t *gameState);

#endif //TPFINAL_LEVEL_H
