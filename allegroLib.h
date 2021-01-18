//
// Created by gonzalo on 23/11/20.
//

#ifndef TPFINAL_ALLEGROLIB_H
#define TPFINAL_ALLEGROLIB_H

#include "configuracion.h"
#include "matiasBrosGame.h"

#if MODOJUEGO == 0


#include "allegro.h"
#include "render.h"

//Inicializamos los addons
int inicializarAllegro();
int cargarTexturasMenu(image_t **textura);
int cargarSonidosMenu(sonido_t **sonido);
int cargarFuentesMenu(fuente_t **fuente);
void destroyResources(bufferRecursos_t *resourcesBuffer);

#endif

int loadGameState(estadoJuego_t *gameState);

#endif //TPFINAL_ALLEGROLIB_H
