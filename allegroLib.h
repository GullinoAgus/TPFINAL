//
// Created by gonzalo on 23/11/20.
//

#ifndef TPFINAL_ALLEGROLIB_H
#define TPFINAL_ALLEGROLIB_H

#include "configuracion.h"

typedef ALLEGRO_BITMAP* image_t [TEXTURASMAX];
typedef ALLEGRO_SAMPLE* sonido_t [SONIDOSMAX];
typedef ALLEGRO_FONT* fuente_t [FUENTESMAX];

typedef struct{
    image_t image;
    sonido_t sound;
    fuente_t font;
}bufferRecursos;

//Inicializamos los addons
int inicializarAllegro(ALLEGRO_DISPLAY* disp);
int cargarTexturasMenu(img_t *textura);
int cargarSonidosMenu(sonido_t *sonido);
int cargarFuentesMenu(fuente_t *fuente);

#endif //TPFINAL_ALLEGROLIB_H
