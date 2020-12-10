//
// Created by damian on 30/11/20.
//

#include <pthread.h>
#include "gamelogic.h"
#include "IEvents.h"
#include "menu.h"
#include "animacion.h"
#include "semaphore.h"


static sem_t semGameLogic;
static sem_t semRender;
static sem_t semFisica;
static sem_t semAnimaciones;
static char nivelInicializado = 0;  //0 si el juego no comenzo y 1 si el juego ya comenzo

static void startInGameThreads(pthread_t *fisicas, pthread_t *animaciones, estadoJuego_t *gameState);
static void finishInGameThreads(pthread_t *fisicas, pthread_t *animaciones);

void *gamelogic (void *p2GameState) {

    pthread_t fisicas, animaciones;
    estadoJuego_t *gameState = (estadoJuego_t *) p2GameState;
    char evento = 0;
    char ultimoEvento = 0;
    int menuLoaded = 0;

    if (sem_init(&semGameLogic, 0, 1) != 0){
        printf("Error al inicializar el semaforo semGameLogic\n");
        exit(1);
    }

    if (sem_init(&semRender, 0, 0) != 0){
        printf("Error al inicializar el semaforo semRender\n");
        exit(1);
    }

    gameState->state = MENU;
    gameState->menuSelection = PLAYGAME;

    if(menuLoaded == 0){
        if(loadMenuData() == 1){
            printf("Error al cargar la data del menu");
        }
        else{
            menuLoaded = 1;
        }
    }

    while (gameState->state != GAMECLOSED) {

        usleep(UTIEMPOREFRESCO);

        sem_wait(&semGameLogic);

        //printf("Gamelogic\n");

        if (!esBufferVacio()) {
            ultimoEvento = evento;
            evento = getInputEvent();
        }

        switch (gameState->state) {
            case MENU:

                if (evento == DOWNBOTON) {
                    switch (gameState->menuSelection) {
                        case PLAYGAME:
                            cargarMapa(&gameState->level,ONE);
                            initEntities(gameState);
                            gameState->state = INGAME;
                            break;

                        case SCORETABLE:
                            gameState->state = INSCORETABLE;
                            break;

                        case LEVELSELECTOR:
                            gameState->state = CHOOSINGLEVEL;
                            break;
                    }

                } else if ((evento == DOWNARRIBA) || (evento == DOWNABAJO)) {
                    updateMenuArrow(&gameState->menuSelection, evento);
                }
                break;
            case CHOOSINGLEVEL: //seleccion de nivel

                break;

            case INSCORETABLE: //tabla de scores

                if (evento == DOWNBOTON) {      
                    gameState->state = MENU;
                }
                break;

            case INGAME: //en juego

                if (!nivelInicializado) {
                    startInGameThreads(&fisicas, &animaciones, gameState);
                    setClosestPlayer(&gameState->entidades.jugador);
                    nivelInicializado = 1;
                }

                if (gameState->entidades.jugador.estado == DEAD) {

                    finishInGameThreads(&fisicas, &animaciones);
                    for(int i = 0; gameState->entidades.enemigos[i].identificador != NULLENTITIE; i++){
                        gameState->entidades.enemigos[i].estado = DEAD;
                    }

                    gameState->state = MENU;
                    nivelInicializado = 0;
                }

                for(int i = 0; gameState->entidades.enemigos[i].identificador != NULLENTITIE; i++){
                    if(isColliding(&gameState->entidades.jugador.fisica, &gameState->entidades.enemigos[i].fisica)){
                        gameState->entidades.jugador.estado = DEAD;
                    }
                }

                switch (evento) {

                    case DOWNIZQUIERDA:
                        gameState->entidades.jugador.fisica.velx = -1.0f;
                        break;

                    case DOWNDERECHA:
                        gameState->entidades.jugador.fisica.velx = 1.0f;
                        break;

                    case UPIZQUIERDA:
                        gameState->entidades.jugador.fisica.velx = 0.0f;
                        break;

                    case UPDERECHA:
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

        sem_post(&semRender);
    }

    pthread_join(fisicas, NULL);
    pthread_join(animaciones, NULL);
    pthread_exit(NULL);
}


sem_t* getGameLogicSem(){
    return &semGameLogic;
}

sem_t* getPhysicsSem(){
    return &semFisica;
}

sem_t* getRenderSem(){
    return &semRender;
}

sem_t* getAnimationSem(){
    return &semAnimaciones;
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