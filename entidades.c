/***************************************************************************//**
  file     entidades.h
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

#define MOVDELAY 1
#define RESTTIME 1

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static jugador_t* closestPlayer;
static pthread_mutex_t miMutex;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static void diagonalMove(enemigo_t* thisEnemy);
static void moveDown(enemigo_t* thisEnemy);
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
}

void setClosestPlayer(jugador_t* player){
    closestPlayer = player;
    pthread_mutex_init(&miMutex, NULL);
}

void *cheepcheep (void *enemy){

    pthread_detach(pthread_self());
    srand(time(NULL));

    enemigo_t *thisEnemy = (enemigo_t*) enemy;
    int waypointReached = 1;
    int lastPosY;
    int offsetY;

    pthread_mutex_lock(&miMutex);
    if(thisEnemy->identificador == FASTCHEEPCHEEP) {
        thisEnemy->fisica.velx = -0.05f;
    }
    else{
        thisEnemy->fisica.velx = -0.03f;
    }

    while(thisEnemy->estado == ALIVE){

        if(waypointReached == 1){
            lastPosY = (int) thisEnemy->fisica.posy;
            offsetY = RANDOMNUM(10, 50, 1);
            if(!( PIXELSPERUNIT < (lastPosY + offsetY) && (lastPosY + offsetY) < (SCREENHEIGHT - PIXELSPERUNIT))){   //Si se van de los limites con el offset
                offsetY = -offsetY; //Lo damos vuelta
            }
            waypointReached = 0;
        }

        if(offsetY > 0){
            thisEnemy->fisica.vely = 0.02f;
            if((int)thisEnemy->fisica.posy >= lastPosY + offsetY){
                waypointReached = 1;
            }
        }
        else if(offsetY <= 0){
            thisEnemy->fisica.vely = -0.02f;
            if((int)thisEnemy->fisica.posy <= lastPosY + offsetY){
                waypointReached = 1;
            }
        }
        pthread_mutex_unlock(&miMutex);
    }

    pthread_exit(NULL);
}

void *blooper (void* enemy){

    pthread_detach(pthread_self());

    enemigo_t *thisEnemy = (enemigo_t*) enemy;
    jugador_t *player = closestPlayer;

    sleep(RANDOMNUM(0,1,0));

    while(thisEnemy->estado == ALIVE) {
        pthread_mutex_lock(&miMutex);
        //Esperamos a que el juego comienze
        if (player != NULL) {

            //Si esta debajo del personaje
            if ((thisEnemy->fisica.posy + (float)thisEnemy->fisica.alto) > player->fisica.posy) {
                diagonalMove(thisEnemy);  //Se mueve hacia el jugador
                moveDown(thisEnemy);    //Hace el descanso del enemigo
            }
            else {
                moveDown(thisEnemy);
            }
        }
        pthread_mutex_unlock(&miMutex);
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
static void moveDown(enemigo_t* thisEnemy){
    thisEnemy->fisica.velx = 0;
    thisEnemy->fisica.vely = 0.16f;
    thisEnemy->sprite = 1;
    sleep(RESTTIME);
}

/**
 * @brief Mueve a un blooper en diagonal
*/
static void diagonalMove(enemigo_t * thisEnemy){

    if(thisEnemy->fisica.posx <= closestPlayer->fisica.posx){
        thisEnemy->fisica.velx = 0.3f;
    }
    else{
        thisEnemy->fisica.velx = -0.3f;
    }

    thisEnemy->fisica.vely = -0.3f;
    thisEnemy->sprite = 0;
    sleep(MOVDELAY);
}