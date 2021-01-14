//
// Created by alvaro on 2/12/20.
//

#ifndef TPFINAL_ANIMACION_H
#define TPFINAL_ANIMACION_H

#include "entidades.h"
#include "level.h"
#include "data.h"
#include <stdlib.h>
#include "matiasBrosGame.h"

#if MODOJUEGO == 0

#include "allegro.h"
#include "allegroLib.h"

#endif


enum {FPSTIMER, INGAMETIMER, PHYSICSTIMER, DEATHANIM, PLAYERSWIMMINGANIM, SEAWEEDANIM, BLINKINGCOINANIM, CHEEPCHEEPANIM, LIFEUPANIM, NUMOFDEFAULTTIMERS};

void * animar (void* entrada);
void stopInGameAnimations();
void startInGameAnimations();

#endif //TPFINAL_ANIMACION_H
