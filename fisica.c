//
// Created by agus on 23/11/20.
//


#include "fisica.h"

#define UTIEMPOESPERA 100

int* actualizacionPos(entidad_t* listaEntidades){

    for (int i = 0; &listaEntidades[i] != NULL ; ++i) {
        listaEntidades[i].fisica.posx += UTIEMPOESPERA*listaEntidades[i].fisica.velx;
        listaEntidades[i].fisica.posy += UTIEMPOESPERA*listaEntidades[i].fisica.vely;
    }

}

int* colisiones(entidad_t* listaEntidades){

    for (int i = 1; &listaEntidades[i] != NULL ; ++i) {
        if((listaEntidades[0].fisica.posx + listaEntidades[0].fisica.ancho < listaEntidades[i].fisica.posx
        || listaEntidades[0].fisica.posx > listaEntidades[i].fisica.posx + listaEntidades[i].fisica.ancho)
        && (listaEntidades[0].fisica.posy + listaEntidades[0].fisica.alto < listaEntidades[i].fisica.posy
        || listaEntidades[0].fisica.posy > listaEntidades[i].fisica.posy + listaEntidades[i].fisica.alto)){

        }

    }

}