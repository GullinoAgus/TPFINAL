#include "matiasBrosGame.h"
#include "IEvents.h"
#include "allegroLib.h"
#include "menu.h"
#include <unistd.h>
#include <pthread.h>
#include "level.h"
#include "gamelogic.h"
#include "Animaciones.h"

#if MODOJUEGO == 0

void mapaCon(estadoJuego_t *eJ);

int main(void) {

    int closedGame = 0;
    ALLEGRO_DISPLAY* disp;
    bufferRecursos_t resourcesBuffer;
    estadoJuego_t gameState;

    gameState.buffer = resourcesBuffer;

    entidades_t entidades;

    gameState.state=3;  //comenzamos en el menu

    //Inicializamos allegro, los recursos del juego y verificamos que se haya hecho correctamente
    if(inicializarAllegro(disp) == 1) {
        al_destroy_display(disp);
        return 1;
    }

    resourcesBuffer.imageQuant = cargarTexturasMenu(&resourcesBuffer.image);
    if(resourcesBuffer.imageQuant == -1) {
        destroyResources(&resourcesBuffer);
        al_destroy_display(disp);
        return -1;
    }

    resourcesBuffer.soundQuant = cargarSonidosMenu(&resourcesBuffer.sound);
    if(resourcesBuffer.soundQuant == -1) {
        destroyResources(&resourcesBuffer);
        al_destroy_display(disp);
        return -1;
    }

    resourcesBuffer.fontQuant = cargarFuentesMenu(&resourcesBuffer.font);
    if(resourcesBuffer.fontQuant == -1){
        destroyResources(&resourcesBuffer);
        al_destroy_display(disp);
        return -1;
    }

    if(loadGameState(&gameState) == 1) {
        destroyResources(&resourcesBuffer);
        al_destroy_display(disp);
        return 1;
    }

    pthread_t eventoTeclado, fisicas, gameLogic, animaciones, render;
    pthread_create(&eventoTeclado, NULL, keyboardChanges, NULL);
    //pthread_create(&animaciones, NULL, animar, &estructuras);
    pthread_create(&render, NULL, renderizar, &gameState);
    pthread_create(&gameLogic, NULL, gamelogic, &gameState);

/*
    while(!closedGame) {

        while(esBufferVacio());
        char evento = getInputEvent();

        updateMenu(&gameState.menuSelection, evento);

        if(evento == DOWNBOTON) {
            closedGame = 1;
        }
    }
*/
    //Cargamos los datos del nivel
    cargarMapa(&gameState.level, ONE);

    if(initEntities(&gameState) == 1) {
        destroyResources(&resourcesBuffer);
        al_destroy_display(disp);
        return 1;
    }

    pthread_create(&fisicas, NULL, fisica, &gameState);
    closedGame = 0;
    /*
    while(!closedGame) {
        if (!esBufferVacio()) {
            switch (getInputEvent()) {
                case DOWNIZQUIERDA:
                    gameState.entidades.jugador.fisica.velx = -1.0f;
                    break;
                case DOWNDERECHA:
                    gameState.entidades.jugador.fisica.velx = 1.0f;
                    break;
                case DOWNARRIBA:
                    gameState.entidades.jugador.fisica.vely = -1.0f;
                    break;
                case UPIZQUIERDA:
                    gameState.entidades.jugador.fisica.velx = 0.0f;
                    break;
                case UPDERECHA:
                    gameState.entidades.jugador.fisica.velx = 0.0f;
                    break;
            }
        }

        drawLevel(&gameState, &resourcesBuffer);

    }
*/
    pthread_join(eventoTeclado, NULL);

    destroyResources(&resourcesBuffer);
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

void mapaCon(estadoJuego_t* eJ){
    for(int i = 0; i < eJ->level.levelHeight; i++){
        for(int j = 0; j < eJ->level.levelWidht; j++){
            printf("%c ", eJ->level.level[i][j]);
        }
        printf("\n");
    }
}
