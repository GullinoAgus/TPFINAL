/***************************************************************************//**
  file     gamelogic.c
  @brief    Uno de los threads principales donde se maneja la logica del videojuego
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <pthread.h>
#include <zconf.h>
#include "gamelogic.h"

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void startInGameThreads(pthread_t *fisicas, pthread_t *animaciones, estadoJuego_t *gameState);
static void finishInGameThreads(const pthread_t *fisicas, const pthread_t *animaciones);
static void decreaseGameTime(void* gameState);
static void* finishLevel(void* gs);
static void clearEntities(estadoJuego_t* gameState);

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

//Variable que indica si hay un nivel inicializado
static char nivelInicializado = 0;  //0 si el juego no comenzo y 1 si el juego ya comenzo
static sem_t renderSem;
static sem_t fisicaSem;
static sem_t animacionSem;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void *gamelogic (void *p2GameState) {

    pthread_t fisicas, animaciones, endThread;                           //Declaramos los threads de fisicas y animaciones
    estadoJuego_t *gameState = (estadoJuego_t *) p2GameState;
    unsigned char evento = 0;                                            //Evento leido del buffer de eventos
    int livesRecord = 0, numberOfLetter = 0, nombreLleno = 0, powerUpstateRecord = 0, maxLevelsAvailable;
    estadosjuego_t lastGameState;

    maxLevelsAvailable = getMaxLevelsAvailable();
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

        evento = getInputEvent();                                   //Aqui se recibe el siguiente evento del buffer

        if(lastGameState != gameState->state){                      //Cuando cambiamos el estado del juego limpiamos el buffer
            lastGameState = gameState->state;
            limpiarBuffer();
        }

        switch (gameState->state) {
            case MENU: //Menu
                if (evento == DOWNBOTON) {                          //Si se presiono el espacio, o el boton de la raspi decidimos adonde ir
                    switch (gameState->menuSelection) {
                        case LEVELSELECTOR:
                            gameState->state = CHOOSINGLEVEL;
                            break;

                        case SCORETABLE:
                            usleep(100000);
                            gameState->state = INSCORETABLE;
                            break;

                        case EXITGAME:
                            gameState->state = GAMECLOSED;
                            break;
                    }

                } else if ((evento == DOWNARRIBA) || (evento == DOWNABAJO)) {
                    updateMenuArrow(&gameState->menuSelection, evento);     //Actualizamos la posicion del la flecha del menu
                }
                break;

            case CHOOSINGLEVEL: //Seleccion de nivel
                switch (evento) {
                    case DOWNDERECHA:
                        if (gameState->gameUI.level < maxLevelsAvailable){  //Si se apreto la tecla derecha y hay mas niveles disponibles
                            gameState->gameUI.level++;                      //Avanzamos al siguiente
                        }
                        break;
                    case DOWNIZQUIERDA:
                        if(gameState->gameUI.level > 1) {   //Si el nivel es menor a 1, no lo cambiamos
                            gameState->gameUI.level--;
                        }
                        break;
                    case DOWNBOTON:
                        gameState->state = INGAME;  //Si se apreto enter, comenzamos el nivel
                        break;
                    case DOWNESCAPE:
                        gameState->gameUI.level = 1;
                        gameState->state = MENU;
                        break;
                    default:
                        break;
                }
                break;

            case INSCORETABLE: //Tabla de scores
                if (evento == DOWNBOTON) {  //Salimos de la tabla de escores al apretar el espacio o el boton de la raspi
                    usleep(100000);
                    gameState->state = MENU;
                }
                break;

            case INGAME: //en juego

                if (!nivelInicializado) {

                    if(gameState->entidades.jugador.vidas != 0){        //Si venimos de otro nivel, mantenemos las vidas actuales con su powerUpState
                        livesRecord = gameState->entidades.jugador.vidas;
                        powerUpstateRecord = gameState->entidades.jugador.powerUpsState;
                    }

                    setCameraScrollX(0);
                    cargarMapa(&(gameState->level), gameState->gameUI.level);   //Cargamos el nivel
                    initEntities(gameState);                                    //Inicializamos las entidades
                    if(gameState->gameUI.score == 0) {                          //Si es la primera vez que estamos en el nivel, ponemos la vidas al maximo
                        gameState->entidades.jugador.vidas = MAXLIVES;
                    }
                    else{
                        gameState->entidades.jugador.vidas = livesRecord;                   //Sino cargamos los datos del nivel anterior
                        gameState->entidades.jugador.powerUpsState = powerUpstateRecord;
                        if(powerUpstateRecord == SMALL) {
                            gameState->entidades.jugador.fisica.alto = PIXELSPERUNIT;
                        }
                        else {
                            gameState->entidades.jugador.fisica.alto = PIXELSPERUNIT*2;
                        }
                    }
                    setClosestPlayer(&(gameState->entidades.jugador));          //Ponemos al jugador objetivo de los enemigos
                    startInGameThreads(&fisicas, &animaciones, gameState);      //Comenzamos los thread de fisica y de animaciones
                    startTimer(INGAMETIMER);                            //Comenzamos el contador del nivels
                    nivelInicializado = 1;
                    resetWavePosition();
                    playMusicFromMemory(gameState->buffer.sound[UNDERWATERTHEME], gameState->buffer.sound[UNDERWATERTHEME]->volume);      //Iniciamos la musica
                }

                if(gameState->gameUI.time <= 0){                                //Si se acaba el tiempo, matamos al jugador
                    gameState->entidades.jugador.estado = DEAD;
                    gameState->entidades.jugador.powerUpsState = SMALL;
                    gameState->entidades.jugador.fisica.alto = PIXELSPERUNIT;
                    playMusicFromMemory(gameState->buffer.sound[UNDERWATERTHEME], 0);
                    playSoundFromMemory(gameState->buffer.sound[MARIODIES], gameState->buffer.sound[MARIODIES]->volume);
                }

                if (gameState->entidades.jugador.estado == DEAD) {              //Si el jugador esta muerto

                    gameState->entidades.jugador.vidas--;                   //Pierde una vida

                    stopTimer(INGAMETIMER);
                    stopTimer(PHYSICSTIMER);                           //Paramos el contador y las fisicas
                    stopTimer(ANIMETIMER);

                    resetEntitiesState(gameState);                              //Reiniciamos las posiciones de los objetos del nivel
                    resetWavePosition();

                    if(gameState->entidades.jugador.vidas > 0){                 //Si aun le quedan vidas al jugador, mostramos las vidas para jugar de nuevo
                        gameState->state = RETRYSCREEN;
                        nivelInicializado = 0;
                        setCameraScrollX(0);
                        nivelInicializado = 1;
                    }
                    else{                                                       //Sino paramos los thread de fisicas, animaciones y la de los enemigos
                        nivelInicializado = 0;
                        gameState->state = GAMEOVERSCREEN;
                        finishInGameThreads(&fisicas, &animaciones);
                        pthread_create(&endThread, NULL, finishLevel, gameState);
                        playSoundFromMemory(gameState->buffer.sound[GAMEOVERSOUND], gameState->buffer.sound[GAMEOVERSOUND]->volume);
                    }

                }

                //Si el jugador no esta ni muerto ni casi muerto, habilitamos la pausa
                if(gameState->entidades.jugador.estado != DEAD && gameState->entidades.jugador.estado != ALMOSTDEAD) {
                    if(evento == DOWNESCAPE || evento == DOWNP){
                        stopTimer(INGAMETIMER);
                        stopTimer(PHYSICSTIMER);
                        stopTimer(ANIMETIMER);
                        gameState->pauseSelection = 0;
                        gameState->state = PAUSE;
                        playSoundFromMemory(gameState->buffer.sound[PAUSEGAME], gameState->buffer.sound[PAUSEGAME]->volume);
                    }

                    if(evento == DOWNARRIBA){   //Si se presiona la tecla de arriba hacemos el sonido del salto
                        playSoundFromMemory(gameState->buffer.sound[JUMPSMALL], gameState->buffer.sound[JUMPSMALL]->volume);
                    }
                    movePlayer(evento, &gameState->entidades.jugador);      //Movemos al jugador segun el input
                }

                break;

            case PAUSE:

                switch(evento){

                    case DOWNESCAPE:
                    case DOWNP:
                        startTimer(INGAMETIMER);
                        startTimer(PHYSICSTIMER);
                        startTimer(ANIMETIMER);
                        gameState->state = INGAME;                  //Despauseamos el juego
                        break;
                    case DOWNBOTON:
                        switch (gameState->pauseSelection) {        //Si se presiono espacio
                            case RESUME:
                                startTimer(INGAMETIMER);
                                startTimer(PHYSICSTIMER);
                                startTimer(ANIMETIMER);
                                gameState->state = INGAME;          //Despauseamos el juego
                                break;

                            case BACKTOMENU:                        //Terminamos el nivel

                                nivelInicializado = 0;
                                gameState->entidades.jugador.estado = DEAD;

                                finishInGameThreads(&fisicas, &animaciones);
                                clearEntities(gameState);

                                initUI(&gameState->gameUI);
                                gameState->menuSelection = LEVELSELECTOR;
                                gameState->state = MENU;
                                playMusicFromMemory(gameState->buffer.sound[SUPERMARIOTHEME], gameState->buffer.sound[SUPERMARIOTHEME]->volume);

                                break;
                        }
                        break;

                    case DOWNARRIBA:
                    case DOWNABAJO:
                        updatePauseArrow(&gameState->pauseSelection, evento);       //Actualizamos la posicion de la flecha
                        break;
                    default:
                        break;
                }

                break;

            case NEXTLEVEL:

                nivelInicializado = 0;
                stopTimer(PHYSICSTIMER);
                stopTimer(ANIMETIMER);
                stopTimer(INGAMETIMER);
                gameState->gameUI.score += gameState->gameUI.time;      //Reiniciamos el tiempo, aumentamos el nivel y el score
                gameState->gameUI.level++;
                gameState->gameUI.time = MAXLEVELTIME;
                resetWavePosition();

                finishInGameThreads(&fisicas, &animaciones);            //Cerramos la fisica y las animaciones
                clearEntities(gameState);                               //Eliminamos las entidades

                sleep(1);
                gameState->state = INGAME;                              //Vamos al siguiente nivel

                break;

            case RETRYSCREEN:
                sleep(2);
                resetWavePosition();
                gameState->state = INGAME;
                gameState->gameUI.time = MAXLEVELTIME;                  //Reiniciamos el UI
                startTimer(PHYSICSTIMER);
                startTimer(INGAMETIMER);
                startTimer(ANIMETIMER);
                playMusicFromMemory(gameState->buffer.sound[UNDERWATERTHEME], gameState->buffer.sound[UNDERWATERTHEME]->volume);
                break;

            case GAMEOVERSCREEN:

                nivelInicializado = 0;

                #if MODOJUEGO == ALLEGRO

                //Si hubo un nuevo high score, esperamos a que se ingrese el nombre del jugador
                if(wasNewHighScoreAchieved(gameState)) {
                    if ((evento >= DOWNA) && (evento <= UP9) && ((evento - DOWNA) % 2 == 0)) {
                        *((gameState->pPlayerName) + numberOfLetter) = (char) ((evento - DOWNA) / 2 + 'A');

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


                    //Si se presiono enter, guardamos el score y volvemos al menu
                    if (evento == UPENTER && numberOfLetter > 0) {
                        saveNewHighScore(gameState);
                        initUI(&gameState->gameUI);
                        resetLastBlockInMap();
                        numberOfLetter = 0;
                        nombreLleno = 0;
                        gameState->menuSelection = LEVELSELECTOR;
                        gameState->state = MENU;
                        playMusicFromMemory(gameState->buffer.sound[SUPERMARIOTHEME], gameState->buffer.sound[SUPERMARIOTHEME]->volume);

                        for (int i = 0; i < MAXPLAYERNAME; i++) {
                            *((gameState->pPlayerName) + i) = '\0';
                        }
                    }
                }
                else{
                    //Si no se consiguio un nuevo highscore, mostramos el gameover y volvemos al menu
                    initUI(&gameState->gameUI);
                    resetLastBlockInMap();
                    sleep(4);
                    gameState->menuSelection = LEVELSELECTOR;
                    gameState->state = MENU;
                    playMusicFromMemory(gameState->buffer.sound[SUPERMARIOTHEME], gameState->buffer.sound[SUPERMARIOTHEME]->volume);

                }

                #elif MODOJUEGO == RASPI

                //Si hubo un nuevo high score, esperamos a que se ingrese el nombre del jugador
                if(wasNewHighScoreAchieved(gameState)) {
                    gameState->pPlayerName = "Raspberry";
                    saveNewHighScore(gameState);
                }

                //Hacemos un delay y volvemos al menu
                sleep(4);
                resetLastBlockInMap();
                initUI(&gameState->gameUI);
                gameState->menuSelection = LEVELSELECTOR;
                gameState->state = MENU;

                #endif

                break;

            default:
                break;
        }
    }

    pthread_exit(NULL);
}

char wasLevelInitialized(){
    return nivelInicializado;       //Devolvemos la variable que indica si el nivel esta inicializado o no
}

sem_t* getPhysicsSem(){
    return &fisicaSem;
}

sem_t* getAnimeSem(){
    return &animacionSem;
}

sem_t* getRenderSem(){
    return &renderSem;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void decreaseGameTime(void* gameState){
    estadoJuego_t *gs = gameState;
    gs->gameUI.time--;
    if(gs->gameUI.time == HURRYUPTIME && gs->entidades.jugador.estado == ALIVE){        //Si el tiempo es el limite y el jugador este vivo
        playSoundFromMemory(gs->buffer.sound[WARNINGTIMEOUT], gs->buffer.sound[WARNINGTIMEOUT]->volume);    //Ponemos el sonido de limite de tiempo
    }
}

static void startInGameThreads(pthread_t *fisicas, pthread_t *animaciones, estadoJuego_t *gameState) {
    sem_init(&animacionSem, 0, 1);             //Inicializamos el semaforo de control para este thread
    sem_init(&fisicaSem, 0, 0);                //Inicializamos el semaforo de control de ejecucion del thread de fisicas para controlarlo
    pthread_create(animaciones, NULL, animar, gameState);   //Creamos los threads de fisicas y de animaciones
    pthread_create(fisicas, NULL, fisica, gameState);

}

static void finishInGameThreads(const pthread_t *fisicas, const pthread_t *animaciones){
    stopTimer(PHYSICSTIMER);
    stopTimer(ANIMETIMER);
    pthread_cancel(*fisicas);
    pthread_cancel(*animaciones);       //Cerramos los thread de fisicas y animaciones

}

static void* finishLevel(void* gs){

    pthread_detach(pthread_self());

    estadoJuego_t* gameState = gs;

    //Matamos a todos los enemigos que esten vivos
    for(int i = 0; gameState->entidades.enemigos[i].identificador != NULLENTITIE && gameState->entidades.enemigos[i].estado == ALIVE; i++){
        gameState->entidades.enemigos[i].estado = DEAD;
    }

    destroyEntities(gameState);     //Destruimos las entidades y el mapa
    destroyMap(gameState);
    return NULL;
}

static void clearEntities(estadoJuego_t* gameState){

    //Destruimos todas las entidades del juego
    for(int i = 0; gameState->entidades.enemigos[i].identificador != NULLENTITIE && gameState->entidades.enemigos[i].estado == ALIVE; i++){
        gameState->entidades.enemigos[i].estado = DEAD;
    }
    usleep(100000);
    resetEntitiesState(gameState);
    resetWavePosition();
    resetLastBlockInMap();
    destroyMap(gameState);
    destroyEntities(gameState);
}