//
// Created by damian on 30/11/20.
//

#include <pthread.h>
#include "render.h"
#include "configuracion.h"
#include "menu.h"
#include "level.h"
#include "semaphore.h"

extern sem_t semGlInMenu;
extern sem_t semRender;
extern sem_t semFisica;
extern char nivelInicializado;

//Si el juego debe renderizarse en la pantalla de la computadora
#if MODOJUEGO == 0

void *render (void *gs) {

    ALLEGRO_DISPLAY* disp;
    estadoJuego_t *gameState = (estadoJuego_t *) gs;
    int salida = 0;

    if (sem_init(&semFisica, 0, 0) != 0){
        printf("Error al inicializar el semaforo semFisica");
    }

    disp = al_create_display(SCREENWIDHT, SCREENHEIGHT);

    while (gameState->state != GAMECLOSED) {

        sem_wait(&semRender);

        switch (gameState->state) {

            case MENU: //menu
                salida = drawMenu(gameState);
                break;

            case LEVELSELECTOR: //seleccion de nivel


                break;

            case SCORETABLE: //tabla de scores


                break;

            case INGAME: //en juego
                drawLevel(gameState);
                break;
            }

        if (!nivelInicializado) {
            sem_post(&semGlInMenu);
        }
        else{
            sem_post(&semFisica);
        }
    }

    pthread_exit(NULL);

}

#elif MODOJUEGO == 1



#endif