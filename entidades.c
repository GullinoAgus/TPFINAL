//
// Created by agus on 23/11/20.
//

#include "entidades.h"
#include "configuracion.h"
#include "data.h"

#define TOWORLDPOS(v) (v*PIXELSPERUNIT)

void initEntities(estadoJuego_t* gameState){

    int blocksIndex = 0;
    int enemiesIndex = 0;

    for(int i = 0; i < gameState->levelWidht; i++){
        for(int j = 0; j < gameState->levelHeight; j++){
            switch ((gameState->level)[i][j]) {

                case LADRILLO:
                    gameState->entidades->bloques[blocksIndex].sprite = 0;
                    gameState->entidades->bloques[blocksIndex].identificador = LADRILLO;
                    gameState->entidades->bloques[blocksIndex].fisica.posx = TOWORLDPOS(i);
                    gameState->entidades->bloques[blocksIndex].fisica.posy = TOWORLDPOS(j);
                    gameState->entidades->bloques[blocksIndex].fisica.ancho = PIXELSPERUNIT;
                    gameState->entidades->bloques[blocksIndex].fisica.alto = PIXELSPERUNIT;
                    gameState->entidades->bloques[blocksIndex].fisica.velX = 0;
                    gameState->entidades->bloques[blocksIndex].fisica.velY = 0;
                    blocksIndex++;
                    break;

                case JUGADOR:
                    gameState->entidades.jugador.vidas = 3;
                    gameState->entidades.jugador.sobreBloque = 0;
                    gameState->entidades.jugador.estado = 1;        //VIVO?
                    gameState->entidades.jugador.sprite = 0;
                    gameState->entidades.jugador.fisica.posX = TOWORLDPOS(i);
                    gameState->entidades.jugador.fisica.posX = TOWORLDPOS(j);
                    gameState->entidades.jugador.fisica.ancho = 0;
                    gameState->entidades.jugador.fisica.alto = 0;
                    gameState->entidades.jugador.fisica.velX = 0;
                    gameState->entidades.jugador.fisica.velY = 0;
                    break;

                case CHEEPCHEEP:
                    gameState->entidades->enemigos[enemiesIndex].sprite = 0;
                    gameState->entidades->enemigos[enemiesIndex].identificador = CHEEPCHEEP;
                    gameState->entidades->enemigos[enemiesIndex].fisica.posx = TOWORLDPOS(i);
                    gameState->entidades->enemigos[enemiesIndex].fisica.posy = TOWORLDPOS(j);
                    gameState->entidades->enemigos[enemiesIndex].fisica.ancho = PIXELSPERUNIT;
                    gameState->entidades->enemigos[enemiesIndex].fisica.alto = PIXELSPERUNIT;
                    gameState->entidades->enemigos[enemiesIndex].fisica.velX = -10;             //Le puse una velocidad al cheep cheep para la izquierda
                    gameState->entidades->enemigos[enemiesIndex].fisica.velY = 0;
                    enemiesIndex++;
                    break;

                case PULPITO:
                    gameState->entidades->enemigos[enemiesIndex].sprite = 0;
                    gameState->entidades->enemigos[enemiesIndex].identificador = PULPITO;
                    gameState->entidades->enemigos[enemiesIndex].fisica.posx = TOWORLDPOS(i);
                    gameState->entidades->enemigos[enemiesIndex].fisica.posy = TOWORLDPOS(j);
                    gameState->entidades->enemigos[enemiesIndex].fisica.ancho = PIXELSPERUNIT*2;
                    gameState->entidades->enemigos[enemiesIndex].fisica.alto = PIXELSPERUNIT;
                    gameState->entidades->enemigos[enemiesIndex].fisica.velX = -10;             //Le puse una velocidad al cheep cheep para la izquierda
                    gameState->entidades->enemigos[enemiesIndex].fisica.velY = 0;
                    enemiesIndex++;
                    break;
            }
        }
    }
}