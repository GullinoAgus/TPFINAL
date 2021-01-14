//
// Created by agus on 23/11/20.
//

#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include "fisica.h"
#include "matiasBrosGame.h"
#include "IEvents.h"
#include "times.h"
#include "gamelogic.h"
#include "render.h"

static pthread_mutex_t myMutex;

#define MOD(x) ((x < 0) ? (-x) : (x))
#define SALTO  (-1.0f)

static int detectCollision = 0;

static int isColliding(fisica_t* object1, fisica_t* object2);
static void detectCollisions(void* gs);

void* fisica(void* entrada){

    estadoJuego_t *gameState = entrada;
    gameState->entidades.jugador.isMoving = 0;
    pthread_mutex_init(&myMutex, 0);
    createNewTimer(1.0f/(FPS*4), detectCollisions, PHYSICSTIMER);
    startTimer(PHYSICSTIMER);

    while(gameState->state != GAMECLOSED) {

        while(gameState->state != INGAME);

        if(detectCollision == 1) {

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
                    gameState->entidades.jugador.fisica.velx * (1.0f/(FPS*4)) * 1000;
            gameState->entidades.jugador.fisica.posy +=
                    gameState->entidades.jugador.fisica.vely * (1.0f/(FPS*4)) * 1000;
            pthread_mutex_unlock(&myMutex);
            if (gameState->entidades.jugador.sobreBloque && gameState->entidades.jugador.fisica.vely != 0) {
                gameState->entidades.jugador.sobreBloque = false;
            }

            for (int i = 0; gameState->entidades.enemigos[i].identificador != NULLENTITIE; i++) {
                if(isInsideScreenX(&gameState->entidades.enemigos[i].fisica)){
                    pthread_mutex_lock(&myMutex);
                    gameState->entidades.enemigos[i].fisica.posx +=
                            gameState->entidades.enemigos[i].fisica.velx * (1.0f/(FPS*4)) * 1000;
                    gameState->entidades.enemigos[i].fisica.posy +=
                            gameState->entidades.enemigos[i].fisica.vely * (1.0f/(FPS*4)) * 1000;
                    pthread_mutex_unlock(&myMutex);
                }
            }

            gameState->entidades.jugador.fisica.vely += GRAVEDAD;


            if (gameState->entidades.jugador.fisica.posy < PIXELSPERUNIT) { //MANTIENE QUE MARIO NO SE ZARPE DEL TECHO

                gameState->entidades.jugador.fisica.posy += (PIXELSPERUNIT - gameState->entidades.jugador.fisica.posy);
                gameState->entidades.jugador.fisica.vely = 0.0f;
            }

            float scrollX = getCameraScrollX();
            if (gameState->entidades.jugador.fisica.posx < 0.0f + scrollX) {  //MANTIENE QUE MARIO NO SE ZARPE DE LA IZQUIERDA
                gameState->entidades.jugador.fisica.velx = 0.0f;
                gameState->entidades.jugador.fisica.posx = scrollX;
            }

            if(gameState->entidades.jugador.fisica.posy > SCREENHEIGHT + PIXELSPERUNIT){
                gameState->entidades.jugador.estado = DEAD;
            }


            //if(a.max.x < b.min.x or a.min.x > b.max.x) return false;
            //if(a.max.y < b.min.y or a.min.y > b.max.y) return false;

            //Si colisiona con algun enemigo

            for (int i = 0; gameState->entidades.enemigos[i].identificador != NULLENTITIE; ++i) {
                if (isColliding(&gameState->entidades.jugador.fisica, &gameState->entidades.enemigos[i].fisica)) {
                    if (gameState->entidades.jugador.estado != INVULNERABLE) {        //Si puede ser dañado
                        if (gameState->entidades.jugador.powerUpsState == SMALL &&
                            (gameState->entidades.jugador.estado != ALMOSTDEAD)) {    //Si es chiquito
                            gameState->entidades.jugador.estado = DEAD;   //FIXME: Aca va almostdead
                        } else if (gameState->entidades.jugador.powerUpsState == BIG) { //Si es grande
                            gameState->entidades.jugador.powerUpsState = SMALL;     //Lo hacemos chiquito
                        }
                        break;
                    }
                }
            }


            // COLISIONES
            if (gameState->entidades.jugador.estado != ALMOSTDEAD && gameState->entidades.jugador.estado != DEAD) { //SI MARIO ESTA MUERTO O POR MORIRSE FISICAS NO CHECKEA COLISIONES
                for (int i = 0; wasLevelInitialized() && gameState->entidades.bloques[i].identificador != NULLENTITIE; ++i) {
                    if (isColliding(&gameState->entidades.jugador.fisica,
                                    &gameState->entidades.bloques[i].fisica)) {

                        if (gameState->entidades.bloques[i].identificador == MONEDA) {
                            gameState->gameUI.coins++;
                            if(gameState->gameUI.coins >= 100){
                                gameState->gameUI.coins = 0;
                                gameState->entidades.jugador.vidas++;
                            }
                            gameState->entidades.bloques[i].fisica.posy = -100;
                            gameState->gameUI.score += 10;
                        } else if (gameState->entidades.bloques[i].identificador == TOPPIPE) {
                            gameState->state = NEXTLEVEL;
                        } else if ((gameState->entidades.jugador.fisica.posx +
                                    gameState->entidades.jugador.fisica.ancho -
                                    gameState->entidades.bloques[i].fisica.posx <= VELOCIDADXMAX + (1.0f/(FPS*4)) * 1000) !=
                                   (VELOCIDADXMAX + (1.0f/(FPS*4)) * 1000 >=
                                    (gameState->entidades.bloques[i].fisica.posx +
                                     gameState->entidades.bloques[i].fisica.ancho) -
                                    gameState->entidades.jugador.fisica.posx)) {
                            gameState->entidades.jugador.fisica.velx = 0.0f;
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

            detectCollision = 0;
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
    detectCollision = 1;
}

static int isColliding(fisica_t* object1, fisica_t* object2){
    int collision = 0;
    if( ((object1->posx + object1->ancho) > object2->posx) && (object1->posx < (object2->posx + object2->ancho)) &&
        ((object1->posy + object1->alto) > object2->posy) && (object1->posy < (object2->posy + object2->alto))){
        collision = 1;
    }

    return collision;
}



