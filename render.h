//
// Created by damian on 30/11/20.
//

#ifndef TPFINAL_RENDER_H
#define TPFINAL_RENDER_H

#include "matiasBrosGame.h"

void *render (void *gs);
void updateCameraPosition(void* gs);
int isInsideScreenX(fisica_t* object1);
void setCameraScrollX(float coordX);
float getCameraScrollX();
void resetLastBlockInMap();

#if MODOJUEGO == 1

void writeDisplay(char matriz[16][16]);//ESTA FUNCION CONTROLA EL BUFFER  DEL DISPLAY A PARTIR DE LA MATRIZ DADA
// SI EL ELEMENTO EN LA MATRIZ VALE 0, SE APAGA EL LED CORRESPONDIENTE Y
// SI VALE 1 SE LO PRENDE
// EN CUALQUIER OTRO CASO, SE LO DEJA IGUAL QUE ANTES

int isInsideScreenY(fisica_t* object1);

#elif MODOJUEGO == 0

#include "allegro.h"

#endif

#endif //TPFINAL_RENDER_H
