//
// Created by gonzalo on 23/11/20.
//

#ifndef TPFINAL_ALLEGROLIB_H
#define TPFINAL_ALLEGROLIB_H

#include "configuracion.h"

#if MODOJUEGO == 0

#include "matiasBrosGame.h"
#include "allegro.h"

typedef ALLEGRO_BITMAP* image_t;
typedef ALLEGRO_SAMPLE* sonido_t;
typedef ALLEGRO_FONT* fuente_t;

typedef struct{
    int imageQuant;
    int soundQuant;
    int fontQuant;
    image_t *image;
    sonido_t *sound;
    fuente_t *font;
}bufferRecursos_t;

//Inicializamos los addons
int inicializarAllegro(ALLEGRO_DISPLAY **disp);
int cargarTexturasMenu(image_t **textura);
int cargarSonidosMenu(sonido_t **sonido);
int cargarFuentesMenu(fuente_t **fuente);
int loadGameState(estadoJuego_t *gameState);   //Cargamos los datos del juego
void destroyResources(bufferRecursos_t *resourcesBuffer);

#endif

#endif //TPFINAL_ALLEGROLIB_H
