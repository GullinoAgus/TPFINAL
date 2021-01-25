/***************************************************************************//**
  @file     entidades.h
  @brief    definicion de entidades_t y funciones relacionadas a la misma
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "matiasBrosGame.h"
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define RANDOMNUM(lower, higher, negativeEnabled) ( (negativeEnabled == 1) ? ((rand()%2 == 1) ? (-(rand() % (higher-lower+1) + lower))  : (rand() % (higher-lower+1) + lower)) : (rand() % (higher-lower+1) + lower) )


/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static jugador_t* closestPlayer;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void startEnemy(enemigo_t* thisEnemy){
    thisEnemy->estado = ALIVE;
    pthread_create(&(thisEnemy->enemyIA), NULL, thisEnemy->funcionMovimiento, thisEnemy);
}

void destroyEnemyIA(enemigo_t* thisEnemy){
    thisEnemy->estado = DEAD;
    pthread_join(thisEnemy->enemyIA, NULL);
}

void setClosestPlayer(jugador_t* player){
    closestPlayer = player;
}

void *cheepcheep (void *enemy){


    srand(time(NULL));

    enemigo_t *thisEnemy = (enemigo_t*) enemy;
    int waypointReached = 1;
    float lastPosY;
    int offsetY;

    if(thisEnemy->identificador == FASTCHEEPCHEEP) {
        thisEnemy->fisica.velx = -0.05f;
    }
    else{
        thisEnemy->fisica.velx = -0.03f;
    }

    while(thisEnemy->estado == ALIVE){

        if(waypointReached == 1){
            lastPosY = thisEnemy->fisica.posy;
            offsetY = RANDOMNUM(10, 50, 1);
            if(!(PIXELSPERUNIT < (lastPosY + offsetY) && (lastPosY + offsetY) < (SCREENHEIGHT - PIXELSPERUNIT))){   //Si se van de los limites con el offset
                offsetY = -offsetY; //Lo damos vuelta
            }
            waypointReached = 0;
        }

        if(offsetY > 0){
            thisEnemy->fisica.vely = 0.02f;
            if(thisEnemy->fisica.posy >= lastPosY + offsetY){
                waypointReached = 1;
            }
        }
        else if(offsetY <= 0){
            thisEnemy->fisica.vely = -0.02f;
            if(thisEnemy->fisica.posy <= lastPosY + offsetY){
                waypointReached = 1;
            }
        }

    }

    pthread_exit(NULL);
}

void *blooper (void* enemy){

    int distanceToSwim;
    int waypointReached = 1;
    float lastPosY;

    enemigo_t *thisEnemy = (enemigo_t*) enemy;
    jugador_t *player = NULL;

    while(thisEnemy->estado == ALIVE) {

        //Esperamos a que el juego comienze
        if (player != NULL) {

            if(waypointReached == 1){
                lastPosY = thisEnemy->fisica.posy;
                waypointReached = 0;

                if(((thisEnemy->fisica.posy + thisEnemy->fisica.alto) > player->fisica.posy)){
                    distanceToSwim = -PIXELSPERUNIT*5;
                }
                else{
                    distanceToSwim = PIXELSPERUNIT*3;
                }
            }

            if(distanceToSwim > 0){

                thisEnemy->fisica.velx = 0;
                thisEnemy->fisica.vely = 0.16f;
                thisEnemy->sprite = 1;

                if(thisEnemy->fisica.posy >= lastPosY + distanceToSwim){
                    waypointReached = 1;
                }
            }
            else{

                thisEnemy->fisica.vely = -0.3f;
                thisEnemy->sprite = 0;
                if(thisEnemy->fisica.posx <= closestPlayer->fisica.posx){
                    thisEnemy->fisica.velx = 0.3f;
                }
                else{
                    thisEnemy->fisica.velx = -0.3f;
                }

                if (thisEnemy->fisica.posy <= lastPosY + distanceToSwim) {
                    waypointReached = 1;
                }
            }

        }
        else {
            player = closestPlayer;
        }
    }
    pthread_exit(NULL);
}

