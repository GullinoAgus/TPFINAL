#include"matiasBrosGame.h"
#include "IEvents.h"
#include "allegroLib.h"
#include "menu.h"
#include <unistd.h>
#include <pthread.h>
#include "gamelogic.h"
#include "animacion.h"
#include "render.h"


#if MODOJUEGO == 0

int main() {


    estadoJuego_t gameState;
    pthread_t eventoTeclado, gameLogic, renderizar;


    //Inicializamos allegro, los recursos del juego y verificamos que se haya hecho correctamente
    if(inicializarAllegro() == 1) {
        return 1;
    }

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

    //Lanzamos los threads del juego
    pthread_create(&gameLogic, NULL, gamelogic, &gameState);
    pthread_create(&eventoTeclado, NULL, keyboardChanges, &gameState);
    pthread_create(&renderizar, NULL, render, &gameState);

    //Esperamos a que terminen los threads
    pthread_join(eventoTeclado, NULL);
    pthread_join(gameLogic, NULL);
    pthread_join(renderizar, NULL);

    gameState.entidades.jugador.angle = 0;

    destroyResources(&gameState.buffer);
    destroyMenu();

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