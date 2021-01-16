//
// Created by gonzalo on 23/11/20.
//

#ifndef TPFINAL_CONFIGURACION_H
#define TPFINAL_CONFIGURACION_H

enum THREADS {INPUT, GAMELOGIC, ANIMATION, PHYSICS, RENDER};
enum PLATAFORMAS {PC = 0, RASPBERRYPI = 1};

#define MODOJUEGO 0     //0 para jugar con la pc, 1 para jugar con la raspi
#define SCREENWIDHT 1280
#define SCREENHEIGHT 640
#define MAXMENUWORDSIZE 30
#define PIXELSPERUNIT (SCREENHEIGHT/16)
#define MAXSCORELENGTH 4
#define MAXPLAYERNAME 11
#define NULLENTITIE -1
#define UTIEMPOREFRESCO 3000
#define MAXLEVELTIME 400
#define FPS 100
#define MAXLIVES 3

// CODIGO DE COMPILACION PARA LA RASPBERRYPI, hay que updatearlo
// gcc main.c animacion.c data.c entidades.c fisica.c gamelogic.c IEvents.c level.c menu.c render.c times.c disdrv.o joydrv.o -o MAINRASPIEXE -Wall -pthread -g

#endif //TPFINAL_CONFIGURACION_H
