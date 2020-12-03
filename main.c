#include "matiasBrosGame.h"
#include "IEvents.h"
#include "allegroLib.h"
#include "menu.h"
#include <unistd.h>
#include <pthread.h>
#include "level.h"
#include "gamelogic.h"
#include "Animaciones.h"

#if MODOJUEGO == 0

int main(void) {

    int closed = 0;
    ALLEGRO_DISPLAY* disp;
    bufferRecursos_t resourcesBuffer;   //TODO: Refactor de esto
    estadoJuego_t gameState;
    pthread_t eventoTeclado, fisicas, gameLogic, animaciones, render;

    gameState.buffer = resourcesBuffer;

    entidades_t entidades;

    gameState.state = 3;  //comenzamos en el menu

    //Inicializamos allegro, los recursos del juego y verificamos que se haya hecho correctamente
    if(inicializarAllegro(disp) == 1) {
        al_destroy_display(disp);
        return 1;
    }

    resourcesBuffer.imageQuant = cargarTexturasMenu(&resourcesBuffer.image);
    if(resourcesBuffer.imageQuant == -1) {
        destroyResources(&resourcesBuffer);
        al_destroy_display(disp);
        return -1;
    }

    resourcesBuffer.soundQuant = cargarSonidosMenu(&resourcesBuffer.sound);
    if(resourcesBuffer.soundQuant == -1) {
        destroyResources(&resourcesBuffer);
        al_destroy_display(disp);
        return -1;
    }

    resourcesBuffer.fontQuant = cargarFuentesMenu(&resourcesBuffer.font);
    if(resourcesBuffer.fontQuant == -1){
        destroyResources(&resourcesBuffer);
        al_destroy_display(disp);
        return -1;
    }

    if(loadGameState(&gameState) == 1) {
        destroyResources(&resourcesBuffer);
        al_destroy_display(disp);
        return 1;
    }

    gameState.threadTurn = -1; //Para que no se inicie ningun thread hasta que este cargado


    pthread_create(&eventoTeclado, NULL, keyboardChanges, &gameState);
    //pthread_create(&animaciones, NULL, animar, &estructuras);
    pthread_create(&render, NULL, renderizar, &gameState);
    pthread_create(&gameLogic, NULL, gamelogic, &gameState);

    drawMenu(&resourcesBuffer); //Dibujamos el menu
    gameState.threadTurn = INPUT;

    /*
    while(!closed) {

        while(esBufferVacio());
        char evento = getInputEvent();

        updateMenu(&gameState.menuSelection, evento);

        if(evento == DOWNBOTON) {
            closed = 1;
        }
    }
    */

    //Cargamos los datos del nivel
    cargarMapa(&gameState.level, ONE);

    if(initEntities(&gameState) == 1) {
        destroyResources(&resourcesBuffer);
        al_destroy_display(disp);
        return 1;
    }


    pthread_create(&fisicas, NULL, fisica, &gameState);

    /*
    //Comenzamos el movimiento de los bloopers      ///     TODO: Deberian inicializarse los threads cuando el juego haya comenzado mirando el estado de gameState.state
    int i = 0;
    int j = 0;
    int blopperCounter = 0;

    while(gameState.entidades.enemigos[i].identificador != NULLENTITIE) {
        if(gameState.entidades.enemigos[i].identificador == PULPITO) {
            blopperCounter++;
        }
        i++;
    }

    pthread_t *blopperEnemy;
    blopperEnemy = (pthread_t *) malloc(sizeof(pthread_t) * blopperCounter);
    if(blopperEnemy == NULL) return -1;

    i = 0;
    while(gameState.entidades.enemigos[i].identificador != NULLENTITIE) {
        if(gameState.entidades.enemigos[i].identificador == PULPITO) {
            pthread_create(&(blopperEnemy[j]), NULL, gameState.entidades.enemigos[i].funcionMovimiento, &gameState.entidades); //TODO: No olvidarse del pthread_join
            j++;
        }
        i++;
    }

    ////////////////////////////////////////////////////////*/

    while(gameState.state != GAMECLOSED) {
        if (!esBufferVacio()) {
            switch (getInputEvent()) {
                case DOWNIZQUIERDA:
                    gameState.entidades.jugador.fisica.velx = -1.0f;
                    break;
                case DOWNDERECHA:
                    gameState.entidades.jugador.fisica.velx = 1.0f;
                    break;
                case DOWNARRIBA:
                    gameState.entidades.jugador.fisica.vely = -1.0f;
                    break;
                case UPIZQUIERDA:
                    gameState.entidades.jugador.fisica.velx = 0.0f;
                    break;
                case DOWNBOTON:
                    gameState.state = GAMECLOSED;
                    break;
            }
        }

        drawLevel(&gameState, &resourcesBuffer);

    }


    destroyResources(&resourcesBuffer);
    destroyMenu();
    al_destroy_display(disp);

    //Esperamos a que terminen los threads
    pthread_join(fisicas, NULL);
    pthread_join(eventoTeclado, NULL);

    return 0;
}

#elif MODOJUEGO == 1

int main (void){

    disp_init();				//inicializa el display
    disp_clear();				//limpia el display
    disp_update();              //muestra en pantalla el display limpito

    joy_init();                 //inicializa el joystick
    estadoJuego_t gameState;

    pthread_t EventoJoy, fisicas, gameLogic;
    pthread_create(&EventoJoy, NULL, InputEvent, NULL);

    pthread_create(&fisicas, NULL, fisica, &gameState);
    pthread_create(&gameLogic, NULL, gamelogic, &gameState);

    pthread_join(EventoJoy, NULL);
    pthread_join(fisicas, NULL);
    pthread_join(gameLogic, NULL);

    disp_clear();
    disp_update();
}

#endif