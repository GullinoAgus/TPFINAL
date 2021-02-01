/***************************************************************************//**
  file     +fisica.c+
  @brief    +archivo de codigo fuente que contiene las funciones para el control de fisicas del juego+
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "fisica.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define MOD(x) ((x < 0) ? (-x) : (x))       //Macro para obtener el modulo de un numero
#define SALTO  (-(25.0f * (1.0f/FPS)))      //Constante para la velocidad de un salto

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static int isColliding(fisica_t* object1, fisica_t* object2);
static void detectCollisions(void* gs);
static void doVulnerable(void* gs);

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static sem_t* fisicaSem;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

//Funcion utilizada como thread de motor de fisicas. Al momento de crear el thread recibe la estructura gamestate_t que reciben todos los threads principales
void* fisica(void* entrada) {

    pthread_detach(pthread_self());                     //Como no devuelve ningun valor, directamente hacemos un detach al comienzo

    fisicaSem = getPhysicsSem();

    int maxLevelsAvailable = getMaxLevelsAvailable();
    float scrollX;                                  //Inicializamos el scroll de pantalla
    estadoJuego_t *gameState = entrada;                 //casteamos el puntero recibido a gamestate_t para poder leer la informacion
    gameState->entidades.jugador.isMoving = 0;          //Inicializamos la variable de direccion de movimiento del personaje


    sem_wait(fisicaSem);
    createNewTimer(1.0f / (FPS), detectCollisions, PHYSICSTIMER);   //Inicializamos el timer de control para el thread de motor de fisicas. Este timer permite que ttodo este codigo no se este ejecutando permanentemente
    createNewTimer(1.5f, doVulnerable, DOVULNERABLETIMER);          //inicializamos un timer para dar un tiempo de invulnerabilidad al personaje, se utiliza mas adelante
    createNewTimer(1.0f / (FPS), detectCollisions, PHYSICSTIMER);
    startTimer(PHYSICSTIMER);                                                 //Comenzamos el timer de control para las fisicas

    while (gameState->state != GAMECLOSED) {        //while de control para el thread

        sem_wait(fisicaSem);                       //esperamos a que el timer de el post para ejecutar
        scrollX = getCameraScrollX();

        /*ACTUALIZACION DE VELOCIDAD DEL JUGADOR*/

        gameState->entidades.jugador.fisica.vely += GRAVEDAD;           //Aplicamos la gravedad
        if (gameState->entidades.jugador.fisica.vely > VELOCIDADYMAX) {         //controlamos que la velocidad en Y no se exceda del maximo programado
            gameState->entidades.jugador.fisica.vely = VELOCIDADYMAX;
        }

        if (gameState->entidades.jugador.isMoving) {                        //Control del movimiento horizontal de acuerdo a la direccion almacenada en isMoving( Ver funcion movePlayer). Aqui se tiene en cuenta la inercia del movimiento
            switch (gameState->entidades.jugador.isMoving) {
                case DOWNIZQUIERDA:
                    gameState->entidades.jugador.fisica.velx -= (1 - INERCIA);
                    break;
                case DOWNDERECHA:
                    gameState->entidades.jugador.fisica.velx += (1 - INERCIA);
                    break;

            }
        } else {                                                            //En caso de que el jugador no se encuentre presionando ninguna tecla de movimiento se lo frena con inercia
            gameState->entidades.jugador.fisica.velx *= INERCIA;
        }
        if (MOD(gameState->entidades.jugador.fisica.velx) > VELOCIDADXMAX) {    //Control de velocidad maxima en el sentido horizontal
            gameState->entidades.jugador.fisica.velx = VELOCIDADXMAX *
                                                       (MOD(gameState->entidades.jugador.fisica.velx) /
                                                        gameState->entidades.jugador.fisica.velx);
        }

        // ACTUALIZACION DE POSICIONES DEL JUGADOR
        //pthread_mutex_lock(&fisicaMutex);
        gameState->entidades.jugador.fisica.posx +=
                gameState->entidades.jugador.fisica.velx * (1.0f / (FPS)) * 1000;
        gameState->entidades.jugador.fisica.posy +=
                gameState->entidades.jugador.fisica.vely * (1.0f / (FPS)) * 1000;
        //pthread_mutex_unlock(&fisicaMutex);
        if (gameState->entidades.jugador.sobreBloque && gameState->entidades.jugador.fisica.vely != 0) {
            gameState->entidades.jugador.sobreBloque = false;
        }
        //Recorremos todos los enemigos para evaluar colisiones y actualizar sus posiciones
        for (int i = 0; gameState->entidades.enemigos[i].identificador != NULLENTITIE; i++) {
            if (isInsideScreenX(&gameState->entidades.enemigos[i].fisica)) {        //Si el enemigo se encuentra a la vista del jugador

                if(gameState->entidades.enemigos[i].estado == SLEPT){
                    startEnemy(&(gameState->entidades.enemigos[i]));
                }
                /* Actualizacion de posicion*/
                //pthread_mutex_lock(&fisicaMutex);
                gameState->entidades.enemigos[i].fisica.posx +=
                        gameState->entidades.enemigos[i].fisica.velx * (1.0f / (FPS)) * 1000;
                gameState->entidades.enemigos[i].fisica.posy +=
                        gameState->entidades.enemigos[i].fisica.vely * (1.0f / (FPS)) * 1000;
                //pthread_mutex_unlock(&fisicaMutex);

                /*Evaluacion de colisiones con enemigos en pantalla*/
                if (isColliding(&gameState->entidades.jugador.fisica, &gameState->entidades.enemigos[i].fisica)) {
                    if (gameState->entidades.jugador.estado != INVULNERABLE) {        //Si puede ser dañado
                        if (gameState->entidades.jugador.powerUpsState == SMALL && (gameState->entidades.jugador.estado != ALMOSTDEAD)) {    //Si es chiquito y no esta muerto
                            playMusicFromMemory(gameState->buffer.sound[UNDERWATERTHEME], 0);
                            playSoundFromMemory(gameState->buffer.sound[MARIODIES], gameState->buffer.sound[MARIODIES]->volume);
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
            else{
                if((gameState->entidades.enemigos[i].fisica.posx < (float)gameState->entidades.enemigos[i].fisica.ancho + scrollX) && gameState->entidades.enemigos[i].estado == ALIVE){
                    gameState->entidades.enemigos[i].estado = DEAD;
                }
            }
        }



        if ((int) gameState->entidades.jugador.fisica.posy < PIXELSPERUNIT) { //MANTIENE QUE MARIO NO SE ZARPE DEL TECHO

            gameState->entidades.jugador.fisica.posy += (PIXELSPERUNIT - gameState->entidades.jugador.fisica.posy);
            gameState->entidades.jugador.fisica.vely = 0.0f;
        }


        if (gameState->entidades.jugador.fisica.posx <
            scrollX) {  //MANTIENE QUE MARIO NO SE ZARPE DE LA IZQUIERDA
            gameState->entidades.jugador.fisica.velx = 0.0f;
            gameState->entidades.jugador.fisica.posx = scrollX;
        }

        if (gameState->entidades.jugador.fisica.posy > SCREENHEIGHT + PIXELSPERUNIT) {
            gameState->entidades.jugador.estado = DEAD;
        }


        // COLISIONES CON BLOQUES
        if (gameState->entidades.jugador.estado != ALMOSTDEAD && gameState->entidades.jugador.estado != DEAD) { //SI MARIO ESTA MUERTO O POR MORIRSE FISICAS NO CHECKEA COLISIONES
            for (int i = 0;
                 wasLevelInitialized() && gameState->entidades.bloques[i].identificador != NULLENTITIE; ++i) {
                if (isColliding(&gameState->entidades.jugador.fisica,
                                &gameState->entidades.bloques[i].fisica)) {         //Para cada entidad bloque verificamos si hay colision
                    switch (gameState->entidades.bloques[i].identificador) {
                        case MONEDA:                                                //Para las monedas subimos el puntaje y en caso de alcanzar la cantidad necesaria se otorga una vida extra
                            gameState->gameUI.coins++;
                            if (gameState->gameUI.coins >= 100) {
                                gameState->gameUI.coins = 0;
                                if(gameState->entidades.jugador.vidas < 9){
                                    gameState->entidades.jugador.vidas++;
                                }
                            }
                            playSoundFromMemory(gameState->buffer.sound[PICKUPCOIN], gameState->buffer.sound[PICKUPCOIN]->volume);
                            gameState->entidades.bloques[i].fisica.posy = -100;
                            gameState->gameUI.score += 10;
                            break;

                        case TOPPIPE:                                           //Al tocar la pipa de arriba pasa de nivel
                            if(gameState->gameUI.level+1 > maxLevelsAvailable){
                                gameState->entidades.jugador.vidas = 0;
                                gameState->entidades.jugador.estado = DEAD;
                            }
                            else{
                                gameState->state = NEXTLEVEL;
                            }
                            playSoundFromMemory(gameState->buffer.sound[ENTERPIPE], gameState->buffer.sound[ENTERPIPE]->volume);
                            break;

                        case MUSHROOM:                                          //Al comer el hongo hacemos crecer al jugador
                            playSoundFromMemory(gameState->buffer.sound[POWERUPSOUND], gameState->buffer.sound[POWERUPSOUND]->volume);
                            gameState->gameUI.score += 20;
                            gameState->entidades.jugador.powerUpsState = BIG;
                            gameState->entidades.jugador.fisica.alto = PIXELSPERUNIT*2;
                            gameState->entidades.bloques[i].fisica.posy = -100;
                            break;

                        default:                                                        //En el caso default solamente puede ser un alga o un bloque.
                            if ((gameState->entidades.jugador.fisica.posx +
                                 (float)gameState->entidades.jugador.fisica.ancho -
                                 gameState->entidades.bloques[i].fisica.posx <=
                                 VELOCIDADXMAX + (1.0f / (FPS)) * 500) !=
                                (VELOCIDADXMAX + (1.0f / (FPS)) * 500 >=
                                 (gameState->entidades.bloques[i].fisica.posx +
                                  (float)gameState->entidades.bloques[i].fisica.ancho) -
                                 gameState->entidades.jugador.fisica.posx)) {                   //Verificamos si es un choque lateral
                                if (gameState->entidades.jugador.fisica.posx <
                                    gameState->entidades.bloques[i].fisica.posx) { //Choque por izquierda
                                    gameState->entidades.jugador.fisica.posx =
                                            gameState->entidades.bloques[i].fisica.posx -
                                            (float)gameState->entidades.jugador.fisica.ancho;

                                } else if (                                         //si no fue por izquierda es por derecha, igual se verifica para evitar falsos positivos
                                        gameState->entidades.jugador.fisica.posx -
                                        (float)gameState->entidades.jugador.fisica.ancho <
                                        gameState->entidades.bloques[i].fisica.posx +
                                        (float)gameState->entidades.bloques[i].fisica.ancho) {
                                    gameState->entidades.jugador.fisica.posx =
                                            gameState->entidades.bloques[i].fisica.posx +
                                            (float)gameState->entidades.bloques[i].fisica.ancho;
                                }
                            } else if (                                             //Verificamos choque en sentido vertical
                                    ((gameState->entidades.jugador.fisica.posy +
                                      (float)gameState->entidades.jugador.fisica.alto) >
                                     gameState->entidades.bloques[i].fisica.posy) !=
                                    ((gameState->entidades.jugador.fisica.posy) >
                                     (gameState->entidades.bloques[i].fisica.posy +
                                      (float)gameState->entidades.bloques[i].fisica.alto))) {

                                gameState->entidades.jugador.fisica.vely = 0;
                                if (gameState->entidades.jugador.fisica.posy <
                                    gameState->entidades.bloques[i].fisica.posy) { //las patas tocan piso
                                    gameState->entidades.jugador.fisica.posy =
                                            gameState->entidades.bloques[i].fisica.posy -
                                            (float)gameState->entidades.jugador.fisica.alto;
                                    gameState->entidades.jugador.sobreBloque = true;
                                } else {
                                    gameState->entidades.jugador.fisica.posy =
                                            gameState->entidades.bloques[i].fisica.posy +
                                            (float)gameState->entidades.bloques[i].fisica.alto;
                                }
                            }
                            break;
                    }
                }
            }
        }
    }
    pthread_exit(NULL);
}

//Funcion utilizada para indicar movimientos del jugador. Se le pasa el evento de tecla presionada y un puntero a la entidad jugador_t.
// Con esta informacion la funcion modifica al jugador para que luego el motor de fisicas comience a moverlo
void movePlayer(int direction, void* player){

    jugador_t* matias = player;             //casteamos el puntero de void a jugador_t
    static int ultimoEvento;                //Variable de memoria para mantener el ultimo evento que ocurrio

    switch (direction) {                    //switch para distinguir los movimientos del personaje y asi cargarlo en la estructura

        case DOWNIZQUIERDA:
        case DOWNDERECHA:
            matias->isMoving = direction;
            break;

        case UPDERECHA:
            if (ultimoEvento == DOWNDERECHA || ultimoEvento == DOWNARRIBA || ultimoEvento == UPARRIBA || ultimoEvento == UPIZQUIERDA){
                matias->isMoving = false;
            }
            break;

        case UPIZQUIERDA:
            if (ultimoEvento == DOWNIZQUIERDA || ultimoEvento == DOWNARRIBA || ultimoEvento == UPARRIBA || ultimoEvento == UPDERECHA) {
                matias->isMoving = false;
            }
            break;

        case DOWNARRIBA:
            matias->fisica.vely = SALTO;
            break;

            // A continuacion tambien los del joystick, los cuales no se tiene acceso desde la PC
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

        default:
            break;
    }
    if (direction != 0){
        ultimoEvento = direction;
    }

}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
 //Funcion utilizada para controlar la ejecucion del thread de fisicas. Se ejecuta a traves del modulo times con un timer
static void detectCollisions(void* gs){
    sem_post(fisicaSem);
 }

//Funcion que detecta si dos objetos estan colisionando. para esto se le debe pasar la estructura de fisica_t de cada entidad u objeto
static int isColliding(fisica_t* object1, fisica_t* object2){
    int collision = 0;
    if( ((object1->posx + (float)object1->ancho) > object2->posx) && (object1->posx < (object2->posx + (float)object2->ancho)) &&
        ((object1->posy + (float)object1->alto) > object2->posy) && (object1->posy < (object2->posy + (float)object2->alto))){
        collision = 1;
    }

    return collision;
}

// Funcion para timer que hace vulnerable al jugador cuando comienza el juego. se utiliza junto con el modulo times
static void doVulnerable(void* gs){

    estadoJuego_t* gameState = gs;
    
    gameState->entidades.jugador.estado = ALIVE;
    stopTimer(DOVULNERABLETIMER);
}



