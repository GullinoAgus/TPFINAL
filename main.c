#include "matiasBrosGame.h"
#include <unistd.h>
#include <pthread.h>
#include "IEvents.h"
#include "menu.h"

#if MODOJUEGO == 0
int main(void) {

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

    pthread_t EventoTeclado;
    pthread_create(&EventoTeclado, NULL, keyboardChanges, NULL);

    int ventana = 0; /* VENTANA indica que es lo que veremos en la pantalla :   0 para el menu
                                                                                1 para empezar el juego
                                                                                2 para ver la tabla de puntajes
                 */

    while (ventana != 1) { //Mientras que no se seleccione PLAY en el menu para empezar el juego

        if (ventana == 0) {
            ventana = actualizarMenu(&resourcesBuffer);
        }
        else if (ventana == 2){
            ventana = verTopScores(&gameState, &resourcesBuffer);
        }
    }
    destroyResources(&resourcesBuffer);
    destroyMenu();
    al_destroy_display(disp);

    return 0;
}

#elif MODOJUEGO == 1

int main (void){

    disp_init();				//inicializa el display
    disp_clear();				//limpia todo el display

    joy_init();                 //inicializa el joystick

    pthread_t EventoJoy;
    pthread_create(&EventoJoy, NULL, InputEvent, NULL);

    int ventana = 0; /* VENTANA indica que es lo que veremos en la pantalla :   0 para el menu
                                                                                1 para empezar el juego
                                                                                2 para ver la tabla de puntajes
                                                                                3 Es la ventana con el top score
                 */

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
