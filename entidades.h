//
// Created by agus on 23/11/20.
//

#ifndef TPFINAL_ENTIDADES_H
#define TPFINAL_ENTIDADES_H

#include "data.h"
#include "fisica.h"
#include <pthread.h>

typedef struct {

    fisica_t fisica;
    int vidas;
    int sobreBloque;
    int estado;         //Muerto, vivo, grande, chiquito,
    int sprite;
    int animation_counter;
    double angle;
} jugador_t;

typedef struct {

    fisica_t fisica;
    pthread_t enemyIA;
    int estado;         //Vivo o muerto
    int identificador;
    int sprite;
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
void destroyEnemy(enemigo_t* thisEnemy);
void setClosestPlayer(jugador_t* player);
void *blooper (void* enemy);
void *cheepcheep (void *enemy);

#endif //TPFINAL_ENTIDADES_H
