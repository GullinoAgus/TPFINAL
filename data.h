//
// Created by gonzalo on 23/11/20.
//

#ifndef TPFINAL_DATA_H
#define TPFINAL_DATA_H

#include "matiasBrosGame.h"

#if MODOJUEGO == 0

int loadGameState(estadoJuego_t *gameState);   //Cargamos los datos del juego
int openTexturesFile(FILE **texturaData);   //Abrimos el archivo que contiene las texturas
int openSoundsFile(FILE **soundData);
int openFontsFile(FILE **fontsData);
int openMenuData(FILE **imageMenuData, FILE **textMenuData);            //Cargamos la informacion de las imagenes y textos del menu

#endif

#endif //TPFINAL_DATA_H
