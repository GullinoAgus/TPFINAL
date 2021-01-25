/***************************************************************************//**
  @file     entidades.h
  @brief    definicion de entidades_t y funciones relacionadas a la misma
 ******************************************************************************/

#ifndef TPFINAL_ENTIDADES_H
#define TPFINAL_ENTIDADES_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "data.h"
#include "fisica.h"
#include <pthread.h>

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef struct {

    double angleRotation;
    fisica_t fisica;
    int vidas;
    int sobreBloque;
    int isMoving;
    int powerUpsState;
    int estado;         //Muerto, vivo,
    int sprite;
} jugador_t;

typedef struct {

    fisica_t fisica;
    pthread_t enemyIA;
    int estado;         //Vivo o muerto o dormido
    int identificador;
    int sprite;
    int moveAgain;
    void* (*funcionMovimiento)(void*);
} enemigo_t;

typedef struct {
    fisica_t fisica;
    int identificador;
    int sprite;
} bloque_t;

typedef struct {
    jugador_t jugador;
    enemigo_t *enemigos;
    bloque_t *bloques;
} entidades_t;

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Inicializa el thread de un enemigo correspondiente a su movimiento
 * @param *thisEnemy puntero a estructura enemigo_t de donde obtendra la funcion de su movimiento y el id del thread
*/
void startEnemy(enemigo_t* thisEnemy);

/**
 * @brief Destruye el thread de un enemigo correspondiente a su movimiento
 * @param *thisEnemy puntero a estructura enemigo_t de donde obtendra el id del thread
*/
void destroyEnemyIA(enemigo_t* thisEnemy);

/**
 * @brief Se utiliza para determinar al jugador actual (matias), es necesaria para que luego los blooper puedan moverse adecuadamente
 * @param *player Recibe un puntero a jugador_t player
*/
void setClosestPlayer(jugador_t* player);

/**
 * @brief Funcion que determina, en un thread particular, el movimiento de un blooper
 * @param *enemy recibe un puntero al enemigo del cual modificaremos su movimiento
*/
void *blooper (void* enemy);

/**
 * @brief Funcion que determina, en un thread particular, el movimiento de un cheepcheep
 * @param *enemy recibe un puntero al enemigo del cual modificaremos su movimiento
*/
void *cheepcheep (void *enemy);

#endif //TPFINAL_ENTIDADES_H
