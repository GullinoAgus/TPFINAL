#include "IEvents.h"
#include "raspi.h"
#include "allegro.h"
#include <pthread.h>

static char inputBuffer [MAXIMOEVENTOSBUFFER] = {0}; //He aqui el buffer de eventos
static char* bufferPointer = inputBuffer;

#if MODOJUEGO == 0
enum keys {KEY_0 = 0,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8,KEY_9,KEY_A,KEY_B,KEY_C,KEY_D,KEY_E,KEY_F,KEY_G,KEY_H,KEY_I,KEY_J,KEY_K,KEY_L,KEY_M,KEY_N,
    KEY_ENIE,KEY_O,KEY_P,KEY_Q,KEY_R,KEY_S,KEY_T,KEY_U,KEY_V,KEY_W,KEY_X,KEY_Y,KEY_Z,KEY_LEFT,KEY_RIGHT,KEY_UP,KEY_DOWN,KEY_SPACE,KEY_ESCAPE};

int a = 0;
static bool key_pressed[KEY_ESCAPE+1];
#endif

char getInputEvent (void){

    char salida;
    if (bufferPointer <= inputBuffer + MAXIMOEVENTOSBUFFER && bufferPointer > inputBuffer){

        salida = *bufferPointer;
        *bufferPointer = VACIO;
        bufferPointer--;

    }
    else{

        salida = *bufferPointer;
        *bufferPointer = VACIO;
        bufferPointer = inputBuffer + MAXIMOEVENTOSBUFFER;
    }

    return salida;
}
void storeInputEvent (char evento){
    static int i = 0;
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
    int salida = 1; //asumo que el buffer esta vacio hasta que encuentre un evento

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

#if MODOJUEGO == 1

void * InputEvent(void * UnusedP) {
    jcoord_t myCoords;      //He aqui las coordenadas del joystick
    joy_init();                //inicializo el joystick
    char ultimoEvento = VACIO;
    char ultimoEventoBoton = VACIO;
    while (1) { //ACA SERIA MEJOR SETEARLO CON EL TIMER, O PONER UN DELAY EN EL CODIGO
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


#endif

#if MODOJUEGO == 0

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
void * keyboardChanges (void* myGameState){

    int tecla;
    int estado = 2;

    estadoJuego_t *gameState = (estadoJuego_t*) myGameState;

    ALLEGRO_EVENT_QUEUE * colaEventos = al_create_event_queue();  //genero mi cola de eventos
    al_register_event_source(colaEventos, al_get_keyboard_event_source());

    ALLEGRO_EVENT evento;

    while (gameState->state != GAMECLOSED) {

        usleep(UTIEMPOREFRESCO);

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
                        if (key_pressed[KEY_UP] == false)
                            storeInputEvent(DOWNARRIBA);
                        key_pressed[KEY_UP] = true;
                        break;

                    case ALLEGRO_KEY_DOWN:
                        if (key_pressed[KEY_DOWN] == false)
                            storeInputEvent(DOWNABAJO);
                        key_pressed[KEY_DOWN] = true;
                        break;

                    case ALLEGRO_KEY_RIGHT:
                        if (key_pressed[KEY_RIGHT] == false)
                            storeInputEvent(DOWNDERECHA);
                        key_pressed[KEY_RIGHT] = true;
                        break;

                    case ALLEGRO_KEY_LEFT:
                        if (key_pressed[KEY_LEFT] == false)
                            storeInputEvent(DOWNIZQUIERDA);
                        key_pressed[KEY_LEFT] = true;
                        break;

                    case ALLEGRO_KEY_SPACE:
                        if (key_pressed[KEY_SPACE] == false)
                            storeInputEvent(DOWNBOTON);
                        key_pressed[KEY_SPACE] = true;
                        break;

                    case ALLEGRO_KEY_0:
                        if (key_pressed[KEY_0] == false)
                            a++;
                        key_pressed[KEY_0] = true;
                        break;

                    case ALLEGRO_KEY_1:
                        if (key_pressed[KEY_1] == false)
                            a++;
                        key_pressed[KEY_1] = true;
                        break;

                    case ALLEGRO_KEY_2:
                        if (key_pressed[KEY_2] == false)
                            a++;
                        key_pressed[KEY_2] = true;
                        break;

                    case ALLEGRO_KEY_3:
                        if (key_pressed[KEY_3] == false)
                            a++;
                        key_pressed[KEY_3] = true;
                        break;

                    case ALLEGRO_KEY_4:
                        if (key_pressed[KEY_4] == false)
                            a++;
                        key_pressed[KEY_4] = true;
                        break;

                    case ALLEGRO_KEY_5:
                        if (key_pressed[KEY_5] == false)
                            a++;
                        key_pressed[KEY_5] = true;
                        break;

                    case ALLEGRO_KEY_6:
                        if (key_pressed[KEY_6] == false)
                            a++;
                        key_pressed[KEY_6] = true;
                        break;

                    case ALLEGRO_KEY_7:
                        if (key_pressed[KEY_7] == false)
                            a++;
                        key_pressed[KEY_7] = true;
                        break;

                    case ALLEGRO_KEY_Q:
                        if (key_pressed[KEY_Q] == false)
                            a++;
                        key_pressed[KEY_Q] = true;
                        break;

                    case ALLEGRO_KEY_S:
                        if (key_pressed[KEY_S] == false)
                            a++;
                        key_pressed[KEY_S] = true;
                        break;

                    case ALLEGRO_KEY_P:
                        if (key_pressed[KEY_P] == false) {
                            storeInputEvent(DOWNP);
                        }
                        key_pressed[KEY_P] = true;
                        break;

                    case ALLEGRO_KEY_C:
                        if (key_pressed[KEY_C] == false)
                            a++;
                        key_pressed[KEY_C] = true;
                        break;

                    case ALLEGRO_KEY_T:
                        if (key_pressed[KEY_T] == false)
                            a++;
                        key_pressed[KEY_T] = true;
                        break;

                    case ALLEGRO_KEY_A:
                        if (key_pressed[KEY_A] == false)
                            a++;
                        key_pressed[KEY_A] = true;
                        break;

                    case ALLEGRO_KEY_B:
                        if (key_pressed[KEY_B] == false)
                            a++;
                        key_pressed[KEY_B] = true;
                        break;
                    case ALLEGRO_KEY_ESCAPE:
                        if (key_pressed[KEY_ESCAPE] == false) {
                            storeInputEvent(DOWNESCAPE);
                        }
                        key_pressed[KEY_ESCAPE] = true;
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

                    case ALLEGRO_KEY_0:
                        key_pressed[KEY_0] = false;
                        break;

                    case ALLEGRO_KEY_1:
                        key_pressed[KEY_1] = false;
                        break;

                    case ALLEGRO_KEY_2:
                        key_pressed[KEY_2] = false;
                        break;

                    case ALLEGRO_KEY_3:
                        key_pressed[KEY_3] = false;
                        break;

                    case ALLEGRO_KEY_4:
                        key_pressed[KEY_4] = false;
                        break;

                    case ALLEGRO_KEY_5:
                        key_pressed[KEY_5] = false;
                        break;

                    case ALLEGRO_KEY_6:
                        key_pressed[KEY_6] = false;
                        break;

                    case ALLEGRO_KEY_7:
                        key_pressed[KEY_7] = false;
                        break;

                    case ALLEGRO_KEY_Q:
                        key_pressed[KEY_Q] = false;
                        break;

                    case ALLEGRO_KEY_S:
                        key_pressed[KEY_S] = false;
                        break;

                    case ALLEGRO_KEY_P:
                        storeInputEvent(UPP);
                        key_pressed[KEY_P] = false;
                        break;

                    case ALLEGRO_KEY_C:
                        key_pressed[KEY_C] = false;
                        break;

                    case ALLEGRO_KEY_T:
                        key_pressed[KEY_T] = false;
                        break;

                    case ALLEGRO_KEY_A:
                        key_pressed[KEY_A] = false;
                        break;

                    case ALLEGRO_KEY_B:
                        key_pressed[KEY_B] = false;
                        break;

                    case ALLEGRO_KEY_ESCAPE:
                        storeInputEvent(UPSCAPE);
                        key_pressed[KEY_ESCAPE] = false;
                        break;
                }
            }
        }
    }

    pthread_exit(NULL);
}
#endif
