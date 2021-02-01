/***************************************************************************//**
  @file     configuracion.h
  @brief    Archivo principal de configuracion del juego.
 ******************************************************************************/

#ifndef TPFINAL_CONFIGURACION_H
#define TPFINAL_CONFIGURACION_H

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define ALLEGRO 0
#define RASPI 1
#define MODOJUEGO ALLEGRO     //0 para jugar con la pc, 1 para jugar con la raspi

#define SCREENWIDHT 1280
#define SCREENHEIGHT 640
#define MAXMENUWORDSIZE 30
#define PIXELSPERUNIT (SCREENHEIGHT/16)
#define MAXPLAYERNAME 11
#define NULLENTITIE -1
#define MAXLEVELTIME 200
#define FPS 60
#define MAXLIVES 3
#define HURRYUPTIME 60

//TODO: Si hacemos un makefile tenemos que recordar sacar esta wea del codigo de compilacion. Si la dejamos tenemos que sacarle los .h

// CODIGO DE COMPILACION PARA LA RASPBERRYPI
//gcc main.c IEvents.c IEvents.h fisica.c fisica.h entidades.h entidades.c matiasBrosGame.h data.c data.h menu.c menu.h configuracion.h allegroLib.c allegroLib.h level.c level.h raspi.h gamelogic.c gamelogic.h render.c render.h animacion.c animacion.h times.c times.h audio.c audio.h disdrv.o disdrv.h joydrv.o joydrv.h -lSDL2 -lpthread -o TPFINAL -Wall -g

/*******************************************************************************
 ******************************************************************************/

#endif //TPFINAL_CONFIGURACION_H
