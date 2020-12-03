//
// Created by agus on 23/11/20.
//

#include "matiasBrosGame.h"
#include "unistd.h"
#include "pthread.h"

//TODO: Mover todo esto a game logic o de lo contrario, evaluar que podemos hacer
//FIXME: It doesn't work yet :v

#define MOVDELAY 200    //Tiempo en microsegundos

static int enemyID = -1;
static pthread_mutex_t myMutex;

static void* waitUntilNextMove(void* enemy);
static void diagonalMove(enemigo_t* enemy);

//Cargamos el indice del blooper de estado juego
void setEnemyID(int id){
    enemyID = id;
}

void *cheepcheep (void *entidades){
    enemigo_t *thisEnemy = &((entidades_t *) entidades)->enemigos[enemyID];
    jugador_t *player = &((entidades_t *) entidades)->jugador;

    int dead = 0;
    int lastPosY = thisEnemy->fisica.posy;

    while(!dead){
        dead = 1;
    }

}

void *blooper (void* entidades){

    enemigo_t *thisEnemy = &((entidades_t *) entidades)->enemigos[enemyID];
    jugador_t *player = &((entidades_t *) entidades)->jugador;

    pthread_t movement;
    pthread_create(&movement, NULL, waitUntilNextMove, NULL);

    int dead = 0;
    int ableToMoveAgain = 1;

    pthread_mutex_init(&myMutex, NULL);

    while(!dead){

        //Si los tentaculos del blooper estan debajo de la cabeza de personaje + un offset
        if((thisEnemy->fisica.posy + thisEnemy->fisica.alto) < player->fisica.posy ){

            if(ableToMoveAgain){
                if(thisEnemy->fisica.posx < player->fisica.posx){
                    thisEnemy->fisica.velx = 0.5f;      //TODO: Deberiamos poner los cambios de velocidades en fisica (?
                }
                else{
                    thisEnemy->fisica.velx = -0.5f;
                }

                diagonalMove(thisEnemy);
                ableToMoveAgain = 0;
            }
        }
        else{

        }

        if(!ableToMoveAgain) {

            pthread_mutex_lock(&myMutex);


            pthread_join(movement, NULL);
            ableToMoveAgain = 1;

            pthread_mutex_unlock(&myMutex);
        }

        if(thisEnemy->estado == DEAD){
            dead = 1;
        }
    }

    pthread_mutex_destroy(&myMutex);
}

static void* waitUntilNextMove(void* enemy){
    enemigo_t *thisEnemy = (enemigo_t*) enemy;
    thisEnemy->fisica.velx = 0;
    thisEnemy->fisica.vely = 0;
    thisEnemy->sprite = 1;
    usleep(MOVDELAY);
    pthread_exit(NULL);
}

static void diagonalMove(enemigo_t* enemy){
    enemy->fisica.vely = 0.5f;
    enemy->sprite = 0;
    usleep(MOVDELAY);
}