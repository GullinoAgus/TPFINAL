//
// Created by gonzalo on 23/11/20.
//

#ifndef TPFINAL_ALLEGROLIB_H
#define TPFINAL_ALLEGROLIB_H

#include "configuracion.h"
#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_audio.h"
#include "allegro5/allegro_ttf.h"

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
}bufferRecursos;

//Inicializamos los addons
int inicializarAllegro(ALLEGRO_DISPLAY **disp);
int cargarTexturasMenu(image_t **textura);
int cargarSonidosMenu(sonido_t **sonido);
int cargarFuentesMenu(fuente_t **fuente);
void destroyResources(bufferRecursos *resourcesBuffer);

#endif //TPFINAL_ALLEGROLIB_H
