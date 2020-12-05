//
// Created by gonzalo on 23/11/20.
//

#ifndef TPFINAL_CONFIGURACION_H
#define TPFINAL_CONFIGURACION_H

enum THREADS {INPUT, GAMELOGIC, ANIMATION, PHYSICS, RENDER};

#define MODOJUEGO 0     //0 para jugar con la pc, 1 para jugar con la raspi
#define SCREENWIDHT 800
#define SCREENHEIGHT 400
#define MAXMENUWORDSIZE 30
#define PIXELSPERUNIT (SCREENHEIGHT/16)
#define MAXSCORELENGTH 4
#define NULLENTITIE -1

// CODIGO DE COMPILACION PARA LA RASPBERRYPI
// gcc main.c disdrv.o IEvents.c joydrv.o menu.c -o MAINRASPIEXE -Wall -pthread -g

#endif //TPFINAL_CONFIGURACION_H
