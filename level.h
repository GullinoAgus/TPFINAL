//
// Created by gonzalo on 27/11/20.
//

#ifndef TPFINAL_LEVEL_H
#define TPFINAL_LEVEL_H

#include "matiasBrosGame.h"
#include "allegroLib.h"

#define MAXPLAYERNAME 11


int cargarMapa(level_t* level , int id);
void destroyMap(estadoJuego_t* gameState);
void drawLevel(estadoJuego_t *gameState);
void initUI(gameUI_t* gameUI);
int initEntities(estadoJuego_t* gameState);
void drawRetryScreen(estadoJuego_t* gameState);
void drawGameOverScreen(estadoJuego_t* gameState);
void drawPause(estadoJuego_t *gameState);
void destroyEntities(estadoJuego_t * gameState);
void resetEntitiesState(estadoJuego_t* gameState);
void drawNextLevelScreen(estadoJuego_t *gameState);
void resetWavePosition(void);
int wasNewHighScoreAchieved(estadoJuego_t* gameState);
void saveNewHighScore(estadoJuego_t* gameState);

#if MODOJUEGO == 1

void drawGameOverScreenHighScore(estadoJuego_t* gameState);

#endif

#endif //TPFINAL_LEVEL_H
