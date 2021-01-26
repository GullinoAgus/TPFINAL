/***************************************************************************//**
  @file     gamelogic.c
  @brief    Uno de los threads principales donde se maneja la logica del videojuego
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <pthread.h>
#include <zconf.h>
#include "gamelogic.h"
#include "IEvents.h"
#include "menu.h"
#include "animacion.h"
#include "times.h"
#include "render.h"
#include "level.h"
#include "audio.h"

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void startInGameThreads(pthread_t *fisicas, pthread_t *animaciones, estadoJuego_t *gameState);
static void finishInGameThreads(pthread_t *fisicas, pthread_t *animaciones);
static void decreaseGameTime(void* gameState);
static void* endLevelInfo(void* pointer);
static void clearEntities(estadoJuego_t* gs);

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

//Variable que indica si hay un nivel inicializado
static char nivelInicializado = 0;  //0 si el juego no comenzo y 1 si el juego ya comenzo

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void *gamelogic (void *p2GameState) {

    pthread_t fisicas, animaciones, endThread;                           //Declaramos los threads de fisicas y animaciones
    estadoJuego_t *gameState = (estadoJuego_t *) p2GameState;
    unsigned char evento = 0;                                            //Evento leido del buffer de eventos
    int livesRecord = 0, numberOfLetter = 0, nombreLleno = 0, powerUpstateRecord = 0, lastGameState = -1;

    gameState->state = MENU;                            //Inicializamos el estado del juego en el menu
    gameState->menuSelection = LEVELSELECTOR;           //Inicializamos la seleccion del menu en la primera opcion
    gameState->entidades.jugador.vidas = 0;
    initUI(&gameState->gameUI);
    setCurrentGameState(gameState);
    resetWavePosition();

    createNewTimer(1.0f, decreaseGameTime, INGAMETIMER);
    lastGameState = gameState->state;

    playMusicFromMemory(gameState->buffer.sound[SUPERMARIOTHEME], gameState->buffer.sound[SUPERMARIOTHEME]->volume);

    while (gameState->state != GAMECLOSED) {

        evento = getInputEvent();  //Aqui se recibe el siguiente evento del buffer

        if(lastGameState != gameState->state){  //Cuando cambiamos el estado del juego limpiamos el buffer
            lastGameState = gameState->state;
            limpiarBuffer();
        }

        switch (gameState->state) {
            case MENU:
                if (evento == DOWNBOTON) {  //Si se presiono el espacio, o el boton de la raspi decidimos adonde ir
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

            case CHOOSINGLEVEL: //Seleccion de nivel
                if (evento == DOWNDERECHA && gameState->gameUI.level < MAXLEVELAVAILABLE) {
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

            case INSCORETABLE: //Tabla de scores
                if (evento == DOWNBOTON) {  //Salimos de la tabla de escores al apretar el espacio o el boton de la raspi
                    usleep(100000);
                    limpiarBuffer();
                    gameState->state = MENU;
                }
                break;

            case INGAME: //en juego

                if (!nivelInicializado) {

                    if(gameState->entidades.jugador.vidas != 0){
                        livesRecord = gameState->entidades.jugador.vidas;
                        powerUpstateRecord = gameState->entidades.jugador.powerUpsState;
                    }

                    setCameraScrollX(0);
                    cargarMapa(&(gameState->level), gameState->gameUI.level);
                    initEntities(gameState);
                    if(gameState->gameUI.score == 0) {
                        gameState->entidades.jugador.vidas = MAXLIVES;
                    }
                    else{
                        gameState->entidades.jugador.vidas = livesRecord;
                        gameState->entidades.jugador.powerUpsState = powerUpstateRecord;
                        if(powerUpstateRecord == SMALL) {
                            gameState->entidades.jugador.fisica.alto = PIXELSPERUNIT;
                        }
                        else {
                            gameState->entidades.jugador.fisica.alto = PIXELSPERUNIT*2;
                        }
                    }
                    setClosestPlayer(&(gameState->entidades.jugador));
                    startInGameThreads(&fisicas, &animaciones, gameState);
                    startTimer(INGAMETIMER);
                    nivelInicializado = 1;
                    playMusicFromMemory(gameState->buffer.sound[UNDERWATERTHEME], gameState->buffer.sound[UNDERWATERTHEME]->volume);
                }

                if(gameState->gameUI.time <= 0){
                    gameState->entidades.jugador.estado = DEAD;
                }

                if (gameState->entidades.jugador.estado == DEAD) {

                    gameState->entidades.jugador.vidas--;                   //Perdio una vida

                    stopTimer(INGAMETIMER);
                    stopTimer(PHYSICSTIMER);

                    resetEntitiesState(gameState);
                    resetWavePosition();

                    if(gameState->entidades.jugador.vidas > 0){
                        gameState->state = RETRYSCREEN;
                        nivelInicializado = 0;
                        setCameraScrollX(0);
                        nivelInicializado = 1;
                    }
                    else{
                        nivelInicializado = 0;
                        gameState->state = GAMEOVERSCREEN;
                        finishInGameThreads(&fisicas, &animaciones);
                        pthread_create(&endThread, NULL, endLevelInfo, gameState);
                        playSoundFromMemory(gameState->buffer.sound[GAMEOVERSOUND], gameState->buffer.sound[GAMEOVERSOUND]->volume);
                    }

                }

                if(gameState->entidades.jugador.estado != DEAD && gameState->entidades.jugador.estado != ALMOSTDEAD) {

                    if(evento == DOWNESCAPE || evento == DOWNP){
                        stopTimer(INGAMETIMER);
                        stopTimer(PHYSICSTIMER);
                        gameState->pauseSelection = 0;
                        gameState->state = PAUSE;
                        playSoundFromMemory(gameState->buffer.sound[PAUSEGAME], gameState->buffer.sound[PAUSEGAME]->volume);
                    }

                    if(evento == DOWNARRIBA){
                        playSoundFromMemory(gameState->buffer.sound[JUMPSMALL], gameState->buffer.sound[JUMPSMALL]->volume);
                    }
                    movePlayer(evento, &gameState->entidades.jugador);
                }

                break;

            case PAUSE:

                switch(evento){

                    case DOWNESCAPE:
                    case DOWNP:
                        startTimer(INGAMETIMER);
                        startTimer(PHYSICSTIMER);
                        gameState->state = INGAME;
                        break;
                    case DOWNBOTON:
                        switch (gameState->pauseSelection) {
                            case RESUME:
                                startTimer(INGAMETIMER);
                                startTimer(PHYSICSTIMER);
                                gameState->state = INGAME;
                                break;

                            case BACKTOMENU:

                                nivelInicializado = 0;
                                gameState->entidades.jugador.estado = DEAD;

                                finishInGameThreads(&fisicas, &animaciones);
                                clearEntities(gameState);

                                initUI(&gameState->gameUI);
                                gameState->menuSelection = LEVELSELECTOR;
                                gameState->state = MENU;
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

                nivelInicializado = 0;
                gameState->gameUI.score += gameState->gameUI.time;
                gameState->gameUI.level++;
                gameState->gameUI.time = MAXLEVELTIME;
                stopTimer(PHYSICSTIMER);

                finishInGameThreads(&fisicas, &animaciones);
                clearEntities(gameState);

                sleep(1);
                gameState->state = INGAME;

                break;

            case RETRYSCREEN:
                sleep(2);
                gameState->state = INGAME;
                gameState->gameUI.time = MAXLEVELTIME;
                startTimer(PHYSICSTIMER);
                startTimer(INGAMETIMER);
                playMusicFromMemory(gameState->buffer.sound[UNDERWATERTHEME], gameState->buffer.sound[UNDERWATERTHEME]->volume);
                break;

            case GAMEOVERSCREEN:

                nivelInicializado = 0;

                #if MODOJUEGO == 0

                if(wasNewHighScoreAchieved(gameState)) {
                    if ((evento >= DOWNA) && (evento <= UP9) && ((evento - DOWNA) % 2 == 0)) {   //FIXME Habria que poner un switch case, mas piola
                        *((gameState->pPlayerName) + numberOfLetter) = (evento - DOWNA) / 2 + 'A';

                        if (numberOfLetter <= MAXPLAYERNAME - 2) {
                            numberOfLetter++;
                        } else {
                            nombreLleno = 1;
                        }
                    }

                    if ((evento == DOWNBACKSPACE) && (numberOfLetter > 0)) {
                        if (nombreLleno == 0) {
                            *((gameState->pPlayerName) + numberOfLetter - 1) = '\0';
                            numberOfLetter--;
                        } else {
                            nombreLleno = 0;
                            *((gameState->pPlayerName) + numberOfLetter) = '\0';
                        }
                    }


                    if (evento == UPENTER && numberOfLetter > 0) {
                        saveNewHighScore(gameState);
                        initUI(&gameState->gameUI);
                        resetLastBlockInMap();
                        numberOfLetter = 0;
                        nombreLleno = 0;
                        gameState->menuSelection = LEVELSELECTOR;
                        gameState->state = MENU;

                        for (int i = 0; i < MAXPLAYERNAME; i++) {
                            *((gameState->pPlayerName) + i) = '\0';
                        }
                    }
                }
                else{
                    initUI(&gameState->gameUI);
                    resetLastBlockInMap();
                    sleep(4);
                    gameState->menuSelection = LEVELSELECTOR;
                    gameState->state = MENU;
                }
                break;

                #elif MODOJUEGO == 1

                if(wasNewHighScoreAchieved(gameState)) {
                    gameState->pPlayerName = "Raspberry";
                    saveNewHighScore(gameState);
                    sleep(4);
                    initUI(&gameState->gameUI);
                    gameState->menuSelection = LEVELSELECTOR;
                    gameState->state = MENU;

                }
                else{
                    sleep(4);
                    initUI(&gameState->gameUI);
                    gameState->menuSelection = LEVELSELECTOR;
                    gameState->state = MENU;
                }

                break;

                #endif
        }
    }

    pthread_exit(NULL);
}

char wasLevelInitialized(){
    return nivelInicializado;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void decreaseGameTime(void* gameState){
    estadoJuego_t *gs = gameState;
    gs->gameUI.time--;
}

static void startInGameThreads(pthread_t *fisicas, pthread_t *animaciones, estadoJuego_t *gameState){
    pthread_create(fisicas, NULL, fisica, gameState);
    pthread_create(animaciones, NULL, animar, gameState);
}

static void finishInGameThreads(pthread_t *fisicas, pthread_t *animaciones){
    pthread_cancel(*fisicas);
    pthread_cancel(*animaciones);
}

static void* endLevelInfo(void* gs){

    pthread_detach(pthread_self());

    estadoJuego_t* gameState = gs;

    for(int i = 0; gameState->entidades.enemigos[i].identificador != NULLENTITIE && gameState->entidades.enemigos[i].estado == ALIVE; i++){
        destroyEnemyIA(&gameState->entidades.enemigos[i]);
    }

    destroyEntities(gameState);
    destroyMap(gameState);
    return NULL;
}

static void clearEntities(estadoJuego_t* gameState){

    for(int i = 0; gameState->entidades.enemigos[i].identificador != NULLENTITIE && gameState->entidades.enemigos[i].estado == ALIVE; i++){
        destroyEnemyIA(&gameState->entidades.enemigos[i]);
    }
    resetEntitiesState(gameState);
    resetWavePosition();
    resetLastBlockInMap();
    destroyMap(gameState);
    destroyEntities(gameState);
}