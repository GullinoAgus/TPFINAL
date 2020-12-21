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
#define ALIVE 0
#define DEAD 1

enum ESTADOSDELJUEGO { MENU = 10, CHOOSINGLEVEL, INSCORETABLE, INGAME, GAMECLOSED}; //estado de gamestate.state
enum MENUOPTIONS {LEVELSELECTOR = 1, SCORETABLE, PLAYGAME}; //estados de gamestate.menuSelection

typedef ALLEGRO_BITMAP* image_t;
typedef ALLEGRO_SAMPLE* sonido_t;
typedef ALLEGRO_FONT* fuente_t;

typedef struct {
    int** level;                                 //numeros que indica que bloques hay en cada posicion del juego
    int levelWidht;
    int levelHeight;
} level_t;

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
    entidades_t entidades;

    level_t level;

    bufferRecursos_t buffer;

    gameUI_t gameUI;

}estadoJuego_t;


#endif //TPFINAL_MATIASBROSGAME_H
