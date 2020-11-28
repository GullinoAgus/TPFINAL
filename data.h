//
// Created by gonzalo on 23/11/20.
//

#ifndef TPFINAL_DATA_H
#define TPFINAL_DATA_H

#include "matiasBrosGame.h"

#define JUGADOR 'J'
#define BORDE 'B'
#define NADA '0'
#define LADRILLO '1'
#define ALGA '2'
#define CHEEPCHEEP '3'
#define PULPITO '4'        // <- MOVER?

#if MODOJUEGO == 0

int openGameStateFile(FILE **gameStateData);
int openTexturesFile(FILE **texturaData);   //Abrimos el archivo que contiene las texturas
int openSoundsFile(FILE **soundData);
int openFontsFile(FILE **fontsData);
int openMenuData(FILE **imageMenuData, FILE **textMenuData);            //Cargamos la informacion de las imagenes y textos del menu
int openLevelData(FILE **levelData, int id);                             //La nueva version del openData para cargar el mapa
//int* openLevelData(int id);     //Cargar los datos del nivel segun id REVISAR

#endif

#endif //TPFINAL_DATA_H
