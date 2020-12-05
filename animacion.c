//
// Created by alvaro on 2/12/20.
//

#include "animacion.h"
#include <pthread.h>
#include "semaphore.h"

extern sem_t semAnimaciones;
extern sem_t semGlInGame;

void * animar (void* gs){

    estadoJuego_t *gameState = (estadoJuego_t*) gs;
    int a =0;

    while (gameState->state != GAMECLOSED) {
        sem_wait(&semAnimaciones);

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

                case CHEEPCHEEP:
                    if(enemigo.sprite==4)
                        enemigo.sprite==3;
                    else
                        enemigo.sprite==4;
                    break;
            }
        i++;
        }

        sem_post(&semGlInGame);

    }

    //TODO: Quizas falte un exit aca para este thread

}