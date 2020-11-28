#include "matiasBrosGame.h"
#include "IEvents.h"
#include <unistd.h>
#include <pthread.h>

int main() {

    int closeGame = 0;
    ALLEGRO_DISPLAY* disp;
    bufferRecursos resourcesBuffer;
    estadoJuego_t gameState;

    //threads
    pthread_t EventoTeclado;
    pthread_create(&EventoTeclado, NULL, keyboardChanges, NULL);


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





    destroyResources(&resourcesBuffer);
    destroyMenu();
    al_destroy_display(disp);

    return 0;
}

/*
void jeje(){
    int* prueba;
    prueba = leerNivel("texto.txt");
    int contlineas = 0;
    for (int i = 0; contlineas < 16*2 ; ++i) {
        printf(" %c ", prueba[i]);
        if (prueba[i] == BORDE){
            contlineas++;
            if (contlineas%2 == 0){
                printf("\n");
            }
        }
    }
}*/
