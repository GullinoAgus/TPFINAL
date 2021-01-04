//
// Created by damian on 30/11/20.
//

#ifndef TPFINAL_RENDER_H
#define TPFINAL_RENDER_H

#include "matiasBrosGame.h"
#include "allegro.h"
//#include "level.h"

void *render (void *gs);
void redraw();
void updateCameraPosition(jugador_t* player);
void setCameraScrollX(float coordX);
float getCameraScrollX();


#endif //TPFINAL_RENDER_H
