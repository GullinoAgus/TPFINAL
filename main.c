#include "matiasBrosGame.h"
#include "IEvents.h"
#include "allegroLib.h"
#include "menu.h"
#include <unistd.h>
#include <pthread.h>
#include "level.h"
#include "gamelogic.h"
#include "animacion.h"
#include "render.h"

#if MODOJUEGO == 0

int main(void) {

    int closed = 0;
    ALLEGRO_DISPLAY* disp;
    estadoJuego_t gameState;
    pthread_t eventoTeclado, fisicas, gameLogic, animaciones, renderizar;


    //Inicializamos allegro, los recursos del juego y verificamos que se haya hecho correctamente
    if(inicializarAllegro(disp) == 1) {
        al_destroy_display(disp);
        return 1;
    }

    gameState.buffer.imageQuant = cargarTexturasMenu(&gameState.buffer.image);
    if(gameState.buffer.imageQuant == -1) {
        destroyResources(&gameState.buffer);
        al_destroy_display(disp);
        return -1;
    }

    gameState.buffer.soundQuant = cargarSonidosMenu(&gameState.buffer.sound);
    if(gameState.buffer.soundQuant == -1) {
        destroyResources(&gameState.buffer);
        al_destroy_display(disp);
        return -1;
    }

    gameState.buffer.fontQuant = cargarFuentesMenu(&gameState.buffer.font);
    if(gameState.buffer.fontQuant == -1){
        destroyResources(&gameState.buffer);
        al_destroy_display(disp);
        return -1;
    }

    if(loadGameState(&gameState) == 1) {
        destroyResources(&gameState.buffer);
        al_destroy_display(disp);
        return 1;
    }


    //Lanzamos los threads del juego
    pthread_create(&gameLogic, NULL, gamelogic, &gameState);
    pthread_create(&eventoTeclado, NULL, keyboardChanges, &gameState);
    pthread_create(&renderizar, NULL, render, &gameState);
    pthread_create(&animaciones, NULL, animar, &gameState);
    pthread_create(&fisicas, NULL, fisica, &gameState);


    //Esperamos a que terminen los threads
    pthread_join(fisicas, NULL);
    pthread_join(eventoTeclado, NULL);
    pthread_join(gameLogic, NULL);
    pthread_join(animaciones, NULL);
    pthread_join(renderizar, NULL);

    destroyResources(&gameState.buffer);
    destroyMenu();
    al_destroy_display(disp);

    return 0;
}


#elif MODOJUEGO == 1

int main (void){

    disp_init();				//inicializa el display
    disp_clear();				//limpia el display
    disp_update();              //muestra en pantalla el display limpito

    joy_init();                 //inicializa el joystick
    estadoJuego_t gameState;

    pthread_t EventoJoy, fisicas, gameLogic;
    pthread_create(&EventoJoy, NULL, InputEvent, NULL);

    pthread_create(&fisicas, NULL, fisica, &gameState);
    pthread_create(&gameLogic, NULL, gamelogic, &gameState);

    pthread_join(EventoJoy, NULL);
    pthread_join(fisicas, NULL);
    pthread_join(gameLogic, NULL);

    disp_clear();
    disp_update();
}

#endif