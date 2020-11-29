//
// Created by agus on 23/11/20.
//

#ifndef TPFINAL_ENTIDADES_H
#define TPFINAL_ENTIDADES_H

#include "data.h"
#include "fisica.h"

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



#endif //TPFINAL_ENTIDADES_H
