//
// Created by gonzalo on 23/11/20.
//

#ifndef TPFINAL_MATIASBROSGAME_H
#define TPFINAL_MATIASBROSGAME_H

#include "configuracion.h"
#include "entidades.h"

#define MAXTOPSCORES 10
#define MAXCIFRASSCORE 10
#define MAXBESTSCORENAME 20

//cargo el nivel, inicializo entidades: de Gonza, NO BORRAR

typedef struct{

    int maxEntries;                                         //Cantidad de scores a leer
    int bestScores[MAXTOPSCORES];                           //Mejores puntajes
    char bestScoresName[MAXTOPSCORES][MAXBESTSCORENAME];    //Nombre de los jugadores con mejor puntaje

    int state;                                  // 0: menu
                                                // 1: seleccion de nivel
                                                // 2: tabla de scores
                                                // 3: en juego

    int menuSelection;                          /*Posicion de la flecha para seleccionar las opciones del menu   0 para el menu
                                                                                                                 1 para empezar el juego
                                                                                                                 2 para ver la tabla de puntajes */
    entidades_t entidades;

    int *level;                                 //numeros que indica que bloques hay en cada posicion del juego


}estadoJuego_t;

#if MODOJUEGO == 0
    #include <stdio.h>
    #include <allegro5/allegro5.h>
    #include <allegro5/allegro_image.h>
    #include <allegro5/allegro_native_dialog.h>
    #include <allegro5/allegro_font.h>
    #include <allegro5/allegro_ttf.h>
    #include <allegro5/allegro_audio.h>
    #include <allegro5/allegro_acodec.h>
    #include "menu.h"
    #include "data.h"
    #include "allegroLib.h"

#elif MODOJUEGO == 1
    //Incluimos las bibliotecas para la raspi
#endif

#endif //TPFINAL_MATIASBROSGAME_H
