//
// Created by alvaro on 2/12/20.
//

#include "animacion.h"
#include <pthread.h>
#include "semaphore.h"
#include "gamelogic.h"
#include "time.h"

typedef struct {
    time_t tv_sec;
    time_t tv_nsec;
} tiempo_t ;

tiempo_t tiempo;

void * animar (void* gs){

    estadoJuego_t *gameState = (estadoJuego_t*) gs;
    static int segundos=0;
    time_t previous_time, current_time;

    while (gameState->state != GAMECLOSED) {

        if (gameState->entidades.jugador.estado == ALMOST_DEAD) {

            if (gameState->entidades.jugador.animation_counter == -1) {
                clock_gettime(CLOCK_REALTIME,&tiempo);
                previous_time = tiempo.tv_nsec;
                gameState->entidades.jugador.angle=0;
                segundos=0;
                (gameState->entidades.jugador.animation_counter)++;
            }
            if (gameState->entidades.jugador.animation_counter < 100) {
                clock_gettime(CLOCK_REALTIME,&tiempo);
                current_time = tiempo.tv_nsec;
                if(current_time < previous_time) {
                    segundos++;
                    current_time = current_time + 1000000000 * segundos;
                }
                if ((current_time - previous_time) >= 10000000) {
                    (gameState->entidades.jugador.angle) += 1.5*3.1416 / 20;
                    clock_gettime(CLOCK_REALTIME,&tiempo);
                    previous_time = tiempo.tv_nsec;
                    (gameState->entidades.jugador.animation_counter)++;
                }
            }
            if (gameState->entidades.jugador.animation_counter == 100) {
                gameState->entidades.jugador.estado = DEAD;
                gameState->entidades.jugador.animation_counter = -1;
                gameState->entidades.jugador.angle=0;
            }
        }








        /*


        int i = 0;

        fisica_t jugador = gameState->entidades.jugador.fisica;

        bloque_t bloque;

        while(gameState->entidades.bloques[i].identificador != NULLENTITIE){
            bloque = gameState->entidades.bloques[i];
            switch (bloque.identificador){
                case ALGA:

                    break;

                case LADRILLO:
                    for (int j = 0; j < bloque.fisica.ancho/PIXELSPERUNIT; ++j) {

                    }
                    break;
            }
            i++;
        }

        i=0;
        while(gameState->entidades.enemigos[i].identificador != NULLENTITIE){
            enemigo_t enemigo = gameState->entidades.enemigos[i];
            switch (enemigo.identificador) {
                case PULPITO:

                    break;

                case FASTCHEEPCHEEP:
                    if(enemigo.sprite==4)
                        enemigo.sprite==3;
                    else
                        enemigo.sprite==4;
                    break;
            }
        i++;
        }

        */;

    }
}