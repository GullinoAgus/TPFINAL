//
// Created by damian on 30/11/20.
//

#ifndef TPFINAL_GAMELOGIC_H
#define TPFINAL_GAMELOGIC_H

#include "matiasBrosGame.h"
#include "semaphore.h"

void *gamelogic (void *p2GameState);
char wasLevelInitialized();
sem_t* getGameLogicSem();
sem_t* getPhysicsSem();
sem_t* getRenderSem();
sem_t* getAnimationSem();

#endif //TPFINAL_GAMELOGIC_H
