#include "matiasBrosGame.h"

int main() {

    bufferRecursos resourcesBuffer;
    estadoJuego_t gameState;

    //Inicializamos allegro, los recursos del juego y verificamos que se haya hecho correctamente
    if(inicializarAllegro(disp) == 1) return 1;
    if(cargarTexturasMenu(resourcesBuffer.image) == 1) return 1;
    if(cargarSonidosMenu(resourcesBuffer.sound) == 1) return 1;
    if(cargarFuentesMenu(resourcesBuffer.font) == 1) return 1;
    if(loadGameState(&gameState) == 1) return 1;
    if(cargarTexturasMenu(&resourcesBuffer.image)) return 1;
    if(cargarSonidosMenu(&resourcesBuffer.image)) return 1;
    if(cargarTexturasMenu(&resourcesBuffer.image)) return 1;

    drawMenu(resourcesBuffer);

    return 0;
}

