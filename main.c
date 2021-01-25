/***************************************************************************//**
  @file     main.c
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "matiasBrosGame.h"
#include "IEvents.h"
#include "menu.h"
#include <unistd.h>
#include <pthread.h>
#include "gamelogic.h"
#include "render.h"
#include "times.h"

#if MODOJUEGO == ALLEGRO

#include "animacion.h"
#include "allegroLib.h"

#elif MODOJUEGO == RASPI

#include "data.h"

#endif

/*******************************************************************************
 *******************************************************************************
                        MAIN FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

#if MODOJUEGO == ALLEGRO

int main(int argv, char** arg) {
    estadoJuego_t gameState;
    gameState.state = LOADINGGAME;
    pthread_t eventoTeclado, gameLogic, renderizar;


    //Inicializamos allegro, los recursos del juego y verificamos que se haya hecho correctamente
    if(inicializarAllegro() == 1) {
        return 1;
    }

    if(SDL_Init(SDL_INIT_AUDIO) != 0){
        return 1;
    }
    initAudio();

    gameState.buffer.imageQuant = cargarTexturasMenu(&gameState.buffer.image);
    if(gameState.buffer.imageQuant == -1) {
        destroyResources(&gameState.buffer);
        return -1;
    }

    gameState.buffer.soundQuant = cargarSonidosMenu(&gameState.buffer.sound);
    if(gameState.buffer.soundQuant == -1) {
        destroyResources(&gameState.buffer);
        return -1;
    }

    gameState.buffer.fontQuant = cargarFuentesMenu(&gameState.buffer.font);
    if(gameState.buffer.fontQuant == -1){
        destroyResources(&gameState.buffer);
        return -1;
    }

    if(loadGameState(&gameState) == 1) {
        destroyResources(&gameState.buffer);
        return 1;
    }

    if(loadMenuData() == 1){
        printf("Error al cargar la data del menu");
        return 1;
    }

    //Lanzamos los threads del juego
    pthread_create(&gameLogic, NULL, gamelogic, &gameState);
    pthread_create(&eventoTeclado, NULL, keyboardChanges, &gameState);
    pthread_create(&renderizar, NULL, render, &gameState);

    //Esperamos a que terminen los threads
    pthread_join(eventoTeclado, NULL);
    pthread_join(gameLogic, NULL);
    pthread_join(renderizar, NULL);

    destroyAllTimers();
    destroyResources(&gameState.buffer);
    destroyMenu();

    return 0;
}

#elif MODOJUEGO == RASPI

int main (void){

    estadoJuego_t gameState;
    pthread_t EventoJoy, renderizar, gameLogic;

    disp_init();				//inicializa el display
    disp_clear();				//limpia el display
    disp_update();              //muestra en pantalla el display limpito

    joy_init();                 //inicializa el joystick

    if(SDL_Init(SDL_INIT_AUDIO) != 0){
        return 1;
    }
    initAudio();

    gameState.buffer.soundQuant = cargarSonidosMenu(&gameState.buffer.sound);
    if(gameState.buffer.soundQuant == -1) {
        destroyResources(&gameState.buffer);
        return -1;
    }

    if(loadGameState(&gameState) == 1) {
        printf("Error al cargar los datos del juego");
        return -1;
    }

    pthread_create(&EventoJoy, NULL, InputEvent, &gameState);
    pthread_create(&renderizar, NULL, render, &gameState);
    pthread_create(&gameLogic, NULL, gamelogic, &gameState);

    pthread_join(EventoJoy, NULL);
    pthread_join(renderizar, NULL);
    pthread_join(gameLogic, NULL);

    disp_clear();
    disp_update();

    destroyAllTimers();
}

#endif