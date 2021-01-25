//
// Created by agus on 23/11/20.
//

#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "fisica.h"
#include "matiasBrosGame.h"
#include "IEvents.h"
#include "times.h"
#include "gamelogic.h"
#include "render.h"

pthread_mutex_t myMutex;
sem_t fisicaSem;

#define MOD(x) ((x < 0) ? (-x) : (x))
#define SALTO  (-(25.0f * (1.0f/FPS)))

static int isColliding(fisica_t* object1, fisica_t* object2);
static void detectCollisions(void* gs);
static void doVulnerable(void* gs);

void* fisica(void* entrada) {

    pthread_detach(pthread_self());

    float scrollX = 0;
    estadoJuego_t *gameState = entrada;
    gameState->entidades.jugador.isMoving = 0;

    pthread_mutex_init(&myMutex, 0);
    sem_init(&fisicaSem, 0, 1);

    createNewTimer(1.0f / (FPS), detectCollisions, PHYSICSTIMER);
    createNewTimer(1.5f, doVulnerable, DOVULNERABLETIMER);
    startTimer(PHYSICSTIMER);

    while (gameState->state != GAMECLOSED) {

        sem_wait(&fisicaSem);

        while(gameState->state == PAUSE);

        if (gameState->entidades.jugador.fisica.vely > VELOCIDADYMAX) {
            gameState->entidades.jugador.fisica.vely = VELOCIDADYMAX;
        }

        if (gameState->entidades.jugador.isMoving) {
            switch (gameState->entidades.jugador.isMoving) {
                case DOWNIZQUIERDA:
                    gameState->entidades.jugador.fisica.velx -= (1 - INERCIA);
                    break;
                case DOWNDERECHA:
                    gameState->entidades.jugador.fisica.velx += (1 - INERCIA);
                    break;

            }
        } else {
            gameState->entidades.jugador.fisica.velx *= INERCIA;
        }
        if (MOD(gameState->entidades.jugador.fisica.velx) > VELOCIDADXMAX) {
            gameState->entidades.jugador.fisica.velx = VELOCIDADXMAX *
                                                       (MOD(gameState->entidades.jugador.fisica.velx) /
                                                        gameState->entidades.jugador.fisica.velx);
        }

        // ACTUALIZACION DE POSICIONES
        pthread_mutex_lock(&myMutex);
        gameState->entidades.jugador.fisica.posx +=
                gameState->entidades.jugador.fisica.velx * (1.0f / (FPS)) * 1000;
        gameState->entidades.jugador.fisica.posy +=
                gameState->entidades.jugador.fisica.vely * (1.0f / (FPS)) * 1000;
        pthread_mutex_unlock(&myMutex);
        if (gameState->entidades.jugador.sobreBloque && gameState->entidades.jugador.fisica.vely != 0) {
            gameState->entidades.jugador.sobreBloque = false;
        }

        for (int i = 0; gameState->entidades.enemigos[i].identificador != NULLENTITIE; i++) {
            if (isInsideScreenX(&gameState->entidades.enemigos[i].fisica)) {
                pthread_mutex_lock(&myMutex);
                gameState->entidades.enemigos[i].fisica.posx +=
                        gameState->entidades.enemigos[i].fisica.velx * (1.0f / (FPS)) * 1000;
                gameState->entidades.enemigos[i].fisica.posy +=
                        gameState->entidades.enemigos[i].fisica.vely * (1.0f / (FPS)) * 1000;
                pthread_mutex_unlock(&myMutex);
            }
        }

        gameState->entidades.jugador.fisica.vely += GRAVEDAD;


        if (gameState->entidades.jugador.fisica.posy < PIXELSPERUNIT) { //MANTIENE QUE MARIO NO SE ZARPE DEL TECHO

            gameState->entidades.jugador.fisica.posy += (PIXELSPERUNIT - gameState->entidades.jugador.fisica.posy);
            gameState->entidades.jugador.fisica.vely = 0.0f;
        }

        scrollX = getCameraScrollX();
        if (gameState->entidades.jugador.fisica.posx <
            0.0f + scrollX) {  //MANTIENE QUE MARIO NO SE ZARPE DE LA IZQUIERDA
            gameState->entidades.jugador.fisica.velx = 0.0f;
            gameState->entidades.jugador.fisica.posx = scrollX;
        }

        if (gameState->entidades.jugador.fisica.posy > SCREENHEIGHT + PIXELSPERUNIT) {
            gameState->entidades.jugador.estado = DEAD;
        }


        //Si colisiona con algun enemigo

        for (int i = 0; gameState->entidades.enemigos[i].identificador != NULLENTITIE; ++i) {
            if (isColliding(&gameState->entidades.jugador.fisica, &gameState->entidades.enemigos[i].fisica)) {
                if (gameState->entidades.jugador.estado != INVULNERABLE) {        //Si puede ser dañado
                    if (gameState->entidades.jugador.powerUpsState == SMALL && (gameState->entidades.jugador.estado != ALMOSTDEAD)) {    //Si es chiquito y no esta muerto
                        playMusicFromMemory(gameState->buffer.sound[SUPERMARIOTHEME], 0);
                        playSoundFromMemory(gameState->buffer.sound[MARIODIES], SDL_MIX_MAXVOLUME);

#if MODOJUEGO == 0

                        gameState->entidades.jugador.estado = ALMOSTDEAD;

#elif MODOJUEGO == 1    //En el caso de la raspi me quiero evitar la animacion de la muerte, ya que complica entender que te mato

                        gameState->entidades.jugador.estado = DEAD;
#endif

                    } else if (gameState->entidades.jugador.powerUpsState == BIG) { //Si es grande
                        gameState->entidades.jugador.powerUpsState = SMALL;     //Lo hacemos chiquito
                        gameState->entidades.jugador.fisica.alto = PIXELSPERUNIT;
                        gameState->entidades.jugador.estado = INVULNERABLE;
                        startTimer(DOVULNERABLETIMER);
                    }
                    break;
                }
            }
        }


        // COLISIONES
        if (gameState->entidades.jugador.estado != ALMOSTDEAD && gameState->entidades.jugador.estado !=
                                                                 DEAD) { //SI MARIO ESTA MUERTO O POR MORIRSE FISICAS NO CHECKEA COLISIONES
            for (int i = 0;
                 wasLevelInitialized() && gameState->entidades.bloques[i].identificador != NULLENTITIE; ++i) {
                if (isColliding(&gameState->entidades.jugador.fisica,
                                &gameState->entidades.bloques[i].fisica)) {

                    if (gameState->entidades.bloques[i].identificador == MONEDA) {
                        gameState->gameUI.coins++;
                        if (gameState->gameUI.coins >= 100) {
                            gameState->gameUI.coins = 0;
                            gameState->entidades.jugador.vidas++;
                        }
                        playSoundFromMemory(gameState->buffer.sound[PICKUPCOIN], SDL_MIX_MAXVOLUME);
                        gameState->entidades.bloques[i].fisica.posy = -100;
                        gameState->gameUI.score += 10;
                    } else if (gameState->entidades.bloques[i].identificador == TOPPIPE) {
                        gameState->state = NEXTLEVEL;
                        playSoundFromMemory(gameState->buffer.sound[ENTERPIPE], SDL_MIX_MAXVOLUME);
                    } else if (gameState->entidades.bloques[i].identificador == MUSHROOM) {
                        gameState->entidades.jugador.powerUpsState = BIG;
                        gameState->entidades.jugador.fisica.alto = PIXELSPERUNIT*2;
                        gameState->entidades.bloques[i].fisica.posy = -100;
                    } else if ((gameState->entidades.jugador.fisica.posx +
                                gameState->entidades.jugador.fisica.ancho -
                                gameState->entidades.bloques[i].fisica.posx <=
                                VELOCIDADXMAX + (1.0f / (FPS)) * 500) !=
                               (VELOCIDADXMAX + (1.0f / (FPS)) * 500 >=
                                (gameState->entidades.bloques[i].fisica.posx +
                                 gameState->entidades.bloques[i].fisica.ancho) -
                                gameState->entidades.jugador.fisica.posx)) {
                        if (gameState->entidades.jugador.fisica.posx <
                            gameState->entidades.bloques[i].fisica.posx) { //Choque por izquierda
                            gameState->entidades.jugador.fisica.posx =
                                    gameState->entidades.bloques[i].fisica.posx -
                                    gameState->entidades.jugador.fisica.ancho;

                        } else if (
                                gameState->entidades.jugador.fisica.posx -
                                gameState->entidades.jugador.fisica.ancho <
                                gameState->entidades.bloques[i].fisica.posx +
                                gameState->entidades.bloques[i].fisica.ancho) {
                            gameState->entidades.jugador.fisica.posx =
                                    gameState->entidades.bloques[i].fisica.posx +
                                    gameState->entidades.bloques[i].fisica.ancho;
                        }
                    } else if (
                            ((gameState->entidades.jugador.fisica.posy +
                              gameState->entidades.jugador.fisica.alto) >
                             gameState->entidades.bloques[i].fisica.posy) !=
                            ((gameState->entidades.jugador.fisica.posy) >
                             (gameState->entidades.bloques[i].fisica.posy +
                              gameState->entidades.bloques[i].fisica.alto))) {

                        gameState->entidades.jugador.fisica.vely = 0;
                        if (gameState->entidades.jugador.fisica.posy <
                            gameState->entidades.bloques[i].fisica.posy) { //las patas
                            gameState->entidades.jugador.fisica.posy =
                                    gameState->entidades.bloques[i].fisica.posy -
                                    gameState->entidades.jugador.fisica.alto;
                            gameState->entidades.jugador.sobreBloque = true;
                        } else {
                            gameState->entidades.jugador.fisica.posy =
                                    gameState->entidades.bloques[i].fisica.posy +
                                    gameState->entidades.bloques[i].fisica.alto;
                        }
                    }
                }
            }
        }

    }


    pthread_exit(NULL);
}

void movePlayer(int direction, void* player){

    jugador_t* matias = player;
    static int ultimoEvento;

    switch (direction) {

        case DOWNIZQUIERDA:
            matias->isMoving = direction;
            break;

        case DOWNDERECHA:
            matias->isMoving = direction;
            break;

        case UPDERECHA:
        case UPIZQUIERDA:
            matias->isMoving = false;
            break;

        case DOWNARRIBA:
            matias->fisica.vely = SALTO;
            break;

            // A continuacion tambien los del joystick, los cuales no se tiene acceso desde las flechitas
        case DOWNARRIBADERECHA:
            if (ultimoEvento != DOWNARRIBADERECHA) {
                matias->fisica.vely = SALTO;
                matias->isMoving = DOWNDERECHA;
            }
            break;
        case DOWNARRIBAIZQUIERDA:
            if (ultimoEvento != DOWNARRIBAIZQUIERDA) {
                matias->fisica.vely = SALTO;
                matias->isMoving = DOWNIZQUIERDA;
            }
            break;
    }
    ultimoEvento = direction;
}

static void detectCollisions(void* gs){
    sem_post(&fisicaSem);
}

static int isColliding(fisica_t* object1, fisica_t* object2){
    int collision = 0;
    if( ((object1->posx + object1->ancho) > object2->posx) && (object1->posx < (object2->posx + object2->ancho)) &&
        ((object1->posy + object1->alto) > object2->posy) && (object1->posy < (object2->posy + object2->alto))){
        collision = 1;
    }

    return collision;
}

static void doVulnerable(void* gs){

    estadoJuego_t* gameState = gs;
    
    gameState->entidades.jugador.estado = ALIVE;
    stopTimer(DOVULNERABLETIMER);
}



