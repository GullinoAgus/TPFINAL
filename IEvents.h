#include <stdbool.h>
#include "matiasBrosGame.h"
#define MAXIMOEVENTOSBUFFER 40
#define LIMITEMOVDEPALANCA 100
#define VACIO 0
#define NOPRESSED 0
#define PRESSED 1

enum { NOHAYMOVIMIENTO = 50, UPDERECHA, DOWNDERECHA, UPABAJODERECHA, DOWNABAJODERECHA, UPABAJO, DOWNABAJO, UPABAJOIZQUIERDA, DOWNABAJOIZQUIERDA,
        UPIZQUIERDA, DOWNIZQUIERDA, UPARRIBAIZQUIERDA, DOWNARRIBAIZQUIERDA, UPARRIBA, DOWNARRIBA, UPARRIBADERECHA, DOWNARRIBADERECHA, UPBOTON, DOWNBOTON };


char getInputEvent (void); /* ESTA ES LA FUNCION POR LA CUAL SE LEERAN LOS EVENTOS DE ENTRADA DEL BUFFER, LAS CORRESPONDIENTES SE IDENTIFICAN CON LAS
 CONSTANTES DE ARRIBA */

void * InputEvent(void *); //ESTA ES LA FUNCION QUE SE VA A ASOCIAR AL THREAD CORRESPONDIENTE

void storeInputEvent (char evento); //ESTA FUNCION GUARDA UN EVENTO EN EL BUFFER, PUEDE SER UTIL PUBLICARLA PARA FORZARLE EVENTOS EXTERNOS AL JOYSTICK

int esBufferVacio (void); //DEVUELVE 1 EN EL CASO DE QUE EL BUFFER ESTE VACIO, 1 EN EL CASO CONTRARIO

void * keyboardChanges (void* UnusedP);

int mouseChanges(bool estado, int evMouseX, int evMouseY);