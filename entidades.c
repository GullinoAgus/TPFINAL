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
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static int diagonalMove(enemigo_t * thisEnemy);
static int moveDown(enemigo_t* thisEnemy);

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

    int movingDirection = 0;
    enemigo_t *thisEnemy = (enemigo_t*) enemy;
    jugador_t *player = closestPlayer;
    if (((thisEnemy->fisica.posy + thisEnemy->fisica.alto) > player->fisica.posy)){
        movingDirection = 0;
    } else {
        movingDirection = 1;
    }
    while(thisEnemy->estado == ALIVE) {
        //Esperamos a que el juego comienze
        if (player != NULL) {

            //Si el blooper esta debajo del personaje
            if(movingDirection == 0){
                movingDirection = diagonalMove(thisEnemy);  //Devuelve 0 mientras se este moviendo el diagonal, sino 1
            }

            if(movingDirection == 1) {
                movingDirection = moveDown(thisEnemy);    //Hace el descanso del enemigo
            }

        }
    }
    pthread_exit(NULL);
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/**
 * @brief Mueve a un blooper hacia abajo
*/

static int moveDown(enemigo_t* thisEnemy){

    int distanceToSwim = PIXELSPERUNIT*3;
    static int positionReached = 0;
    float lastYPosition;

    if(positionReached == 0){
        positionReached = 1;
        thisEnemy->fisica.velx = 0;
        thisEnemy->fisica.vely = 0.16f;
        thisEnemy->sprite = 1;
        lastYPosition = thisEnemy->fisica.posy;
    }
    else if(thisEnemy->fisica.posy >= (lastYPosition + distanceToSwim) && thisEnemy->fisica.posy + thisEnemy->fisica.alto > closestPlayer->fisica.posy){
        positionReached = 0;
    } else if(thisEnemy->fisica.posy + thisEnemy->fisica.alto < closestPlayer->fisica.posy + 10 && thisEnemy->fisica.posy + thisEnemy->fisica.alto > closestPlayer->fisica.posy - 10 ){
        positionReached = 1;
    }

    return positionReached;
}

/**
 * @brief Mueve a un blooper en diagonal
*/
static int diagonalMove(enemigo_t* thisEnemy){

    int distanceToSwim = PIXELSPERUNIT*5;
    static int positionReached = 1;
    float lastYPosition;
    //Movimiento diagonal
    if(positionReached == 1){
        positionReached = 0;
        lastYPosition = thisEnemy->fisica.posy;
        if(thisEnemy->fisica.posx <= closestPlayer->fisica.posx){
            thisEnemy->fisica.velx = 0.3f;
        }
        else{
            thisEnemy->fisica.velx = -0.3f;
        }

        thisEnemy->fisica.vely = -0.3f;
        thisEnemy->sprite = 0;
    }
    else if (thisEnemy->fisica.posy < 0){
        positionReached = 1;
    }
    else if(thisEnemy->fisica.posy <= (lastYPosition - distanceToSwim)){    //Si llego al punto que debia llegar, ponemos en 1 a positionReached
        positionReached = 1;
    }


    return positionReached;
}