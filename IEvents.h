/***************************************************************************//**
  @file     IEvents.h
  @brief    Interpretacion de eventos de teclado y joystick.
 ******************************************************************************/

#ifndef IEvents_h
#define IEvents_h

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdbool.h>
#include "matiasBrosGame.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define MAXIMOEVENTOSBUFFER 40
#define LIMITEMOVDEPALANCA 100  //Este es el threshold para el cual se detecta el movimiento de la palanca
#define VACIO 0                 //Evento nulo del buffer
#define NOPRESSED 0
#define PRESSED 1

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/* Constantes para el buffer de eventos */
enum { NOHAYMOVIMIENTO = 50, UPDERECHA, DOWNDERECHA, UPABAJODERECHA, DOWNABAJODERECHA, UPABAJO, DOWNABAJO, UPABAJOIZQUIERDA, DOWNABAJOIZQUIERDA,
        UPIZQUIERDA, DOWNIZQUIERDA, UPARRIBAIZQUIERDA, DOWNARRIBAIZQUIERDA, UPARRIBA, DOWNARRIBA, UPARRIBADERECHA, DOWNARRIBADERECHA, UPBOTON, DOWNBOTON, DOWNESCAPE, UPESCAPE, NUMOFEVENTKEYS};

/* Constantes que sirven para guardar en el buffer las letras y los numeros */
enum { DOWNA = NUMOFEVENTKEYS, UPA, DOWNB, UPB, DOWNC, UPC, DOWND, UPD, DOWNE, UPE, DOWNF, UPF, DOWNG, UPG, DOWNH, UPH, DOWNI, UPI, DOWNJ, UPJ, DOWNK, UPK, DOWNL, UPL, DOWNM, UPM, DOWNN, UPN,
      DOWNO, UPO, DOWNP, UPP, DOWNQ, UPQ, DOWNR, UPR, DOWNS, UPS, DOWNT, UPT, DOWNU, UPU, DOWNV, UPV, DOWNW, UPW, DOWNX, UPX, DOWNY, UPY, DOWNZ, UPZ, DOWN0, UP0,
      DOWN1, UP1, DOWN2, UP2, DOWN3, UP3, DOWN4, UP4, DOWN5, UP5, DOWN6, UP6, DOWN7, UP7, DOWN8, UP8, DOWN9, UP9, DOWNENTER, UPENTER, DOWNBACKSPACE, UPBACKSPACE};

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

#if MODOJUEGO == ALLEGRO

/**
 * @brief Esta es la funcion que hace al thread que registra todos los eventos de teclado y los guarda en el buffer
 * @param Un puntero a la estructura gameState
 * @return puntero a void que no se utiliza
*/
void* keyboardChanges (void* myGameState);

#elif MODOJUEGO == RASPI

/**
 * @brief Esta es la funcion que hace al thread que registra todos los eventos del joystick y los guarda en el buffer
 * @param gs Un puntero a la estructura gameState
 * @return Puntero a void que no se utiliza
*/
void * InputEvent(void* gs);

#endif

/**
 * @brief Lee el siguiente elemento del buffer
 * @return Devuelve el evento leido del buffer, el cual sera alguna de las constantes ya definidas en este archivo
*/
unsigned char getInputEvent (void);

/**
 * @brief Se encarga de guardar los eventos que los threads leen en el buffer
 * @param evento El evento a guardar en el buffer, el cual sera alguna de las constantes ya definidas en este archivo
*/
void storeInputEvent (unsigned char evento);

//TODO: Borrar esBufferVacio si todavia no se usa antes de entregarlo

/**
 * @brief Checkea si el buffer esta vacio
 * @return Devuelve 1 en el caso de que el buffer este vacio, y 0 en el caso contrario
*/
int esBufferVacio (void);

/**
 * @brief Limpia el buffer de eventos
*/
void limpiarBuffer (void);

#endif //IEvents_h
