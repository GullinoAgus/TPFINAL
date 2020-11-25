#include "matiasBrosGame.h"
#include <unistd.h>

int main(int argc, char **argv) {

    ALLEGRO_DISPLAY* disp;
    bufferRecursos resourcesBuffer;
    estadoJuego_t gameState;

    //Inicializamos allegro, los recursos del juego y verificamos que se haya hecho correctamente        BORRAR LO DINAMICO
    if(inicializarAllegro(disp) == 1) {
        al_destroy_display(disp);
        return 1;
    }

    if(cargarTexturasMenu(&resourcesBuffer.image) == 1) {
        destroyResources(&resourcesBuffer);
        return 1;
    }

    if(cargarSonidosMenu(&resourcesBuffer.sound) == 1) {
        destroyResources(&resourcesBuffer);
        return 1;
    }
    if(cargarFuentesMenu(&resourcesBuffer.font) == 1){
        destroyResources(&resourcesBuffer);
        return 1;
    }

    if(loadGameState(&gameState) == 1) {
        return 1;
    }

    if(drawMenu(&resourcesBuffer) == 1) {
        destroyResources(&resourcesBuffer);
        return 1;
    }

    al_flip_display();

    sleep(10);

    destroyResources(&resourcesBuffer);

    return 0;
}

