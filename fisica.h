/***************************************************************************//**
  @file     +fisica.h+
  @brief    +Libreria para motor de fisica+
  @author   +Grupo 1+
 ******************************************************************************/
#ifndef TPFINAL_FISICA_H
#define TPFINAL_FISICA_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "gamelogic.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define VELOCIDADXMAX (0.3f)
#define VELOCIDADYMAX (0.6f)
#define GRAVEDAD 0.009f
#define INERCIA 0.975f


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
void* fisica(void* entrada);
void movePlayer(int direction, void* player);

#endif //TPFINAL_FISICA_H
