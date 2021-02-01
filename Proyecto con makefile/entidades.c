/***************************************************************************//**
  file     entidades.h
  @brief    definicion de entidades_t y funciones relacionadas a la misma
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include "entidades.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define BLOOPERSPRITE2HEIGHTCORRECTION 1.1875
#define RANDOMNUM(lower, higher, negativeEnabled) ( (negativeEnabled == 1) ? ((rand()%2 == 1) ? (-(rand() % (higher-lower+1) + lower))  : (rand() % (higher-lower+1) + lower)) : (rand() % (higher-lower+1) + lower) )

#define MOVDELAY 1
#define RESTTIME 1

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static jugador_t* closestPlayer;

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
    pthread_create(&(thisEnemy->enemyIA), NULL, thisEnemy->funcionMovimiento, thisEnemy);       //Lanza el thread asociado al enemigo
}

void setClosestPlayer(jugador_t* player){
    closestPlayer = player;
}

void *cheepcheep (void *enemy){

    pthread_detach(pthread_self());
    srand(time(NULL));  //Creamos una semilla

    enemigo_t *thisEnemy = (enemigo_t*) enemy;
    int waypointReached = 1;
    int lastPosY;
    int offsetY;

    //Definimos la velocidad horizontal del cheepcheep segun el tipo que sea
    if(thisEnemy->identificador == FASTCHEEPCHEEP) {
        thisEnemy->fisica.velx = -0.05f;
    }
    else{
        thisEnemy->fisica.velx = -0.03f;
    }

    //Mientras este vivo
    while(thisEnemy->estado == ALIVE){
        usleep((1.0f/FPS) * 1000000);
        if(waypointReached == 1){   //Si llegamos al waypoint calculado
            lastPosY = (int) thisEnemy->fisica.posy;    //Guardamos la posicion actual
            offsetY = RANDOMNUM(10, 50, 1); //Calculamos el nuevo desplazamiento
            if(!( PIXELSPERUNIT < (lastPosY + offsetY) && (lastPosY + offsetY) < (SCREENHEIGHT - PIXELSPERUNIT))){   //Si se van de los limites con el offset
                offsetY = -offsetY; //Lo damos vuelta
            }
            waypointReached = 0;    //Ponemos que no llegamos al waypoint
        }

        //Si tiene que ir para abajo
        if(offsetY > 0){
            thisEnemy->fisica.vely = 0.02f; //Ponemos su velocidad hacia abajo
            if((int)thisEnemy->fisica.posy >= lastPosY + offsetY){  //Si llegamos o pasamos al waypoint
                waypointReached = 1;    //Cambiamos el estado de la variable
            }
        }
        else if(offsetY <= 0){  //si tiene que ir para arriba
            thisEnemy->fisica.vely = -0.02f;    //Ponemos su velocidad hacia arriba
            if((int)thisEnemy->fisica.posy <= lastPosY + offsetY){  //Si llegamos o pasamos al waypoint
                waypointReached = 1;    //Cambiamos el estado de la variable
            }
        }
    }

    pthread_exit(NULL);
}

void *blooper (void* enemy){

    pthread_detach(pthread_self());

    enemigo_t *thisEnemy = (enemigo_t*) enemy;
    jugador_t *player = closestPlayer;


    while(thisEnemy->estado == ALIVE) {
        //Esperamos a que el juego comienze
        if (player != NULL) {

            //Si esta debajo del personaje
            if ((thisEnemy->fisica.posy + (float)thisEnemy->fisica.alto) > player->fisica.posy) {
                diagonalMove(thisEnemy);  //Se mueve hacia el jugador
            }

            moveDown(thisEnemy);    //Hace el descanso del enemigo
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
static void moveDown(enemigo_t* thisEnemy){
    thisEnemy->fisica.velx = 0;
    thisEnemy->fisica.vely = 0.16f;
    thisEnemy->fisica.alto = (int) (PIXELSPERUNIT * BLOOPERSPRITE2HEIGHTCORRECTION);
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
    thisEnemy->fisica.alto = PIXELSPERUNIT*2;
    thisEnemy->fisica.vely = -0.3f;
    thisEnemy->sprite = 0;
    sleep(MOVDELAY);
}