//
// Created by agus on 23/11/20.
//

#ifndef TPFINAL_ENTIDADES_H
#define TPFINAL_ENTIDADES_H

#include "data.h"
#include "fisica.h"
#include <pthread.h>

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

//Recibe un puntero a gameState
void startEnemy(enemigo_t* thisEnemy);
void destroyEnemyIA(enemigo_t* thisEnemy);
void setClosestPlayer(jugador_t* player);
void *blooper (void* enemy);
void *cheepcheep (void *enemy);

#endif //TPFINAL_ENTIDADES_H
