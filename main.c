#include "matiasBrosGame.h"
#include "IEvents.h"
#include <unistd.h>
#include <pthread.h>

int main() {

    ALLEGRO_DISPLAY* disp;
    bufferRecursos resourcesBuffer;
    estadoJuego_t gameState;

    //Inicializamos allegro, los recursos del juego y verificamos que se haya hecho correctamente
    if(inicializarAllegro(&disp) == 1) {
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

    if(drawMenu(&resourcesBuffer, 0) == 1) {
        destroyResources(&resourcesBuffer);
        al_destroy_display(disp);
        return 1;
    }

    pthread_t EventoTeclado;
    pthread_create(&EventoTeclado, NULL, keyboardChanges, NULL);

    actualizarMenu(&resourcesBuffer);

    sleep(10);

    destroyResources(&resourcesBuffer);
    destroyMenu();
    al_destroy_display(disp);

    return 0;
}

