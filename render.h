//
// Created by damian on 30/11/20.
//

#ifndef TPFINAL_RENDER_H
#define TPFINAL_RENDER_H

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

void * render (void *);

#endif //TPFINAL_RENDER_H
