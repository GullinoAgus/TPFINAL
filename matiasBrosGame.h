//
// Created by gonzalo on 23/11/20.
//

#ifndef TPFINAL_MATIASBROSGAME_H
#define TPFINAL_MATIASBROSGAME_H

#include "configuracion.h"
#include "entidades.h"
#include "allegro.h"

#if MODOJUEGO == 0

#include "render.h"

#elif MODOJUEGO == 1
    #include "joydrv.h"
//  #include "libaudio.h"
    #include "disdrv.h"
    #include "termlib.h"
#endif

#define MAXTOPSCORES 10
#define MAXCIFRASSCORE 10
#define MAXBESTSCORENAME 20

enum CHARACTERSTATE {ALIVE, DEAD, ALMOSTDEAD, INVULNERABLE}; //estados de estado del personaje
enum POWERUPSSTATE {BIG, SMALL, BIGWITHFIRE}; //estado del powerUpsState del personaje
enum ESTADOSDELJUEGO { MENU = 10, CHOOSINGLEVEL, INSCORETABLE, INGAME, LOADINGLEVEL, RETRYSCREEN ,GAMECLOSED, PAUSE}; //estado de gamestate.state
enum MENUOPTIONS {LEVELSELECTOR = 1, SCORETABLE}; //estados de gamestate.menuSelection
enum PAUSEOPTIONS {RESUME = 1, RESTART, BACKTOMENU};

typedef ALLEGRO_BITMAP* image_t;
typedef ALLEGRO_SAMPLE* sonido_t;
typedef ALLEGRO_FONT* fuente_t;

typedef struct{
    int** level;                                 //numeros que indica que bloques hay en cada posicion del juego
    int levelWidht;
    int levelHeight;
}level_t;

typedef struct{
    int score;
    int coins;
    int level;
    int time;
}gameUI_t;

typedef struct{
    int imageQuant;
    int soundQuant;
    int fontQuant;
    image_t *image;
    sonido_t *sound;
    fuente_t *font;
}bufferRecursos_t;

typedef struct{

    int maxEntries;                                         //Cantidad de scores a leer
    int bestScores[MAXTOPSCORES];                           //Mejores puntajes
    char bestScoresName[MAXTOPSCORES][MAXBESTSCORENAME];    //Nombre de los jugadores con mejor puntaje

    int state;                                  // 0: menu
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

    level_t level;

    bufferRecursos_t buffer;

    gameUI_t gameUI;

}estadoJuego_t;


#endif //TPFINAL_MATIASBROSGAME_H
