//
// Created by agus on 23/11/20.
//


#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include "fisica.h"
#include "matiasBrosGame.h"



pthread_mutex_t lock1;

void* fisica(void* entrada){

    estadoJuego_t *gameState = entrada;
    pthread_mutex_init(&lock1,NULL );

    while(1) {
        usleep(UTIEMPOREFRESCO);
        pthread_mutex_lock(&lock1);
        if (entidades->jugador.fisica.velx > VELOCIDADXMAX){
            entidades->jugador.fisica.velx = VELOCIDADXMAX;
        }
        if (gameState->entidades.jugador.fisica.vely > VELOCIDADYMAX){
            entidades->jugador.fisica.vely = VELOCIDADYMAX;
        }
        entidades->jugador.fisica.vely =
                entidades->jugador.fisica.vely < VELOCIDADXMAX ? entidades->jugador.fisica.vely : VELOCIDADXMAX;
        entidades->jugador.fisica.velx =
                entidades->jugador.fisica.velx < VELOCIDADXMAX ? entidades->jugador.fisica.velx : VELOCIDADXMAX;
        /* ACTUALIZACION DE POSICIONES*/
        entidades->jugador.fisica.posx += entidades->jugador.fisica.velx;
        entidades->jugador.fisica.posy += entidades->jugador.fisica.vely;

        if(entidades->jugador.sobreBloque && entidades->jugador.fisica.vely != 0){
            entidades->jugador.sobreBloque = false;
        }

        for (int i = 0; entidades->enemigos[i] != NULL; ++i) {
            entidades->enemigos[i]->fisica.posx += entidades->enemigos[i]->fisica.velx;
            entidades->enemigos[i]->fisica.posy += entidades->enemigos[i]->fisica.vely;
        }

        entidades->jugador.fisica.vely += GRAVEDAD;

        /*if(a.max.x < b.min.x or a.min.x > b.max.x) return false;
        if(a.max.y < b.min.y or a.min.y > b.max.y) return false;*/
        for (int i = 0; entidades->enemigos[i] != NULL; ++i) {
            if ((entidades->jugador.fisica.posx + entidades->jugador.fisica.ancho) >
                entidades->enemigos[i]->fisica.posx &&
                entidades->jugador.fisica.posx <
                (entidades->enemigos[i]->fisica.posx + entidades->enemigos[i]->fisica.ancho) &&
                (entidades->jugador.fisica.posy + entidades->jugador.fisica.alto) >
                entidades->enemigos[i]->fisica.posy &&
                entidades->jugador.fisica.posy <
                (entidades->enemigos[i]->fisica.posy + entidades->enemigos[i]->fisica.alto)) {

            }
        }

        /* COLISIONES*/
        for (int i = 0; entidades->bloques[i] != NULL; ++i) {
            if ((entidades->jugador.fisica.posx + entidades->jugador.fisica.ancho) >
                entidades->bloques[i]->fisica.posx &&
                entidades->jugador.fisica.posx <
                (entidades->bloques[i]->fisica.posx + entidades->bloques[i]->fisica.ancho) &&
                (entidades->jugador.fisica.posy + entidades->jugador.fisica.alto) >
                entidades->bloques[i]->fisica.posy &&
                entidades->jugador.fisica.posy <
                (entidades->bloques[i]->fisica.posy + entidades->bloques[i]->fisica.alto)) {

                if ((entidades->jugador.fisica.posx + entidades->jugador.fisica.ancho - entidades->bloques[i]->fisica.posx <= VELOCIDADXMAX) !=
                    (VELOCIDADXMAX >= (entidades->bloques[i]->fisica.posx + entidades->bloques[i]->fisica.ancho) - entidades->jugador.fisica.posx)) {

                    if (entidades->jugador.fisica.posx < entidades->bloques[i]->fisica.posx) { //Choque por izquierda
                        entidades->jugador.fisica.posx =
                                entidades->bloques[i]->fisica.posx - entidades->jugador.fisica.ancho;
                    } else if ((entidades->jugador.fisica.posx - entidades->jugador.fisica.ancho <
                                entidades->bloques[i]->fisica.posx + entidades->bloques[i]->fisica.ancho)) {
                        entidades->jugador.fisica.posx =
                                entidades->bloques[i]->fisica.posx + entidades->bloques[i]->fisica.ancho;
                    }
                } else if (((entidades->jugador.fisica.posy + entidades->jugador.fisica.alto) >
                            entidades->bloques[i]->fisica.posy) !=
                           ((entidades->jugador.fisica.posy) >
                            (entidades->bloques[i]->fisica.posy + entidades->bloques[i]->fisica.alto))) {
                    entidades->jugador.fisica.vely = 0;
                    if (entidades->jugador.fisica.posy < entidades->bloques[i]->fisica.posy) { //las patas
                        entidades->jugador.fisica.posy =
                                entidades->bloques[i]->fisica.posy - entidades->jugador.fisica.alto;
                        entidades->jugador.sobreBloque = true;
                    } else {
                        entidades->jugador.fisica.posy =
                                entidades->bloques[i]->fisica.posy + entidades->bloques[i]->fisica.alto;
                    }
                }
            }
        }
        pthread_mutex_unlock(&lock1);
    }
}



