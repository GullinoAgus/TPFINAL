//
// Created by gonzalo on 23/11/20.
//

#ifndef TPFINAL_ALLEGROLIB_H
#define TPFINAL_ALLEGROLIB_H

#include "configuracion.h"

#if MODOJUEGO == 0

#include "matiasBrosGame.h"

//Inicializamos los addons
int inicializarAllegro(ALLEGRO_DISPLAY **disp);
int cargarTexturasMenu(image_t **textura);
int cargarSonidosMenu(sonido_t **sonido);
int cargarFuentesMenu(fuente_t **fuente);
int loadGameState(estadoJuego_t *gameState);   //Cargamos los datos del juego
void destroyResources(bufferRecursos *resourcesBuffer);

#endif

#endif //TPFINAL_ALLEGROLIB_H
