//
// Created by agus on 27/11/20.
//

#include "mapa.h"
#include <stdio.h>
#include <stdlib.h>

#define LECTURAVALIDA(x) ((x) == JUGADOR || (x) == BORDE || (x) == NADA || (x) == LADRILLO || (x) == ALGA || (x) == CHEEPCHEEP || (x) == PULPITO)

int* leerNivel(const char* direccion){

    int* mapa;
    int cantBloques = verificarTamanioDeNivel(direccion);
    int contadorChequeo = 0;

    if (cantBloques <= 0){
        return NULL;
    }else {
        mapa = calloc(cantBloques, sizeof(int));
        FILE* nivel = fopen(direccion,"r");
        int lectura;

        do {
            lectura = fgetc(nivel);
            if (LECTURAVALIDA(lectura)){
                *mapa++ = lectura;
                contadorChequeo++;
            }
        }while (lectura != EOF);
        if (cantBloques != contadorChequeo){
            free(mapa);
            mapa = NULL;
        }
        fclose(nivel);
    }
    return (mapa - cantBloques);
}

int verificarTamanioDeNivel(const  char* direccion){

    FILE* nivel = fopen(direccion, "r");
    if (nivel == NULL){
        fclose(nivel);
        return 0;
    }
    char car = 0;
    int celdaVacia = 0;
    int contadorCar = 0;
    int contadorLineas = 0;
    int contadorBordes = 0;
    int hayJugador = 0;
    do {
        car = fgetc(nivel);
        celdaVacia++;
        if (car != ';' && car != '\n' && car != EOF){
            celdaVacia = 0;
            contadorCar++;
            if (car == JUGADOR){
                hayJugador++;
            }else if (car == BORDE){
                contadorBordes++;
            }
        }
        if (celdaVacia == 2){
            celdaVacia = 1;
            contadorCar++;
        }
        if (car == '\n' || car == EOF){
            contadorLineas++;
        }
    }while (car != EOF);
    if (contadorLineas != 16){
        contadorCar = 0;
    } else if( hayJugador != 1){
        contadorCar = -1;
    } else if (((contadorBordes/2) != contadorLineas) || contadorBordes%2 !=0) {
        contadorCar = -2;
    }
    fclose(nivel);
    return contadorCar;

}

