
#ifndef IEvents_h
#define IEvents_h

#include <stdbool.h>
#include "matiasBrosGame.h"

#define MAXIMOEVENTOSBUFFER 40
#define LIMITEMOVDEPALANCA 100  //Este es el threshold para el cual se detecta el movimiento de la palanca
#define VACIO 0
#define NOPRESSED 0
#define PRESSED 1

/*Constantes para el buffer de eventos */
enum { NOHAYMOVIMIENTO = 50, UPDERECHA, DOWNDERECHA, UPABAJODERECHA, DOWNABAJODERECHA, UPABAJO, DOWNABAJO, UPABAJOIZQUIERDA, DOWNABAJOIZQUIERDA,
        UPIZQUIERDA, DOWNIZQUIERDA, UPARRIBAIZQUIERDA, DOWNARRIBAIZQUIERDA, UPARRIBA, DOWNARRIBA, UPARRIBADERECHA, DOWNARRIBADERECHA, UPBOTON, DOWNBOTON, DOWNESCAPE, UPSCAPE, };

/*Constantes que servirian para en un futuro guardar en el buffer las letras y los numeros*/
enum { DOWNA=101, DOWNB, DOWNC, DOWND, DOWNE, DOWNF, DOWNG, DOWNH, DOWNI, DOWNJ, DOWNK, DOWNL, DOWNM, DOWNN,
      DOWNO, DOWNP,UPP, DOWNQ, DOWNR, DOWNS, DOWNT, DOWNU, DOWNV, DOWNW, DOWNX, DOWNY, DOWNZ, DOWNENTER, DOWN0,
      DOWN1, DOWN2, DOWN3, DOWN4, DOWN5, DOWN6, DOWN7, DOWN8, DOWN9 };


char getInputEvent (void); /* ESTA ES LA FUNCION POR LA CUAL SE LEERAN LOS EVENTOS DE ENTRADA DEL BUFFER, LAS CORRESPONDIENTES SE IDENTIFICAN CON LAS
 CONSTANTES DE ARRIBA */

void * InputEvent(void *); //ESTA ES LA FUNCION QUE SE VA A ASOCIAR AL THREAD PARA CORRER EN LA RASPI, ES COMO EL KEYBOARD CHANGES DE INPUT EVENT

void storeInputEvent (char evento); //ESTA FUNCION GUARDA UN EVENTO EN EL BUFFER, PUEDE SER UTIL PUBLICARLA PARA FORZARLE EVENTOS EXTERNOS AL JOYSTICK

int esBufferVacio (void); //DEVUELVE 1 EN EL CASO DE QUE EL BUFFER ESTE VACIO, 0 EN EL CASO CONTRARIO

void * keyboardChanges (void* UnusedP);
/* Este es el thread que registra todos los eventos de teclado, los guarda en el buffer que declaramos arriba usando store input event*/

int mouseChanges(bool estado, int evMouseX, int evMouseY);
/*En principio no sirve de mucho, es medio el template para el thread que se viene de mouseChanges*/

void actualizarDisplay(char matriz[16][16] ); //ESTA FUNCION CONTROLA EL BUFFER  DEL DISPLAY A PARTIR DE LA MATRIZ DADA
// SI EL ELEMENTO EN LA MATRIZ VALE 0, SE APAGA EL LED CORRESPONDIENTE Y
// SI VALE 1 SE LO PRENDE
// EN CUALQUIER OTRO CASO, SE LO DEJA IGUAL QUE ANTES

void limpiarBuffer (void);

#endif //IEvents_h
