/***************************************************************************//**
  @file     matiasBrosGame.h
  @brief    Header que contiene algunos estados del juego y donde principalmente se define estadoJuego_t.
 ******************************************************************************/

#ifndef TPFINAL_MATIASBROSGAME_H
#define TPFINAL_MATIASBROSGAME_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "entidades.h"
#include "audio.h"
#include "IEvents.h"
#include <semaphore.h>

typedef Audio* sonido_t;

#if MODOJUEGO == ALLEGRO

#include "allegro.h"

#elif MODOJUEGO == RASPI

#include "raspi.h"

#endif

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define MAXTOPSCORES 10
#define MAXBESTSCORENAME 20

#if MODOJUEGO == ALLEGRO

#define MAXCIFRASSCORE 10

#elif MODOJUEGO == RASPI

#define MAXCIFRASSCORE 4    //PARA LA RASPI, LA MAXIMA CANTIDAD DE DIGITOS PARA EL PUNTAJE ES DE 4

#endif

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

enum POWERUPSSTATE {SMALL, BIG}; //estado del powerUpsState del personaje
typedef enum ESTADOSDELJUEGO { MENU = 10, CHOOSINGLEVEL, INSCORETABLE, INGAME, RETRYSCREEN ,GAMECLOSED, PAUSE, NEXTLEVEL, GAMEOVERSCREEN} estadosjuego_t; //estado de gamestate.state
enum MENUOPTIONS {LEVELSELECTOR = 1, SCORETABLE, EXITGAME}; //estados de gamestate.menuSelection
enum PAUSEOPTIONS {RESUME = 0, BACKTOMENU};

typedef Audio* sonido_t;

#if MODOJUEGO == ALLEGRO

    typedef ALLEGRO_BITMAP* image_t;
    typedef ALLEGRO_FONT* fuente_t;

    typedef struct{
        int imageQuant;
        int soundQuant;
        int fontQuant;
        image_t *image;
        sonido_t *sound;
        fuente_t *font;
    }bufferRecursos_t;

#elif MODOJUEGO == RASPI

    typedef struct{
        int soundQuant;
        sonido_t *sound;
    }bufferRecursos_t;

#endif

typedef struct{
    int** level; //Aqui se reservara memoria para las constantes que indican que bloques hay en cada posicion del juego
    int levelWidht;
    int levelHeight;
}level_t;

typedef struct{                                //Todos los datos que aparecen en la UI
    int score;
    int coins;
    int level;
    int time;
}gameUI_t;

typedef struct{

    int maxTopScoreEntries;                                         //Cantidad de scores a leer
    int bestScores[MAXTOPSCORES];                           //Mejores puntajes
    char bestScoresName[MAXTOPSCORES][MAXBESTSCORENAME];    //Nombre de los jugadores con mejor puntaje

    estadosjuego_t state;                       // 0: menu
                                                // 1: seleccion de nivel
                                                // 2: tabla de scores
                                                // 3: jugando un nivel
                                                // 4: cerrar juego

    int menuSelection;                          /*Posicion de la flecha para seleccionar las opciones del menu  1 para empezar el juego
                                                                                                                2 para los niveles
                                                                                                                3 para ver la tabla de puntajes
                                                                                                                */
    int pauseSelection;                         /*Posicion de la flecha para seleccionar las opciones de la pausa  1 para reanudar el juego
                                                                                                                   2 para reiniciar el nivel
                                                                                                                   3 para volver al menu principal
                                                                                                                */

    entidades_t entidades;
    entidades_t defaultEntities;

    level_t level;

    bufferRecursos_t buffer;

    gameUI_t gameUI;

    char *pPlayerName;

}estadoJuego_t;

/*******************************************************************************
 ******************************************************************************/


#endif //TPFINAL_MATIASBROSGAME_H
