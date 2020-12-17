//
// Created by damian on 30/11/20.
//

#include <pthread.h>
#include "render.h"
#include "configuracion.h"
#include "menu.h"
#include "level.h"
#include "semaphore.h"
#include "gamelogic.h"


//Si el juego debe renderizarse en la pantalla de la computadora
#if MODOJUEGO == 0

void *render (void *gs) {

    ALLEGRO_DISPLAY* disp;
    estadoJuego_t *gameState = (estadoJuego_t *) gs;
    int salida = 0;

    if (sem_init(getPhysicsSem(), 0, 0) != 0){
        printf("Error al inicializar el semaforo semFisica");
        exit(1);
    }

    disp = al_create_display(SCREENWIDHT, SCREENHEIGHT);

    while (gameState->state != GAMECLOSED) {

        usleep(UTIEMPOREFRESCO);

        sem_wait(getRenderSem());
        //printf("Render\n");

        switch (gameState->state) {

            case MENU: //menu
                salida = drawMenu(gameState);
                break;

            case CHOOSINGLEVEL: //seleccion de nivel

                break;

            case INSCORETABLE: //tabla de scores
                drawTopScores(gameState, &(gameState->buffer) );
                break;

            case INGAME: //en juego
                //drawUI(gameState); //FIXME: Si ponemos esto asi anda re lento el juego :v
                drawLevel(gameState);
                break;
            }


        if (!wasLevelInitialized()) {
            sem_post(getGameLogicSem());
        }
        else{
            sem_post(getPhysicsSem());
        }
    }

    pthread_exit(NULL);

}

#elif MODOJUEGO == 1

void *render (void *gs) {

    estadoJuego_t *gameState = (estadoJuego_t *) gs;

    if (sem_init(getPhysicsSem(), 0, 0) != 0){
        printf("Error al inicializar el semaforo semFisica");
        exit(1);
    }

    while (gameState->state != GAMECLOSED) {

        int salida;

        usleep(UTIEMPOREFRESCO);

        sem_wait(getRenderSem());

        switch (gameState->state) {

            case MENU: //menu

                salida = drawMenu(gameState);
                break;

            case CHOOSINGLEVEL: //seleccion de nivel


                break;

            case INSCORETABLE: //tabla de scores

                imprimirHighScore(gameState->bestScores[0]);
                break;

            case INGAME: //en juego

                //drawLevel(gameState);
                break;
            }


        if (!wasLevelInitialized()) {
            sem_post(getGameLogicSem());
        }
        else{
            sem_post(getPhysicsSem());
        }
    }

    pthread_exit(NULL);


}

#endif