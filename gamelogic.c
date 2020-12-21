//
// Created by damian on 30/11/20.
//

#include <pthread.h>
#include "gamelogic.h"
#include "IEvents.h"
#include "menu.h"
#include "animacion.h"
#include "semaphore.h"

//Variable que indica si hay un nivel inicializado
static char nivelInicializado = 0;  //0 si el juego no comenzo y 1 si el juego ya comenzo

static void startInGameThreads(pthread_t *fisicas, pthread_t *animaciones, estadoJuego_t *gameState);
static void finishInGameThreads(pthread_t *fisicas, pthread_t *animaciones);

/*
 * gameLogic: el thread recibe un puntero void al gameState y se encarga de observar el estado del juego para cargar y borrar la informacion necesaria para el cambio de escenas
 */

void *gamelogic (void *p2GameState) {

    pthread_t fisicas, animaciones;                             //Declararmos lo threads de fisicas y animaciones
    estadoJuego_t *gameState = (estadoJuego_t *) p2GameState;
    char evento = 0;                                            //Evento leido del buffer de eventos
    char ultimoEvento = 0;
    int menuLoaded = 0;             //Variable que indica si el menu fue cargado

    gameState->state = MENU;                    //Inicializamos el estado del juego en el menu
    gameState->menuSelection = LEVELSELECTOR;        //Inicializamos el estado del juego en el menu

    if(menuLoaded == 0){
        if(loadMenuData() == 1){
            printf("Error al cargar la data del menu");
        }
        else{
            menuLoaded = 1;
        }
    }

    while (gameState->state != GAMECLOSED) {

        if ( !(evento == DOWNABAJOIZQUIERDA || evento == DOWNARRIBAIZQUIERDA || evento == DOWNARRIBAIZQUIERDA || evento == DOWNARRIBADERECHA) ){
            evento = VACIO; //Tengo que hacer esto porque sino detecta siempre el mismo evento aunque no se aprete nada
                            //De momento esto creo que funciona, porque yo a ultimoevento lo uso solo para el joystick
        }

        if (!esBufferVacio()) {
            ultimoEvento = evento;
            evento = getInputEvent();
        }

        switch (gameState->state) {
            case MENU:

                if (evento == DOWNBOTON) {
                    switch (gameState->menuSelection) {
                        case LEVELSELECTOR:
                            gameState->state = CHOOSINGLEVEL;
                            break;

                        case SCORETABLE:
                            usleep(100000);
                            limpiarBuffer();
                            gameState->state = INSCORETABLE;
                            break;

                        case PLAYGAME:
                            cargarMapa(&gameState->level,ONE);
                            initEntities(gameState);
                            gameState->state = INGAME;
                            break;
                    }

                } else if ((evento == DOWNARRIBA) || (evento == DOWNABAJO)) {
                    updateMenuArrow(&gameState->menuSelection, evento);
                }
                break;

            case CHOOSINGLEVEL: //seleccion de nivel
                if (evento == DOWNBOTON) {
                    gameState->state = MENU;
                    gameState->menuSelection = PLAYGAME;
                }
                break;

            case INSCORETABLE: //tabla de scores

                if (evento == DOWNBOTON) {
                    usleep(100000);
                    limpiarBuffer();
                    gameState->state = MENU;
                }
                break;

            case INGAME: //en juego

                if (!nivelInicializado) {
                    startInGameThreads(&fisicas, &animaciones, gameState);
                    setClosestPlayer(&gameState->entidades.jugador);
                    initUI(&gameState->gameUI);
                    nivelInicializado = 1;
                }

                if (gameState->entidades.jugador.estado == DEAD) {
                    finishInGameThreads(&fisicas, &animaciones);
                    for(int i = 0; gameState->entidades.enemigos[i].identificador != NULLENTITIE; i++){
                        gameState->entidades.enemigos[i].estado = DEAD;
                    }

                    gameState->state = MENU;
                    gameState->menuSelection = LEVELSELECTOR;
                    nivelInicializado = 0;
                }

                switch (evento) {

                    case DOWNIZQUIERDA:
                        gameState->entidades.jugador.fisica.velx = -1.0f;
                        break;

                    case DOWNDERECHA:
                        gameState->entidades.jugador.fisica.velx = 1.0f;
                        break;

                    case UPDERECHA:
                    case UPIZQUIERDA:
                        gameState->entidades.jugador.fisica.velx = 0.0f;
                        break;

                    case DOWNARRIBA:
                        gameState->entidades.jugador.fisica.vely = -1.5f;
                        break;

                        // A continuacion tambien los del joystick, los cuales no se tiene acceso desde las flechitas
                    case DOWNARRIBADERECHA:
                        if (ultimoEvento != DOWNARRIBADERECHA) {
                            (gameState->entidades).jugador.fisica.vely = -1.0f;
                            (gameState->entidades).jugador.fisica.velx = 0.5f;
                        }
                        break;
                    case DOWNARRIBAIZQUIERDA:
                        if (ultimoEvento != DOWNARRIBAIZQUIERDA) {
                            (gameState->entidades).jugador.fisica.vely = -1.0f;
                            (gameState->entidades).jugador.fisica.velx = -0.5f;
                        }
                        break;
                    case DOWNABAJODERECHA:
                        if (ultimoEvento != DOWNABAJODERECHA) {
                            (gameState->entidades).jugador.fisica.vely = 1.0f;
                            (gameState->entidades).jugador.fisica.velx = 0.5f;
                        }
                        break;
                    case DOWNABAJOIZQUIERDA:
                        if (ultimoEvento != DOWNABAJOIZQUIERDA) {
                            (gameState->entidades).jugador.fisica.vely = 1.0f;
                            (gameState->entidades).jugador.fisica.velx = -0.5f;
                        }
                        break;
                }

                break;
        }
    }

    pthread_join(fisicas, NULL);
    pthread_join(animaciones, NULL);
    pthread_exit(NULL);
}


char wasLevelInitialized(){
    return nivelInicializado;
}

static void startInGameThreads(pthread_t *fisicas, pthread_t *animaciones, estadoJuego_t *gameState){
    pthread_create(fisicas, NULL, fisica, gameState);
    pthread_create(animaciones, NULL, animar, gameState);
}

static void finishInGameThreads(pthread_t *fisicas, pthread_t *animaciones){
    pthread_cancel(*fisicas);
    pthread_cancel(*animaciones);
}