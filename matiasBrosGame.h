//
// Created by gonzalo on 23/11/20.
//

#ifndef TPFINAL_MATIASBROSGAME_H
#define TPFINAL_MATIASBROSGAME_H

#include "configuracion.h"
#define MAXTOPSCORES 10

typedef struct{
    int bestScores[MAXTOPSCORES];       //Mejores puntajes
    char *bestScoresName[MAXTOPSCORES]; //Nombre de los jugadores con mejor puntaje
    int currentTimeLeft;                //Tiempo restante para terminar el nivel
    int currentScore;                   //Puntaje actual
}estadoJuego_t;

#if MODOJUEGO == 0
    #include <stdio.h>
    #include "menu.h"
    #include "data.h"
    #include <allegro5/allegro5.h>
    #include <allegro5/allegro_image.h>
    #include <allegro5/allegro_native_dialog.h>
    #include <allegro5/allegro_font.h>
    #include <allegro5/allegro_ttf.h>
    #include <allegro5/allegro_audio.h>
    #include <allegro5/allegro_acodec.h>
#elif MODOJUEGO == 1
    //Incluimos las bibliotecas para la raspi
#endif

#endif //TPFINAL_MATIASBROSGAME_H
