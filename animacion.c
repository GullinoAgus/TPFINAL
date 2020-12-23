//
// Created by alvaro on 2/12/20.
//

#include "animacion.h"
#include <pthread.h>
#include "semaphore.h"
#include "gamelogic.h"
#include <time.h>

struct timespec tiempo;

void * animar (void* gs){

    estadoJuego_t *gameState = (estadoJuego_t*) gs;
    static int segundos=0;
    struct timespec previous_time, current_time;

    gameState->entidades.jugador.animation_counter = -1;

    while (gameState->state != GAMECLOSED) {

        static int aux = 0;
        if(aux > 10000000){
            clock_gettime(CLOCK_REALTIME, &tiempo);
            printf("%ld\n", tiempo.tv_sec);
            aux = 0;
        }
        else{
            aux++;
        }

        while(gameState->state == PAUSE);


        if (gameState->entidades.jugador.estado == ALMOSTDEAD) {

            if (gameState->entidades.jugador.animation_counter == -1) {
                clock_gettime(CLOCK_REALTIME, &tiempo);
                previous_time.tv_nsec = tiempo.tv_nsec;
                gameState->entidades.jugador.angleRotation=0;
                segundos=0;
                (gameState->entidades.jugador.animation_counter)++;
            }
            if (gameState->entidades.jugador.animation_counter < 100) {
                clock_gettime(CLOCK_REALTIME, &tiempo);
                current_time.tv_nsec = tiempo.tv_nsec;
                if(current_time.tv_nsec < previous_time.tv_nsec) {
                    segundos++;
                    //current_time = current_time + 1000000000 * segundos;
                }
                /*if ((current_time - previous_time) >= 10000000) {
                    (gameState->entidades.jugador.angleRotation) += 1.5 * 3.1416 / 20;
                    clock_gettime(CLOCK_REALTIME, &tiempo);
                    previous_time.tv_nsec = tiempo.tv_nsec;
                    (gameState->entidades.jugador.animation_counter)++;
                }*/
            }
            if (gameState->entidades.jugador.animation_counter == 100) {
                gameState->entidades.jugador.estado = DEAD;
                gameState->entidades.jugador.animation_counter = -1;
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