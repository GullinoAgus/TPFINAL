/***************************************************************************//**
  @file     +fisica.h+
  @brief    +Libreria para motor de fisica+
  @author   +Grupo 1+
 ******************************************************************************/
#ifndef TPFINAL_FISICA_H
#define TPFINAL_FISICA_H

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define VELOCIDADXMAX (0.3f)
#define VELOCIDADYMAX (0.6f)
#define GRAVEDAD 0.009f
#define INERCIA 0.975f

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef struct {
    float posx;
    float posy;
    int ancho;
    int alto;
    float velx;
    float vely;
} fisica_t;

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
void* fisica(void* entrada);
void movePlayer(int direction, void* player);

#endif //TPFINAL_FISICA_H
