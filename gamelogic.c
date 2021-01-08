//
// Created by damian on 30/11/20.
//

#include <pthread.h>
#include "gamelogic.h"
#include "IEvents.h"
#include "menu.h"
#include "animacion.h"
#include "times.h"

//Variable que indica si hay un nivel inicializado
static char nivelInicializado = 0;  //0 si el juego no comenzo y 1 si el juego ya comenzo

static void startInGameThreads(pthread_t *fisicas, pthread_t *animaciones, estadoJuego_t *gameState);
static void finishInGameThreads(pthread_t *fisicas, pthread_t *animaciones);
static void decreaseGameTime(void* gameState);

/*
 * gameLogic: el thread recibe un puntero void al gameState y se encarga de observar el estado del juego para cargar y borrar la informacion necesaria para el cambio de escenas
 */

void *gamelogic (void *p2GameState) {

    pthread_t fisicas, animaciones;                             //Declararmos lo threads de fisicas y animaciones
    estadoJuego_t *gameState = (estadoJuego_t *) p2GameState;
    char evento = 0;                                            //Evento leido del buffer de eventos

    gameState->state = MENU;                    //Inicializamos el estado del juego en el menu
    gameState->menuSelection = LEVELSELECTOR;        //Inicializamos el estado del juego en el menu
    initUI(&gameState->gameUI);
    setCurrentGameState(gameState);

    if(loadMenuData() == 1){
        printf("Error al cargar la data del menu");
        pthread_exit(1);
    }

    createNewTimer(1.0f, decreaseGameTime, INGAMETIMER);

    while (gameState->state != GAMECLOSED) {

        if ( !(evento == DOWNABAJOIZQUIERDA || evento == DOWNARRIBAIZQUIERDA || evento == DOWNARRIBAIZQUIERDA || evento == DOWNARRIBADERECHA) ){
            evento = VACIO; //Tengo que hacer esto porque sino detecta siempre el mismo evento aunque no se aprete nada
                            //De momento esto creo que funciona, porque yo a ultimoevento lo uso solo para el joystick
        }

        if (!esBufferVacio()) {
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

                        case EXITGAME:
                            gameState->state = GAMECLOSED;
                            break;
                    }

                } else if ((evento == DOWNARRIBA) || (evento == DOWNABAJO)) {
                    updateMenuArrow(&gameState->menuSelection, evento);
                }
                break;

            case CHOOSINGLEVEL: //seleccion de nivel
                if (evento == DOWNDERECHA) {
                    gameState->gameUI.level++;
                }
                else if(evento == DOWNIZQUIERDA){
                    if(gameState->gameUI.level > ONE) {
                        gameState->gameUI.level--;
                    }
                }
                else if(evento == DOWNBOTON){
                    gameState->state = INGAME;
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
                    setCameraScrollX(0);
                    cargarMapa(&(gameState->level), gameState->gameUI.level);
                    initEntities(gameState);
                    setClosestPlayer(&(gameState->entidades.jugador));
                    startTimer(INGAMETIMER);
                    nivelInicializado = 1;
                    startInGameThreads(&fisicas, &animaciones, gameState);

                    /*
                    static int chQuant = 0;
                    static int blQuant = 0;
                    for(int i = 0; gameState->entidades.enemigos[i].identificador != NULLENTITIE; i++){
                        if(gameState->entidades.enemigos[i].identificador == FASTCHEEPCHEEP || gameState->entidades.enemigos[i].identificador == SLOWCHEEPCHEEP){
                            chQuant++;
                        }
                        if(gameState->entidades.enemigos[i].identificador == PULPITO){
                            blQuant++;
                        }
                    }
                    printf("CheepCheeps: %d\n", chQuant);
                    printf("Bloopers: %d\n", blQuant);
                    chQuant = 0;
                    blQuant = 0;
                     */
                }

                if(gameState->gameUI.time <= 0){
                    gameState->entidades.jugador.estado = DEAD;
                }

                if (gameState->entidades.jugador.estado == DEAD) {

                    gameState->entidades.jugador.vidas--;                   //Perdio una vida
                    stopTimer(INGAMETIMER);

                    if(gameState->entidades.jugador.vidas > 0){
                        gameState->state = RETRYSCREEN;
                        resetEntitiesPosition(gameState);       //FIXME: La posicion de las monedas no se resetean bien, el jugador a veces queda muerto al reiniciar un nivel sin causa aparente
                        setCameraScrollX(0);                //FIXME: A veces se crashea con el exit del menu
                    }
                    else{
                        finishInGameThreads(&fisicas, &animaciones);
                        gameState->gameUI.time = MAXLEVELTIME;
                        gameState->menuSelection = LEVELSELECTOR;
                        gameState->state = MENU;
                        resetEntitiesPosition(gameState);
                        destroyEntities(gameState);
                        nivelInicializado = 0;
                    }

                    stopInGameAnimations();
                }

                if(evento == DOWNESCAPE || evento == DOWNP){
                    gameState->state = PAUSE;
                    gameState->pauseSelection = 0;
                }

                movePlayer(evento, &gameState->entidades.jugador);

                break;

            case PAUSE:

                switch(evento){

                    case DOWNESCAPE:
                    case DOWNP:
                        gameState->state = INGAME;
                        break;
                    case DOWNBOTON:
                        switch (gameState->pauseSelection) {
                            case RESUME:
                                gameState->state = INGAME;
                                break;
                            case BACKTOMENU:
                                finishInGameThreads(&fisicas, &animaciones);
                                for(int i = 0; gameState->entidades.enemigos[i].identificador != NULLENTITIE; i++){
                                    gameState->entidades.enemigos[i].estado = DEAD;
                                    pthread_cancel(gameState->entidades.enemigos[i].enemyIA);
                                }
                                gameState->state = MENU;
                                gameState->menuSelection = LEVELSELECTOR;
                                stopTimer(INGAMETIMER);
                                gameState->gameUI.time = MAXLEVELTIME;
                                nivelInicializado = 0;
                                break;
                        }
                    break;

                    case DOWNARRIBA:
                    case DOWNABAJO:
                        updatePauseArrow(&gameState->pauseSelection, evento);
                    break;
                }

                break;

            case NEXTLEVEL:

                gameState->gameUI.level++;
                nivelInicializado = 0;
                sleep(1);
                stopTimer(INGAMETIMER);
                gameState->gameUI.time = MAXLEVELTIME;
                gameState->state = INGAME;
                break;
        }
    }

    pthread_exit(NULL);
}

static void decreaseGameTime(void* gameState){
    estadoJuego_t *gs = gameState;
    gs->gameUI.time--;
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