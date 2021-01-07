//
// Created by alvaro on 2/12/20.
//

#ifndef TPFINAL_ANIMACION_H
#define TPFINAL_ANIMACION_H

#include "entidades.h"
#include "level.h"
#include "data.h"
#include <stdlib.h>
#include "allegro.h"
#include "allegroLib.h"
#include "matiasBrosGame.h"


enum {FPSTIMER, INGAMETIMER, PHYSICSTIMER, DEATHTIMER, PLAYERSWIMMINGANIM, NUMOFDEFAULTTIMERS};

void * animar (void* entrada);


#endif //TPFINAL_ANIMACION_H
