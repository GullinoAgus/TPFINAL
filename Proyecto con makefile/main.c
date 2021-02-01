/***************************************************************************//**
  @file     main.c
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "matiasBrosGame.h"
#include "menu.h"
#include <unistd.h>
#include <pthread.h>


#if MODOJUEGO == ALLEGRO

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

int main(void) {

    estadoJuego_t gameState;
    pthread_t eventoTeclado, gameLogic, renderizar;


    //Inicializamos allegro, los recursos del juego y verificamos que se haya hecho correctamente
    if(inicializarAllegro() == 1) {
        return 1;
    }

    // Inicializamos el audio
    if(SDL_Init(SDL_INIT_AUDIO) != 0){
        return 1;
    }
    initAudio();

    //Cargamos las texturas del menu
    gameState.buffer.imageQuant = cargarTexturasMenu(&gameState.buffer.image);
    if(gameState.buffer.imageQuant == -1) {
        destroyResources(&gameState.buffer);
        return -1;
    }

    //Cargamos los sonidos
    gameState.buffer.soundQuant = cargarSonidosMenu(&gameState.buffer.sound);
    if(gameState.buffer.soundQuant == -1) {
        destroyResources(&gameState.buffer);
        return -1;
    }

    //Cargamos las fuentes
    gameState.buffer.fontQuant = cargarFuentesMenu(&gameState.buffer.font);
    if(gameState.buffer.fontQuant == -1){
        destroyResources(&gameState.buffer);
        return -1;
    }

    //Cargamos los highscores
    if(loadGameState(&gameState) == 1) {
        destroyResources(&gameState.buffer);
        return 1;
    }

    //Cargamos datos de imagenes y texto del menu
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

    //Inicializamos el audio
    if(SDL_Init(SDL_INIT_AUDIO) != 0){
        return 1;
    }
    initAudio();

    //Cargamos los sonidos
    gameState.buffer.soundQuant = cargarSonidosMenu(&gameState.buffer.sound);
    if(gameState.buffer.soundQuant == -1) {
        destroyResources(&gameState.buffer);
        return -1;
    }

    //Cargamos los highscores
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