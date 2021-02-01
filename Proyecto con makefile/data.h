/***************************************************************************//**
  @file     data.h
  @brief    Funciones para la lectura de archivos .txt localizados en la carpeta data.
 ******************************************************************************/

#ifndef TPFINAL_DATA_H
#define TPFINAL_DATA_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdio.h>
#include "configuracion.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/**Estas constantes definen como se identifican los simbolos que ponemos en el .txt del nivel */
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
#define MUSHROOM 'H'

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

#if MODOJUEGO == 0

enum menuImages{FONDOMENU, CARTELMENU, FLECHAMENU, LEVELSELECTORIDLE, LEVELSELECTORLEFT, LEVELSELECTORRIGHT, SCORETABLEIMG};
enum menuTexts{SELECTTEXT, LEVELTEXT, SCORETEXT, EXITTEXT};
enum levelImages{MATIASIDLESPRITE = SCORETABLEIMG+1, PLAYERSWIMMING1, PLAYERSWIMMING2, PLAYERSWIMMING3, PLAYERSWIMMING4, MATIASIDLEBIGSPRITE, PLAYERSWIMMINGBIG1, PLAYERSWIMMINGBIG2, PLAYERSWIMMINGBIG3, PLAYERSWIMMINGBIG4, CHEEPCHEEPSPRITE1, CHEEPCHEEPSPRITE2, CHEEPCHEEPSSLOWSPRITE1, CHEEPCHEEPSSLOWSPRITE2, BLOOPERSPRITE1, BLOOPERSPRITE2,
                ALGASPRITE1, ALGASPRITE2, PISOSPRITE, WAVESPRITE, BUBBLESPRITE, COINSPRITE1, COINSPRITE2, COMMONMUSHROOMSPRITE, PIPEMIDDLESPRITE, PIPETOPSPRITE, CASTELSPRITE };
enum fuentes{SUPERMARIOFONT60, SUPERMARIOFONT80, SUPERMARIOFONT100, SUPERMARIOFONT120, SUPERMARIOFONT140};

#endif

enum audio{SUPERMARIOTHEME, UNDERWATERTHEME, PICKUPCOIN, JUMPSMALL, PAUSEGAME, ENTERPIPE, GAMEOVERSOUND, MARIODIES, POWERUPSOUND, ONEUP, WARNINGTIMEOUT};

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Lectura del archivo estadojuegoData.txt donde se encuentran los highscores
 * @param **gameStateData puntero a FILE donde se localizara el archivo abierto
 * @return Devuelve 0 si la operacion fue correcta, en caso contrario devuelve 1
*/
int openGameStateFile(FILE **gameStateData);

#if MODOJUEGO == ALLEGRO

/**
 * @brief Lectura del archivo texturesData.txt donde se encuentran las texturas
 * @param **texturaData puntero a FILE donde se localizara el archivo abierto
 * @return Devuelve 0 si la operacion fue correcta, en caso contrario devuelve 1
*/
int openTexturesFile(FILE **texturaData);

/**
 * @brief Lectura del archivo fontsData.txt donde se encuentran las fuentes
 * @param **fontsData puntero a FILE donde se localizara el archivo abierto
 * @return Devuelve 0 si la operacion fue correcta, en caso contrario devuelve 1
*/
int openFontsFile(FILE **fontsData);

/**
 * @brief Lectura del archivo imgMenuData.txt y textMenuData.txt donde se encuentran las imagenes y textos del menu
 * @param **imageMenuData puntero a FILE donde se localizara el archivo abierto
 * @param **textMenuData puntero a FILE donde se localizara el archivo abierto
 * @return Devuelve 0 si la operacion fue correcta, en caso contrario devuelve 1
*/
int openMenuData(FILE **imageMenuData, FILE **textMenuData);

#endif

/**
 * @brief Lectura del archivo levelX.txt donde se encuentra un nivel en particular
 * @param **levelData puntero a FILE donde se localizara el archivo abierto
 * @param id El nivel a abrir, un numero entre 0 para el nivel 1, 1 para el nivel 2 y 2 para el nivel 3.
 * @return Devuelve 0 si la operacion fue correcta, en caso contrario devuelve 1
*/
int openLevelData(FILE **levelData, int id);

/**
 * @brief Lectura del archivo soundsData.txt donde se encuentran los sonidos
 * @param **soundData puntero a FILE donde se localizara el archivo abierto
 * @return Devuelve 0 si la operacion fue correcta, en caso contrario devuelve 1
*/
int openSoundsFile(FILE **soundData);

/**
 * @brief Lee el nombre de los archivos .txt de la forma level_.txt y devuelve la cantidad de nivele disponibles
 * @return Retorna la cantidad de niveles disponibles
*/
int getMaxLevelsAvailable();

#endif //TPFINAL_DATA_H
