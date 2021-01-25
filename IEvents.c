/***************************************************************************//**
  @file     IEvents.c
  @brief    Interpretacion de eventos de teclado y joystick
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "IEvents.h"
#include "matiasBrosGame.h"

#if MODOJUEGO == ALLEGRO

#include "allegro.h"

#else

#include "raspi.h"

#endif

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

#if MODOJUEGO == ALLEGRO

enum keys {KEY_0 = 0,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_A,KEY_B,KEY_C,KEY_D,KEY_E,KEY_F,KEY_G,KEY_H,KEY_I,KEY_J,KEY_K,KEY_L,KEY_M,KEY_N,
    KEY_ENIE,KEY_O,KEY_P,KEY_Q,KEY_R,KEY_S,KEY_T,KEY_U,KEY_V,KEY_W,KEY_X,KEY_Y,KEY_Z,KEY_LEFT,KEY_RIGHT,KEY_UP,KEY_DOWN,KEY_SPACE,KEY_ESCAPE,KEY_ENTER,KEY_BACKSPACE};

#endif

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

#if MODOJUEGO == ALLEGRO

int a = 0;  /**<TODO: Creo que esto lo puso alvaro para debuggear y quedo aca */

#endif

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

#if MODOJUEGO == ALLEGRO

static bool key_pressed[KEY_BACKSPACE+1];

#endif

static char inputBuffer [MAXIMOEVENTOSBUFFER] = {0}; //He aqui el buffer de eventos
static char* bufferPointer = inputBuffer;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

char getInputEvent (void){

    char salida;
    if (*bufferPointer == VACIO){
        salida = VACIO;
    } else {
        if (bufferPointer <= inputBuffer + MAXIMOEVENTOSBUFFER && bufferPointer > inputBuffer) {
            salida = *bufferPointer;
            *bufferPointer = VACIO;
            bufferPointer--;
        } else {
            salida = *bufferPointer;
            *bufferPointer = VACIO;
            bufferPointer = inputBuffer + MAXIMOEVENTOSBUFFER;
        }
    }
    return salida;
}

void storeInputEvent (char evento){

    if (bufferPointer >= inputBuffer && bufferPointer < inputBuffer + MAXIMOEVENTOSBUFFER){

        bufferPointer++;
        *bufferPointer = evento;
    }
    else{
        bufferPointer = inputBuffer;
        *bufferPointer = evento;
    }
}

int esBufferVacio (void){
    int salida = 1;         //asumo que el buffer esta vacio hasta que encuentre un evento

        if (*bufferPointer != VACIO){
            salida = 0;
        }

    return salida;
}

void limpiarBuffer (void){

    int i = 0;
    for (;i<MAXIMOEVENTOSBUFFER; i++){
        inputBuffer[i] = 0;
    }
}

#if MODOJUEGO == RASPI

void * InputEvent(void* gs) {
    jcoord_t myCoords;      //He aqui las coordenadas del joystick
    joy_init();                //inicializo el joystick
    char ultimoEvento = VACIO;
    char ultimoEventoBoton = VACIO;
    estadoJuego_t* gameState = (estadoJuego_t*) gs;

    while (gameState->state != GAMECLOSED) { //ACA SERIA MEJOR SETEARLO CON EL TIMER, O PONER UN DELAY EN EL CODIGO
        joy_update();                            //primero actualizo las coordenadas medidas
        myCoords = joy_get_coord();                //luego las guardo en myCoords
        if ((myCoords.x >= LIMITEMOVDEPALANCA) &&
            (myCoords.y <= LIMITEMOVDEPALANCA && myCoords.y >= -LIMITEMOVDEPALANCA)) { //se mueve hacia la derecha
            if (ultimoEvento != UPDERECHA) {
                storeInputEvent(ultimoEvento);
                storeInputEvent(DOWNDERECHA);
                ultimoEvento = UPDERECHA;
            }
        } else if ((myCoords.x >= LIMITEMOVDEPALANCA) &&
                   (myCoords.y <= -LIMITEMOVDEPALANCA)) { //se mueve hacia abajo a la derecha
            if (ultimoEvento != UPABAJODERECHA) {
                storeInputEvent(ultimoEvento);
                storeInputEvent(DOWNABAJODERECHA);
                ultimoEvento = (UPABAJODERECHA);
            }
        } else if ((myCoords.x >= -LIMITEMOVDEPALANCA && myCoords.x <= LIMITEMOVDEPALANCA) &&
                   (myCoords.y <= -LIMITEMOVDEPALANCA)) { //se mueve hacia abajo
            if (ultimoEvento != UPABAJO) {
                storeInputEvent(ultimoEvento);
                storeInputEvent(DOWNABAJO);
                ultimoEvento = (UPABAJO);
            }
        } else if ((myCoords.x <= -LIMITEMOVDEPALANCA) &&
                   (myCoords.y <= -LIMITEMOVDEPALANCA)) { //se mueve hacia abajo a la izquierda
            if (ultimoEvento != UPABAJOIZQUIERDA) {
                storeInputEvent(ultimoEvento);
                storeInputEvent(DOWNABAJOIZQUIERDA);
                ultimoEvento = (UPABAJOIZQUIERDA);
            }
        } else if ((myCoords.x <= -LIMITEMOVDEPALANCA) &&
                   (myCoords.y >= -LIMITEMOVDEPALANCA && myCoords.y <= LIMITEMOVDEPALANCA)) { //se mueve hacia la izq
            if (ultimoEvento != UPIZQUIERDA) {
                storeInputEvent(ultimoEvento);
                storeInputEvent(DOWNIZQUIERDA);
                ultimoEvento = (UPIZQUIERDA);
            }
        } else if ((myCoords.x <= -LIMITEMOVDEPALANCA) &&
                   (myCoords.y >= LIMITEMOVDEPALANCA)) { //se mueve hacia arriba a la izquierda
            if (ultimoEvento != UPARRIBAIZQUIERDA) {
                storeInputEvent(ultimoEvento);
                storeInputEvent(DOWNARRIBAIZQUIERDA);
                ultimoEvento = (UPARRIBAIZQUIERDA);
            }
        } else if ((myCoords.x >= -LIMITEMOVDEPALANCA && myCoords.x <= LIMITEMOVDEPALANCA) &&
                   (myCoords.y >= LIMITEMOVDEPALANCA)) { //se mueve hacia arriba
            if (ultimoEvento != UPARRIBA) {
                storeInputEvent(ultimoEvento);
                storeInputEvent(DOWNARRIBA);
                ultimoEvento = (UPARRIBA);
            }
        } else if ((myCoords.x >= LIMITEMOVDEPALANCA) &&
                   (myCoords.y >= LIMITEMOVDEPALANCA)) { // se mueve hacia arriba a la derecha
            if (ultimoEvento != UPARRIBADERECHA) {
                storeInputEvent(ultimoEvento);
                storeInputEvent(DOWNARRIBADERECHA);
                ultimoEvento = (UPARRIBADERECHA);
            }
        } else {   //Si el joystick no indica nada, guardare eso tambien para evitar problemas debido a la circularidad del buffer
            if (ultimoEvento != NOHAYMOVIMIENTO) {
                storeInputEvent(ultimoEvento);
                ultimoEvento = (NOHAYMOVIMIENTO);
            }
        }

        //Checkeo si se presiono el boton del switch

        if (joy_get_switch() == J_PRESS) {
            if (ultimoEventoBoton != DOWNBOTON) {

                storeInputEvent(DOWNBOTON);
                ultimoEventoBoton = DOWNBOTON;
            }
        } else if (joy_get_switch() == J_NOPRESS) {
            if (ultimoEventoBoton != UPBOTON) {

                storeInputEvent(UPBOTON);
                ultimoEventoBoton = UPBOTON;
            }
        }
    }
}

#elif MODOJUEGO == ALLEGRO

/*FIXME: La borramos?
int mouseChanges(bool estado, int evMouseX, int evMouseY){

    int salida = 0;

    if(estado == PRESSED){
    a++;
    }

    if(estado == NOPRESSED){
    a++;
    }

    return salida;
}
*/

void * keyboardChanges (void* myGameState){

    int tecla;
    int estado = 2;

    estadoJuego_t *gameState = (estadoJuego_t*) myGameState;

    ALLEGRO_EVENT_QUEUE * colaEventos = al_create_event_queue();  //genero mi cola de eventos
    al_register_event_source(colaEventos, al_get_keyboard_event_source());

    ALLEGRO_EVENT evento;

    while (gameState->state != GAMECLOSED) {

        while (!al_is_event_queue_empty(colaEventos)) {
            al_get_next_event(colaEventos, &evento);
            switch (evento.type) {                    //Se evalua el evento ocurrido y se actua acordemente

                case ALLEGRO_EVENT_KEY_DOWN:
                    estado = PRESSED;
                    tecla = evento.keyboard.keycode;
                    break;

                case ALLEGRO_EVENT_KEY_UP:
                    estado = NOPRESSED;
                    tecla = evento.keyboard.keycode;
                    break;

            }

            if (estado == PRESSED) {
                switch (tecla) {
                    case ALLEGRO_KEY_UP:
                        storeInputEvent(DOWNARRIBA);
                        key_pressed[KEY_UP] = true;
                        break;

                    case ALLEGRO_KEY_DOWN:
                        storeInputEvent(DOWNABAJO);
                        key_pressed[KEY_DOWN] = true;
                        break;

                    case ALLEGRO_KEY_RIGHT:
                        storeInputEvent(DOWNDERECHA);
                        key_pressed[KEY_RIGHT] = true;
                        break;

                    case ALLEGRO_KEY_LEFT:
                        storeInputEvent(DOWNIZQUIERDA);
                        key_pressed[KEY_LEFT] = true;
                        break;

                    case ALLEGRO_KEY_SPACE:
                        storeInputEvent(DOWNBOTON);
                        key_pressed[KEY_SPACE] = true;
                        break;

                    case ALLEGRO_KEY_ESCAPE:
                        storeInputEvent(DOWNESCAPE);
                        key_pressed[KEY_ESCAPE] = true;
                        break;

                    case ALLEGRO_KEY_BACKSPACE:
                        storeInputEvent(DOWNBACKSPACE);
                        key_pressed[KEY_BACKSPACE] = true;
                        break;

                    case ALLEGRO_KEY_ENTER:
                        storeInputEvent(DOWNENTER);
                        key_pressed[KEY_ENTER] = true;
                        break;

                    case ALLEGRO_KEY_0:
                        storeInputEvent(DOWN0);
                        key_pressed[KEY_0] = true;
                        break;

                    case ALLEGRO_KEY_1:
                        storeInputEvent(DOWN1);
                        key_pressed[KEY_1] = true;
                        break;

                    case ALLEGRO_KEY_2:
                        storeInputEvent(DOWN2);
                        key_pressed[KEY_2] = true;
                        break;

                    case ALLEGRO_KEY_3:
                        storeInputEvent(DOWN3);
                        key_pressed[KEY_3] = true;
                        break;

                    case ALLEGRO_KEY_4:
                        storeInputEvent(DOWN4);
                        key_pressed[KEY_4] = true;
                        break;

                    case ALLEGRO_KEY_5:
                        storeInputEvent(DOWN5);
                        key_pressed[KEY_5] = true;
                        break;

                    case ALLEGRO_KEY_6:
                        storeInputEvent(DOWN6);
                        key_pressed[KEY_6] = true;
                        break;

                    case ALLEGRO_KEY_7:
                        storeInputEvent(DOWN7);
                        key_pressed[KEY_7] = true;
                        break;

                    case ALLEGRO_KEY_8:
                        storeInputEvent(DOWN8);
                        key_pressed[KEY_8] = true;
                        break;

                    case ALLEGRO_KEY_9:
                        storeInputEvent(DOWN9);
                        key_pressed[KEY_9] = true;
                        break;

                    case ALLEGRO_KEY_A:
                        storeInputEvent(DOWNA);
                        key_pressed[KEY_A] = true;
                        break;

                    case ALLEGRO_KEY_B:
                        storeInputEvent(DOWNB);
                        key_pressed[KEY_B] = true;
                        break;

                    case ALLEGRO_KEY_C:
                        storeInputEvent(DOWNC);
                        key_pressed[KEY_C] = true;
                        break;

                    case ALLEGRO_KEY_D:
                        storeInputEvent(DOWND);
                        key_pressed[KEY_D] = true;
                        break;

                    case ALLEGRO_KEY_E:
                        storeInputEvent(DOWNE);
                        key_pressed[KEY_E] = true;
                        break;

                    case ALLEGRO_KEY_F:
                        storeInputEvent(DOWNF);
                        key_pressed[KEY_F] = true;
                        break;

                    case ALLEGRO_KEY_G:
                        storeInputEvent(DOWNG);
                        key_pressed[KEY_G] = true;
                        break;

                    case ALLEGRO_KEY_H:
                        storeInputEvent(DOWNH);
                        key_pressed[KEY_H] = true;
                        break;

                    case ALLEGRO_KEY_I:
                        storeInputEvent(DOWNI);
                        key_pressed[KEY_I] = true;
                        break;

                    case ALLEGRO_KEY_J:
                        storeInputEvent(DOWNJ);
                        key_pressed[KEY_J] = true;
                        break;

                    case ALLEGRO_KEY_K:
                        storeInputEvent(DOWNK);
                        key_pressed[KEY_K] = true;
                        break;

                    case ALLEGRO_KEY_L:
                        storeInputEvent(DOWNL);
                        key_pressed[KEY_L] = true;
                        break;

                    case ALLEGRO_KEY_M:
                        storeInputEvent(DOWNM);
                        key_pressed[KEY_M] = true;
                        break;

                    case ALLEGRO_KEY_N:
                        storeInputEvent(DOWNN);
                        key_pressed[KEY_N] = true;
                        break;

                    case ALLEGRO_KEY_O:
                        storeInputEvent(DOWNO);
                        key_pressed[KEY_O] = true;
                        break;

                    case ALLEGRO_KEY_P:
                        storeInputEvent(DOWNP);
                        key_pressed[KEY_P] = true;
                        break;

                    case ALLEGRO_KEY_Q:
                        storeInputEvent(DOWNQ);
                        key_pressed[KEY_Q] = true;
                        break;

                    case ALLEGRO_KEY_R:
                        storeInputEvent(DOWNR);
                        key_pressed[KEY_R] = true;
                        break;

                    case ALLEGRO_KEY_S:
                        storeInputEvent(DOWNS);
                        key_pressed[KEY_S] = true;
                        break;

                    case ALLEGRO_KEY_T:
                        storeInputEvent(DOWNT);
                        key_pressed[KEY_T] = true;
                        break;

                    case ALLEGRO_KEY_U:
                        storeInputEvent(DOWNU);
                        key_pressed[KEY_U] = true;
                        break;

                    case ALLEGRO_KEY_V:
                        storeInputEvent(DOWNV);
                        key_pressed[KEY_V] = true;
                        break;

                    case ALLEGRO_KEY_W:
                        storeInputEvent(DOWNW);
                        key_pressed[KEY_W] = true;
                        break;

                    case ALLEGRO_KEY_X:
                        storeInputEvent(DOWNX);
                        key_pressed[KEY_X] = true;
                        break;

                    case ALLEGRO_KEY_Y:
                        storeInputEvent(DOWNY);
                        key_pressed[KEY_Y] = true;
                        break;

                    case ALLEGRO_KEY_Z:
                        storeInputEvent(DOWNZ);
                        key_pressed[KEY_Z] = true;
                        break;
                }
            } else if (estado == NOPRESSED) {

                switch (tecla) {
                    case ALLEGRO_KEY_UP:
                        storeInputEvent(UPARRIBA);
                        key_pressed[KEY_UP] = false;
                        break;

                    case ALLEGRO_KEY_DOWN:
                        storeInputEvent(UPABAJO);
                        key_pressed[KEY_DOWN] = false;
                        break;

                    case ALLEGRO_KEY_RIGHT:
                        storeInputEvent(UPDERECHA);
                        key_pressed[KEY_RIGHT] = false;
                        break;

                    case ALLEGRO_KEY_LEFT:
                        storeInputEvent(UPIZQUIERDA);
                        key_pressed[KEY_LEFT] = false;
                        break;

                    case ALLEGRO_KEY_SPACE:
                        storeInputEvent(UPBOTON);
                        key_pressed[KEY_SPACE] = false;
                        break;

                    case ALLEGRO_KEY_ESCAPE:
                        storeInputEvent(UPESCAPE);
                        key_pressed[KEY_ESCAPE] = false;
                        break;

                    case ALLEGRO_KEY_BACKSPACE:
                        storeInputEvent(UPBACKSPACE);
                        key_pressed[KEY_BACKSPACE] = false;
                        break;

                    case ALLEGRO_KEY_ENTER:
                        storeInputEvent(UPENTER);
                        key_pressed[KEY_ENTER] = false;
                        break;

                    case ALLEGRO_KEY_0:
                        storeInputEvent(UP0);
                        key_pressed[KEY_0] = false;
                        break;

                    case ALLEGRO_KEY_1:
                        storeInputEvent(UP1);
                        key_pressed[KEY_1] = false;
                        break;

                    case ALLEGRO_KEY_2:
                        storeInputEvent(UP2);
                        key_pressed[KEY_2] = false;
                        break;

                    case ALLEGRO_KEY_3:
                        storeInputEvent(UP3);
                        key_pressed[KEY_3] = false;
                        break;

                    case ALLEGRO_KEY_4:
                        storeInputEvent(UP4);
                        key_pressed[KEY_4] = false;
                        break;

                    case ALLEGRO_KEY_5:
                        storeInputEvent(UP5);
                        key_pressed[KEY_5] = false;
                        break;

                    case ALLEGRO_KEY_6:
                        storeInputEvent(UP6);
                        key_pressed[KEY_6] = false;
                        break;

                    case ALLEGRO_KEY_7:
                        storeInputEvent(UP7);
                        key_pressed[KEY_7] = false;
                        break;

                    case ALLEGRO_KEY_8:
                        storeInputEvent(UP8);
                        key_pressed[KEY_8] = false;
                        break;

                    case ALLEGRO_KEY_9:
                        storeInputEvent(UP9);
                        key_pressed[KEY_9] = false;
                        break;

                    case ALLEGRO_KEY_A:
                        storeInputEvent(UPA);
                        key_pressed[KEY_A] = false;
                        break;

                    case ALLEGRO_KEY_B:
                        storeInputEvent(UPB);
                        key_pressed[KEY_B] = false;
                        break;

                    case ALLEGRO_KEY_C:
                        storeInputEvent(UPC);
                        key_pressed[KEY_C] = false;
                        break;

                    case ALLEGRO_KEY_D:
                        storeInputEvent(UPD);
                        key_pressed[KEY_D] = false;
                        break;

                    case ALLEGRO_KEY_E:
                        storeInputEvent(UPE);
                        key_pressed[KEY_E] = false;
                        break;

                    case ALLEGRO_KEY_F:
                        storeInputEvent(UPF);
                        key_pressed[KEY_F] = false;
                        break;

                    case ALLEGRO_KEY_G:
                        storeInputEvent(UPG);
                        key_pressed[KEY_G] = false;
                        break;

                    case ALLEGRO_KEY_H:
                        storeInputEvent(UPH);
                        key_pressed[KEY_H] = false;
                        break;

                    case ALLEGRO_KEY_I:
                        storeInputEvent(UPI);
                        key_pressed[KEY_I] = false;
                        break;

                    case ALLEGRO_KEY_J:
                        storeInputEvent(UPJ);
                        key_pressed[KEY_J] = false;
                        break;

                    case ALLEGRO_KEY_K:
                        storeInputEvent(UPK);
                        key_pressed[KEY_K] = false;
                        break;

                    case ALLEGRO_KEY_L:
                        storeInputEvent(UPL);
                        key_pressed[KEY_L] = false;
                        break;

                    case ALLEGRO_KEY_M:
                        storeInputEvent(UPM);
                        key_pressed[KEY_M] = false;
                        break;

                    case ALLEGRO_KEY_N:
                        storeInputEvent(UPN);
                        key_pressed[KEY_N] = false;
                        break;

                    case ALLEGRO_KEY_O:
                        storeInputEvent(UPO);
                        key_pressed[KEY_O] = false;
                        break;

                    case ALLEGRO_KEY_P:
                        storeInputEvent(UPP);
                        key_pressed[KEY_P] = false;
                        break;

                    case ALLEGRO_KEY_Q:
                        storeInputEvent(UPQ);
                        key_pressed[KEY_Q] = false;
                        break;

                    case ALLEGRO_KEY_R:
                        storeInputEvent(UPR);
                        key_pressed[KEY_R] = false;
                        break;

                    case ALLEGRO_KEY_S:
                        storeInputEvent(UPS);
                        key_pressed[KEY_S] = false;
                        break;

                    case ALLEGRO_KEY_T:
                        storeInputEvent(UPT);
                        key_pressed[KEY_T] = false;
                        break;

                    case ALLEGRO_KEY_U:
                        storeInputEvent(UPU);
                        key_pressed[KEY_U] = false;
                        break;

                    case ALLEGRO_KEY_V:
                        storeInputEvent(UPV);
                        key_pressed[KEY_V] = false;
                        break;

                    case ALLEGRO_KEY_W:
                        storeInputEvent(UPW);
                        key_pressed[KEY_W] = false;
                        break;

                    case ALLEGRO_KEY_X:
                        storeInputEvent(UPX);
                        key_pressed[KEY_X] = false;
                        break;

                    case ALLEGRO_KEY_Y:
                        storeInputEvent(UPY);
                        key_pressed[KEY_Y] = false;
                        break;

                    case ALLEGRO_KEY_Z:
                        storeInputEvent(UPZ);
                        key_pressed[KEY_Z] = false;
                        break;
                }
            }
            estado = -1;
        }
    }

    pthread_exit(NULL);
}

#endif
