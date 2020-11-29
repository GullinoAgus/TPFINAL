#include "matiasBrosGame.h"
#include "IEvents.h"
#include <unistd.h>
#include <pthread.h>

#include "level.h"      //No se porque no andan los includes de matiasBrosGame
#include "entidades.h"

#if MODOJUEGO == 0

int main(void) {

    int closedGame = 0;
    ALLEGRO_DISPLAY* disp;
    bufferRecursos_t resourcesBuffer;
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

    pthread_t EventoTeclado;
    pthread_create(&EventoTeclado, NULL, keyboardChanges, NULL);

    drawMenu(&resourcesBuffer);

    while(!closedGame) {

        while(esBufferVacio());
        char evento = getInputEvent();

        updateMenu(&gameState.menuSelection, evento);

        if(evento == DOWNBOTON) {
            closedGame = 1;
        }
    }

    //Cargamos los datos del nivel
    cargarMapa(&gameState.level, 0, &gameState.levelWidht, &gameState.levelHeight);

    //Estas dos no funcionan todavia
    initEntities(&gameState);
    drawLevel(&gameState);

    closedGame = 0;
    while(!closedGame) {

        while(esBufferVacio());
        char evento = getInputEvent();



        if(evento == DOWNBOTON) {
            closedGame = 1;
        }
    }

    pthread_join(EventoTeclado, NULL);
    destroyResources(&resourcesBuffer);
    destroyMenu();
    al_destroy_display(disp);

    return 0;
}

#elif MODOJUEGO == 1

int main (void){

    disp_init();				//inicializa el display
    disp_clear();				//limpia el display
    disp_update();

    joy_init();                 //inicializa el joystick

    int ventana = 0; /* VENTANA indica que es lo que veremos en la pantalla :   0 para el menu
                                                                                1 para empezar el juego
                                                                                2 para ver la tabla de puntajes
                                                                                3 Es la ventana con el top score
                 */

    pthread_t EventoJoy;
    pthread_create(&EventoJoy, NULL, InputEvent, NULL);

    while (ventana != 1) { //Mientras que no se seleccione PLAY en el menu para empezar el juego

        if (ventana == 0) {
            ventana = actualizarMenu();
        }
        else if (ventana == 2){
            ventana = verTopScores();
        }
        else if (ventana == 3){
            ventana = TopScore();
        }
    }

    disp_clear();
}

#endif
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
