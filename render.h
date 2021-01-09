//
// Created by damian on 30/11/20.
//

#ifndef TPFINAL_RENDER_H
#define TPFINAL_RENDER_H

#include "matiasBrosGame.h"
#include "allegro.h"
//#include "level.h"

void *render (void *gs);
void updateCameraPosition(jugador_t* player);
int isInsideScreenX(fisica_t* object1);
void setCameraScrollX(float coordX);
float getCameraScrollX();

#if MODOJUEGO == 1
    void writeDisplay(const char **matriz);
#endif

#endif //TPFINAL_RENDER_H
