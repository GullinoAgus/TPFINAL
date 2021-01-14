
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
        UPIZQUIERDA, DOWNIZQUIERDA, UPARRIBAIZQUIERDA, DOWNARRIBAIZQUIERDA, UPARRIBA, DOWNARRIBA, UPARRIBADERECHA, DOWNARRIBADERECHA, UPBOTON, DOWNBOTON, DOWNESCAPE, UPESCAPE, NUMOFEVENTKEYS};

/*Constantes que servirian para en un futuro guardar en el buffer las letras y los numeros*/
enum { DOWNA = NUMOFEVENTKEYS, UPA, DOWNB, UPB, DOWNC, UPC, DOWND, UPD, DOWNE, UPE, DOWNF, UPF, DOWNG, UPG, DOWNH, UPH, DOWNI, UPI, DOWNJ, UPJ, DOWNK, UPK, DOWNL, UPL, DOWNM, UPM, DOWNN, UPN,
      DOWNO, UPO, DOWNP, UPP, DOWNQ, UPQ, DOWNR, UPR, DOWNS, UPS, DOWNT, UPT, DOWNU, UPU, DOWNV, UPV, DOWNW, UPW, DOWNX, UPX, DOWNY, UPY, DOWNZ, UPZ, DOWNENTER, UPENTER, DOWNBACKSPACE, UPBACKSPACE, DOWN0, UP0,
      DOWN1, UP1, DOWN2, UP2, DOWN3, UP3, DOWN4, UP4, DOWN5, UP5, DOWN6, UP6, DOWN7, UP7, DOWN8, UP8, DOWN9, UP9 };


char getInputEvent (void); /* ESTA ES LA FUNCION POR LA CUAL SE LEERAN LOS EVENTOS DE ENTRADA DEL BUFFER, LAS CORRESPONDIENTES SE IDENTIFICAN CON LAS
 CONSTANTES DE ARRIBA */

void * InputEvent(void *); //ESTA ES LA FUNCION QUE SE VA A ASOCIAR AL THREAD PARA CORRER EN LA RASPI, ES COMO EL KEYBOARD CHANGES DE INPUT EVENT

void storeInputEvent (char evento); //ESTA FUNCION GUARDA UN EVENTO EN EL BUFFER, PUEDE SER UTIL PUBLICARLA PARA FORZARLE EVENTOS EXTERNOS AL JOYSTICK

int esBufferVacio (void); //DEVUELVE 1 EN EL CASO DE QUE EL BUFFER ESTE VACIO, 0 EN EL CASO CONTRARIO

void * keyboardChanges (void* UnusedP);
/* Este es el thread que registra todos los eventos de teclado, los guarda en el buffer que declaramos arriba usando store input event*/

int mouseChanges(bool estado, int evMouseX, int evMouseY);
/*En principio no sirve de mucho, es medio el template para el thread que se viene de mouseChanges*/

void limpiarBuffer (void);

#endif //IEvents_h
