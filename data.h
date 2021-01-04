//
// Created by gonzalo on 23/11/20.
//

#ifndef TPFINAL_DATA_H
#define TPFINAL_DATA_H

#include <stdio.h>

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
enum menuTexts{SELECTTEXT, LEVELTEXT, SCORETEXT};
enum levelImages{MATIASIDLESPRITE = SCORETABLEIMG+1, CHEEPCHEEPSPRITE1, CHEEPCHEEPSPRITE2, CHEEPCHEEPSSLOWSPRITE1, CHEEPCHEEPSSLOWSPRITE2, BLOOPERSPRITE1, BLOOPERSPRITE2,
                ALGASPRITE1, ALGASPRITE2, PISOSPRITE, WAVESPRITE, BUBBLESPRITE, COINSPRITE1, COINSPRITE2, PIPEMIDDLESPRITE, PIPETOPSPRITE };
enum fuentes{SUPERMARIOFONT80, SUPERMARIOFONT50};

int openGameStateFile(FILE **gameStateData);
int openTexturesFile(FILE **texturaData);   //Abrimos el archivo que contiene las texturas
int openSoundsFile(FILE **soundData);
int openFontsFile(FILE **fontsData);
int openMenuData(FILE **imageMenuData, FILE **textMenuData);            //Cargamos la informacion de las imagenes y textos del menu
int openLevelData(FILE **levelData, int id);                             //La nueva version del openData para cargar el mapa
//int* openLevelData(int id);     //Cargar los datos del nivel segun id REVISAR

#endif

#endif //TPFINAL_DATA_H
