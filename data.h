//
// Created by gonzalo on 23/11/20.
//

#ifndef TPFINAL_DATA_H
#define TPFINAL_DATA_H

#include <stdio.h>
#include "configuracion.h"

#define JUGADOR 'J'
#define BORDE 'B'
#define NADA '0'
#define LADRILLO '1'
#define ALGA '2'
#define FASTCHEEPCHEEP '3'
#define SLOWCHEEPCHEEP '4'
#define PULPITO '5'
#define MONEDA 'M'
#define TOPPIPE 'T'
#define MIDDLEPIPE 't'


#if MODOJUEGO == 0

enum menuImages{FONDOMENU, CARTELMENU, FLECHAMENU, LEVELSELECTORIDLE, LEVELSELECTORLEFT, LEVELSELECTORRIGHT, SCORETABLEIMG};
enum menuTexts{SELECTTEXT, LEVELTEXT, SCORETEXT, EXITTEXT};
enum levelImages{MATIASIDLESPRITE = SCORETABLEIMG+1, PLAYERSWIMMING1, PLAYERSWIMMING2, PLAYERSWIMMING3, PLAYERSWIMMING4, CHEEPCHEEPSPRITE1, CHEEPCHEEPSPRITE2, CHEEPCHEEPSSLOWSPRITE1, CHEEPCHEEPSSLOWSPRITE2, BLOOPERSPRITE1, BLOOPERSPRITE2,
                ALGASPRITE1, ALGASPRITE2, PISOSPRITE, WAVESPRITE, BUBBLESPRITE, COINSPRITE1, COINSPRITE2, PIPEMIDDLESPRITE, PIPETOPSPRITE, CASTELSPRITE };
enum fuentes{SUPERMARIOFONT60, SUPERMARIOFONT80, SUPERMARIOFONT100, SUPERMARIOFONT120, SUPERMARIOFONT140};

#endif

const char* getScoreFilePath();
int openGameStateFile(FILE **gameStateData);
int openTexturesFile(FILE **texturaData);   //Abrimos el archivo que contiene las texturas
int openSoundsFile(FILE **soundData);
int openFontsFile(FILE **fontsData);
int openMenuData(FILE **imageMenuData, FILE **textMenuData);            //Cargamos la informacion de las imagenes y textos del menu
int openLevelData(FILE **levelData, int id);                             //La nueva version del openData para cargar el mapa

#endif //TPFINAL_DATA_H
