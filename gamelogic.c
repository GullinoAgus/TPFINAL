//
// Created by damian on 30/11/20.
//

#include <pthread.h>
#include "gamelogic.h"
#include "IEvents.h"
#include "menu.h"
#include "animacion.h"


static void startInGameThreads(pthread_t *fisicas, pthread_t *animaciones, estadoJuego_t *gameState);
static void finishInGameThreads(pthread_t *fisicas, pthread_t *animaciones);

void *gamelogic (void *p2GameState) {

    pthread_t fisicas, animaciones;
    estadoJuego_t *gameState = (estadoJuego_t *) p2GameState;
    char evento = 0;
    char ultimoEvento = 0;
    int nivelInicializado = 0;

    gameState->state = MENU;
    gameState->threadTurn = GAMELOGIC;
    gameState->menuSelection = PLAYGAME;

    while (gameState->state != GAMECLOSED) {
        if (gameState->threadTurn == GAMELOGIC) {


            if (!esBufferVacio()) {
                ultimoEvento = evento;
                evento = getInputEvent();
            }

            switch (gameState->state) {
                case MENU:
                    if (evento == DOWNBOTON) {
                        switch (gameState->menuSelection) {
                            case PLAYGAME:
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

                case SCORETABLE: //tabla de scores

                    if (evento == DOWNBOTON) {
                        gameState->menuSelection = 0;
                    }
                    break;

                case INGAME: //en juego

                    if(!nivelInicializado){
                        startInGameThreads(&fisicas, &animaciones, gameState);
                        nivelInicializado = 1;
                    }

                    if(gameState->entidades.jugador.estado == DEAD){
                        finishInGameThreads(&fisicas, &animaciones);
                        nivelInicializado = 0;
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
                            if (ultimoEvento != DOWNARRIBA) {
                                gameState->entidades.jugador.fisica.vely = -1.5f;
                            }
                            break;

                        case DOWNABAJO:
                            gameState->entidades.jugador.fisica.vely = 1.0f;
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

            gameState->threadTurn = RENDER; //Pasamos al siguiente thread
        }

        }

    pthread_join(fisicas, NULL);
    pthread_join(animaciones, NULL);
    pthread_exit(NULL);
}

static void startInGameThreads(pthread_t *fisicas, pthread_t *animaciones, estadoJuego_t *gameState){
    pthread_create(fisicas, NULL, fisica, &gameState);
    pthread_create(animaciones, NULL, animar, &gameState);
}

static void finishInGameThreads(pthread_t *fisicas, pthread_t *animaciones){
    pthread_cancel(*fisicas);
    pthread_cancel(*animaciones);
}