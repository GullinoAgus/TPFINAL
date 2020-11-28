//
// Created by agus on 23/11/20.
//

#ifndef TPFINAL_ENTIDADES_H
#define TPFINAL_ENTIDADES_H

#include "matiasBrosGame.h"

#define CHEEPCHEEP 1
#define PULPITO 2
#define LADRILLO 1
#define ALGA 2

typedef struct {
    float posx;
    float posy;
    int ancho;
    int alto;
    float velx;
    float vely;
} fisica_t;

typedef struct {

    fisica_t fisica;
    int vidas;
    int sobreBloque;
    int estado;         //Muerto, vivo, grande, chiquito,
    int sprite;
} jugador_t;

typedef struct {

    fisica_t fisica;
    int estado;
    int identificador;
    int sprite;
} enemigo_t;

typedef struct {

    fisica_t fisica;
    int identificador;
    int sprite;
} bloque_t;

typedef struct {
    jugador_t jugador;
    enemigo_t **enemigos;
    bloque_t **bloques;
} entidades_t;

void initEntities(estadoJuego_t* gameState);


#endif //TPFINAL_ENTIDADES_H
