/***************************************************************************//**
  file      IEvents.c
  @brief    Interpretacion de eventos de teclado y joystick
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "IEvents.h"
#include <pthread.h>

#if MODOJUEGO == ALLEGRO

#include "allegro.h"

#elif MODOJUEGO == RASPI

#include "raspi.h"

#endif

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static unsigned char inputBuffer [MAXIMOEVENTOSBUFFER] = {0}; //He aqui el buffer circular de eventos
/*Todos los eventos que se ingresen en cualquier modo se identifican y luego se guardan aca para poder ser interpretados*/

static unsigned char* bufferPointer = inputBuffer;  //El puntero de lectura y escritura del buffer

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

unsigned char getInputEvent (void){

    unsigned char salida;
    if (*bufferPointer == VACIO){   // Si no hay nada escrito simplemente devuelvo eso
        salida = VACIO;
    } else {
        if (bufferPointer <= inputBuffer + MAXIMOEVENTOSBUFFER && bufferPointer > inputBuffer) { //Si todavia no llegue al final
            salida = *bufferPointer;
            *bufferPointer = VACIO;
            bufferPointer--;
        } else {    //Una vez llego al "final" del buffer, empiezo de vuelta y sigo
            salida = *bufferPointer;
            *bufferPointer = VACIO;
            bufferPointer = inputBuffer + MAXIMOEVENTOSBUFFER;
        }
    }
    return salida;
}

void storeInputEvent (unsigned char evento){

    if (bufferPointer >= inputBuffer && bufferPointer < inputBuffer + MAXIMOEVENTOSBUFFER){ //Si todavia no llegue al final

        bufferPointer++;
        *bufferPointer = evento;
    }
    else{               //Una vez llego al "final" del buffer, empiezo de vuelta y sigo
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
        inputBuffer[i] = VACIO;
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
        /* Aqui es donde se realiza la traduccion del evento del joystick al buffer*/

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
                switch (tecla) {        //Aqui es donde se realiza la traduccion del evento de allegro al buffer
                    case ALLEGRO_KEY_UP:
                        storeInputEvent(DOWNARRIBA);
                        break;

                    case ALLEGRO_KEY_DOWN:
                        storeInputEvent(DOWNABAJO);
                        break;

                    case ALLEGRO_KEY_RIGHT:
                        storeInputEvent(DOWNDERECHA);
                        break;

                    case ALLEGRO_KEY_LEFT:
                        storeInputEvent(DOWNIZQUIERDA);
                        break;

                    case ALLEGRO_KEY_SPACE:
                        storeInputEvent(DOWNBOTON);
                        break;

                    case ALLEGRO_KEY_ESCAPE:
                        storeInputEvent(DOWNESCAPE);
                        break;

                    case ALLEGRO_KEY_BACKSPACE:
                        storeInputEvent(DOWNBACKSPACE);
                        break;

                    case ALLEGRO_KEY_ENTER:
                        storeInputEvent(DOWNENTER);
                        break;

                    case ALLEGRO_KEY_0:
                        storeInputEvent(DOWN0);
                        break;

                    case ALLEGRO_KEY_1:
                        storeInputEvent(DOWN1);
                        break;

                    case ALLEGRO_KEY_2:
                        storeInputEvent(DOWN2);
                        break;

                    case ALLEGRO_KEY_3:
                        storeInputEvent(DOWN3);
                        break;

                    case ALLEGRO_KEY_4:
                        storeInputEvent(DOWN4);
                        break;

                    case ALLEGRO_KEY_5:
                        storeInputEvent(DOWN5);
                        break;

                    case ALLEGRO_KEY_6:
                        storeInputEvent(DOWN6);
                        break;

                    case ALLEGRO_KEY_7:
                        storeInputEvent(DOWN7);
                        break;

                    case ALLEGRO_KEY_8:
                        storeInputEvent(DOWN8);
                        break;

                    case ALLEGRO_KEY_9:
                        storeInputEvent(DOWN9);
                        break;

                    case ALLEGRO_KEY_A:
                        storeInputEvent(DOWNA);
                        break;

                    case ALLEGRO_KEY_B:
                        storeInputEvent(DOWNB);
                        break;

                    case ALLEGRO_KEY_C:
                        storeInputEvent(DOWNC);
                        break;

                    case ALLEGRO_KEY_D:
                        storeInputEvent(DOWND);
                        break;

                    case ALLEGRO_KEY_E:
                        storeInputEvent(DOWNE);
                        break;

                    case ALLEGRO_KEY_F:
                        storeInputEvent(DOWNF);
                        break;

                    case ALLEGRO_KEY_G:
                        storeInputEvent(DOWNG);
                        break;

                    case ALLEGRO_KEY_H:
                        storeInputEvent(DOWNH);
                        break;

                    case ALLEGRO_KEY_I:
                        storeInputEvent(DOWNI);
                        break;

                    case ALLEGRO_KEY_J:
                        storeInputEvent(DOWNJ);
                        break;

                    case ALLEGRO_KEY_K:
                        storeInputEvent(DOWNK);
                        break;

                    case ALLEGRO_KEY_L:
                        storeInputEvent(DOWNL);
                        break;

                    case ALLEGRO_KEY_M:
                        storeInputEvent(DOWNM);
                        break;

                    case ALLEGRO_KEY_N:
                        storeInputEvent(DOWNN);
                        break;

                    case ALLEGRO_KEY_O:
                        storeInputEvent(DOWNO);
                        break;

                    case ALLEGRO_KEY_P:
                        storeInputEvent(DOWNP);
                        break;

                    case ALLEGRO_KEY_Q:
                        storeInputEvent(DOWNQ);
                        break;

                    case ALLEGRO_KEY_R:
                        storeInputEvent(DOWNR);
                        break;

                    case ALLEGRO_KEY_S:
                        storeInputEvent(DOWNS);
                        break;

                    case ALLEGRO_KEY_T:
                        storeInputEvent(DOWNT);
                        break;

                    case ALLEGRO_KEY_U:
                        storeInputEvent(DOWNU);
                        break;

                    case ALLEGRO_KEY_V:
                        storeInputEvent(DOWNV);
                        break;

                    case ALLEGRO_KEY_W:
                        storeInputEvent(DOWNW);
                        break;

                    case ALLEGRO_KEY_X:
                        storeInputEvent(DOWNX);
                        break;

                    case ALLEGRO_KEY_Y:
                        storeInputEvent(DOWNY);
                        break;

                    case ALLEGRO_KEY_Z:
                        storeInputEvent(DOWNZ);
                        break;
                    default:
                        break;
                }
            } else if (estado == NOPRESSED) {   //Aqui es donde se realiza la traduccion del evento de allegro al buffer

                switch (tecla) {
                    case ALLEGRO_KEY_UP:
                        storeInputEvent(UPARRIBA);
                        break;

                    case ALLEGRO_KEY_DOWN:
                        storeInputEvent(UPABAJO);
                        break;

                    case ALLEGRO_KEY_RIGHT:
                        storeInputEvent(UPDERECHA);
                        break;

                    case ALLEGRO_KEY_LEFT:
                        storeInputEvent(UPIZQUIERDA);
                        break;

                    case ALLEGRO_KEY_SPACE:
                        storeInputEvent(UPBOTON);
                        break;

                    case ALLEGRO_KEY_ESCAPE:
                        storeInputEvent(UPESCAPE);
                        break;

                    case ALLEGRO_KEY_BACKSPACE:
                        storeInputEvent(UPBACKSPACE);
                        break;

                    case ALLEGRO_KEY_ENTER:
                        storeInputEvent(UPENTER);
                        break;

                    case ALLEGRO_KEY_0:
                        storeInputEvent(UP0);
                        break;

                    case ALLEGRO_KEY_1:
                        storeInputEvent(UP1);
                        break;

                    case ALLEGRO_KEY_2:
                        storeInputEvent(UP2);
                        break;

                    case ALLEGRO_KEY_3:
                        storeInputEvent(UP3);
                        break;

                    case ALLEGRO_KEY_4:
                        storeInputEvent(UP4);
                        break;

                    case ALLEGRO_KEY_5:
                        storeInputEvent(UP5);
                        break;

                    case ALLEGRO_KEY_6:
                        storeInputEvent(UP6);
                        break;

                    case ALLEGRO_KEY_7:
                        storeInputEvent(UP7);
                        break;

                    case ALLEGRO_KEY_8:
                        storeInputEvent(UP8);
                        break;

                    case ALLEGRO_KEY_9:
                        storeInputEvent(UP9);
                        break;

                    case ALLEGRO_KEY_A:
                        storeInputEvent(UPA);
                        break;

                    case ALLEGRO_KEY_B:
                        storeInputEvent(UPB);
                        break;

                    case ALLEGRO_KEY_C:
                        storeInputEvent(UPC);
                        break;

                    case ALLEGRO_KEY_D:
                        storeInputEvent(UPD);
                        break;

                    case ALLEGRO_KEY_E:
                        storeInputEvent(UPE);
                        break;

                    case ALLEGRO_KEY_F:
                        storeInputEvent(UPF);
                        break;

                    case ALLEGRO_KEY_G:
                        storeInputEvent(UPG);
                        break;

                    case ALLEGRO_KEY_H:
                        storeInputEvent(UPH);
                        break;

                    case ALLEGRO_KEY_I:
                        storeInputEvent(UPI);
                        break;

                    case ALLEGRO_KEY_J:
                        storeInputEvent(UPJ);
                        break;

                    case ALLEGRO_KEY_K:
                        storeInputEvent(UPK);
                        break;

                    case ALLEGRO_KEY_L:
                        storeInputEvent(UPL);
                        break;

                    case ALLEGRO_KEY_M:
                        storeInputEvent(UPM);
                        break;

                    case ALLEGRO_KEY_N:
                        storeInputEvent(UPN);
                        break;

                    case ALLEGRO_KEY_O:
                        storeInputEvent(UPO);
                        break;

                    case ALLEGRO_KEY_P:
                        storeInputEvent(UPP);
                        break;

                    case ALLEGRO_KEY_Q:
                        storeInputEvent(UPQ);
                        break;

                    case ALLEGRO_KEY_R:
                        storeInputEvent(UPR);
                        break;

                    case ALLEGRO_KEY_S:
                        storeInputEvent(UPABAJO);
                        storeInputEvent(UPS);
                        break;

                    case ALLEGRO_KEY_T:
                        storeInputEvent(UPT);
                        break;

                    case ALLEGRO_KEY_U:
                        storeInputEvent(UPU);
                        break;

                    case ALLEGRO_KEY_V:
                        storeInputEvent(UPV);
                        break;

                    case ALLEGRO_KEY_W:
                        storeInputEvent(UPW);
                        break;

                    case ALLEGRO_KEY_X:
                        storeInputEvent(UPX);
                        break;

                    case ALLEGRO_KEY_Y:
                        storeInputEvent(UPY);
                        break;

                    case ALLEGRO_KEY_Z:
                        storeInputEvent(UPZ);
                        break;
                    default:
                        break;
                }
            }
            estado = -1;
        }
    }

    pthread_exit(NULL);
}

#endif
