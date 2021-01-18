//
// Created by gonzalo on 27/11/20.
//

#include "level.h"
#include "data.h"
#include "entidades.h"
#include <stdlib.h>
#include "render.h"
#include "menu.h"
#include "gamelogic.h"
#include "string.h"
#include <unistd.h>

static void initBackUpEntities(estadoJuego_t* gameState);
#define TOWORLDPOS(v) ( (v) * PIXELSPERUNIT) //FIXME: YO PUSE ESTO EN GENERAL, NOSE SI ESTARA BIEN

static int countColumns(level_t* level, FILE* mapData);

#if MODOJUEGO == 0

#include "allegro.h"

#define UICOLOR al_map_rgb(76,25,153)

typedef struct{
    int offsetX;
    int moveDelay;
    int offsetXRecord;
    float scale;
}wave_t;

static wave_t wave;

void drawLevel(estadoJuego_t *gameState){

    bufferRecursos_t *resourceBuffer = &gameState->buffer;
    int playerSprite;
    char auxToString[10];
    int flip_player = 0;
    float scrollX;
    int i = 0;


    updateCameraPosition(gameState);
    scrollX = getCameraScrollX();

    al_clear_to_color(al_map_rgb(153, 195, 219));

    //Dibujamos las olas
    if(wave.moveDelay > 0){
        wave.moveDelay--;
    }
    else{
        if(wave.offsetXRecord == 0){
            wave.offsetX -= 15;
            wave.offsetXRecord = 1;
        }
        else{
            wave.offsetX += 15;
            wave.offsetXRecord = 0;
        }
        wave.moveDelay = 15;
    }

    if(scrollX - wave.offsetX + SCREENWIDHT >= al_get_bitmap_width(resourceBuffer->image[WAVESPRITE]) * wave.scale){
        wave.offsetX += al_get_bitmap_width(resourceBuffer->image[WAVESPRITE]) * wave.scale - SCREENWIDHT;
    }
    al_draw_scaled_bitmap(resourceBuffer->image[WAVESPRITE], 0, 0, al_get_bitmap_width(resourceBuffer->image[WAVESPRITE]), al_get_bitmap_height(resourceBuffer->image[WAVESPRITE]), wave.offsetX - scrollX, PIXELSPERUNIT,
                          al_get_bitmap_width(resourceBuffer->image[WAVESPRITE]) * wave.scale, al_get_bitmap_height(resourceBuffer->image[WAVESPRITE]) * wave.scale, 0);

    //Mientras no se hayan leido todos los bloques, dibujamos el siguiente
    bloque_t bloque;

        while(gameState->entidades.bloques[i].identificador != NULLENTITIE && wasLevelInitialized()){        //FIXME: Aca cuando cai en un hueco me tiro segmentation fault con i = 2097 y 208
        bloque = gameState->entidades.bloques[i];
        switch (bloque.identificador){
            case ALGA:
                al_draw_scaled_bitmap(resourceBuffer->image[ALGASPRITE1 + bloque.sprite], 0, 0, al_get_bitmap_width(resourceBuffer->image[ALGASPRITE1]), al_get_bitmap_height(resourceBuffer->image[ALGASPRITE1]), bloque.fisica.posx - scrollX, bloque.fisica.posy,
                                      bloque.fisica.ancho, bloque.fisica.alto, 0);
                break;

            case LADRILLO:
                for (int j = 0; j < bloque.fisica.ancho/PIXELSPERUNIT; j++) {
                    al_draw_scaled_bitmap(resourceBuffer->image[PISOSPRITE], 0, 0, al_get_bitmap_width(resourceBuffer->image[PISOSPRITE]),
                                          al_get_bitmap_height(resourceBuffer->image[PISOSPRITE]), bloque.fisica.posx + j * PIXELSPERUNIT - scrollX,
                                          bloque.fisica.posy,PIXELSPERUNIT, PIXELSPERUNIT, 0);
                }
                break;

            case MONEDA:
                al_draw_scaled_bitmap(resourceBuffer->image[COINSPRITE1 + bloque.sprite], 0, 0, al_get_bitmap_width(resourceBuffer->image[COINSPRITE1]), al_get_bitmap_height(resourceBuffer->image[COINSPRITE1]), bloque.fisica.posx - scrollX, bloque.fisica.posy,
                                      bloque.fisica.ancho, bloque.fisica.alto, 0);
                break;

            case TOPPIPE:
                al_draw_scaled_bitmap(resourceBuffer->image[PIPETOPSPRITE], 0, 0, al_get_bitmap_width(resourceBuffer->image[PIPETOPSPRITE]), al_get_bitmap_height(resourceBuffer->image[PIPETOPSPRITE]), bloque.fisica.posx - scrollX, bloque.fisica.posy,
                                      bloque.fisica.ancho, bloque.fisica.alto, 0);
                break;

            case MIDDLEPIPE:
                al_draw_scaled_bitmap(resourceBuffer->image[PIPEMIDDLESPRITE], 0, 0, al_get_bitmap_width(resourceBuffer->image[PIPEMIDDLESPRITE]), al_get_bitmap_height(resourceBuffer->image[PIPEMIDDLESPRITE]), bloque.fisica.posx - scrollX, bloque.fisica.posy,
                                      bloque.fisica.ancho, bloque.fisica.alto, 0);
                break;
        }

        i++;
    }

    //Dibujamos los enemigos
    i = 0;
    while(gameState->entidades.enemigos[i].identificador != NULLENTITIE){
        enemigo_t enemigo = gameState->entidades.enemigos[i];
        switch (enemigo.identificador){
            case PULPITO:
                al_draw_scaled_bitmap(resourceBuffer->image[BLOOPERSPRITE1 + enemigo.sprite], 0, 0, al_get_bitmap_width(resourceBuffer->image[BLOOPERSPRITE1]),  al_get_bitmap_height(resourceBuffer->image[BLOOPERSPRITE1]), enemigo.fisica.posx - scrollX, enemigo.fisica.posy,
                                      enemigo.fisica.ancho, enemigo.fisica.alto, 0);
                break;

            case FASTCHEEPCHEEP:
                al_draw_scaled_bitmap(resourceBuffer->image[CHEEPCHEEPSPRITE1 + enemigo.sprite], 0, 0, al_get_bitmap_width(resourceBuffer->image[CHEEPCHEEPSPRITE1]),  al_get_bitmap_height(resourceBuffer->image[CHEEPCHEEPSPRITE1]), enemigo.fisica.posx-scrollX, enemigo.fisica.posy,
                                      enemigo.fisica.ancho, enemigo.fisica.alto, 0);
                break;

            case SLOWCHEEPCHEEP:
                al_draw_scaled_bitmap(resourceBuffer->image[CHEEPCHEEPSSLOWSPRITE1 + enemigo.sprite], 0, 0, al_get_bitmap_width(resourceBuffer->image[CHEEPCHEEPSSLOWSPRITE1]),  al_get_bitmap_height(resourceBuffer->image[CHEEPCHEEPSSLOWSPRITE1]), enemigo.fisica.posx-scrollX, enemigo.fisica.posy,
                                      enemigo.fisica.ancho, enemigo.fisica.alto, 0);
                break;
        }

        i++;
    }

    //Dibujamos el UI
    //score
    sprintf(auxToString, "%d", gameState->gameUI.score);
    al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], UICOLOR, 120, 0, 0, "matias");
    al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], UICOLOR, 120, 50, 0, auxToString);

    //coins
    sprintf(auxToString, "%d", gameState->gameUI.coins);
    al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], UICOLOR, 350, 30, 0, " x ");
    al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], UICOLOR, 400, 30, 0, auxToString);
    al_draw_scaled_bitmap(resourceBuffer->image[COINSPRITE1], 0, 0, al_get_bitmap_width(resourceBuffer->image[COINSPRITE1]), al_get_bitmap_height(resourceBuffer->image[COINSPRITE1]), 315, 38,
                          al_get_bitmap_width(resourceBuffer->image[COINSPRITE1]) * 3.0f, al_get_bitmap_height(resourceBuffer->image[COINSPRITE1]) * 3.0f, 0);

    //level
    sprintf(auxToString, "%d", gameState->gameUI.level);
    al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], UICOLOR, 800, 30, 0, "level");
    al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], UICOLOR, 900, 30, 0, auxToString);

    //timer
    sprintf(auxToString, "%d", gameState->gameUI.time);
    al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], UICOLOR, 1000, 30, 0, "time");
    al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], UICOLOR, 1100, 30, 0, auxToString);

    //Dibujamos al jugador
    fisica_t jugador = gameState->entidades.jugador.fisica;
    if(jugador.velx >= 0) {
        flip_player = 0;
    }
    else {
        flip_player = ALLEGRO_FLIP_HORIZONTAL;
    }

    playerSprite = MATIASIDLESPRITE + gameState->entidades.jugador.sprite;
    al_draw_scaled_rotated_bitmap(resourceBuffer->image[playerSprite], (float)al_get_bitmap_width(resourceBuffer->image[playerSprite]) / 2.0, (float)al_get_bitmap_height(resourceBuffer->image[playerSprite]) / 2.0, jugador.posx + jugador.ancho /2.0 - scrollX, jugador.posy + (float) jugador.alto / 2.0,  ((float)jugador.ancho/(float)al_get_bitmap_width(resourceBuffer->image[playerSprite])),  ((float)jugador.alto/(float)al_get_bitmap_height(resourceBuffer->image[playerSprite])), gameState->entidades.jugador.angleRotation, flip_player);

    al_flip_display();
}

void resetWavePosition(void){
    wave.offsetX = -15;
    wave.moveDelay = 15;
    wave.offsetXRecord = 0;
    wave.scale = 3;
}

void drawGameOverScreen(estadoJuego_t* gameState){

    int entryFinished = 0;
    static char playerName[MAXPLAYERNAME] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
    static char auxString[20];

    gameState->pPlayerName = playerName;

    al_clear_to_color(al_map_rgb(153, 195, 219));

    if(wasNewHighScoreAchieved(gameState)) {
        sprintf(auxString, "%s", "GAME OVER");
        al_draw_text(gameState->buffer.font[SUPERMARIOFONT140], al_map_rgb(200, 16, 84), SCREENWIDHT/2 - 170, SCREENHEIGHT/2 - 200, 0, auxString);

        sprintf(auxString, "%s", "NEW HIGH SCORE !!");
        al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], al_map_rgb(57, 16, 84), SCREENWIDHT / 2 + 40, SCREENHEIGHT / 2 - 75, 0, auxString);

        sprintf(auxString, "%d", gameState->gameUI.score);
        al_draw_text(gameState->buffer.font[SUPERMARIOFONT120], al_map_rgb(57, 16, 84), SCREENWIDHT / 2 - 10, SCREENHEIGHT / 2 + 50, 0, auxString);

        sprintf(auxString, "%s", "Enter your name:");
        al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], al_map_rgb(57, 16, 84), SCREENWIDHT / 2 - 500, SCREENHEIGHT / 2 + 200, 0, auxString);
        al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], al_map_rgb(57, 16, 84), SCREENWIDHT / 2,SCREENHEIGHT / 2 + 200, 0, playerName);
    }
    else{
        sprintf(auxString, "%s", "GAME OVER");
        al_draw_text(gameState->buffer.font[SUPERMARIOFONT140], al_map_rgb(200, 16, 84), SCREENWIDHT/2 - 170, SCREENHEIGHT/2 - 50, 0, auxString);
    }

    al_flip_display();
}

void drawRetryScreen(estadoJuego_t *gameState){

    char auxToString[10];

    al_clear_to_color(al_map_rgb(153, 195, 219));

    image_t playerImg = gameState->buffer.image[MATIASIDLESPRITE];
    al_draw_scaled_bitmap(playerImg, 0, 0, al_get_bitmap_width(playerImg), al_get_bitmap_height(playerImg),SCREENWIDHT/2 - 70, SCREENHEIGHT/2, al_get_bitmap_width(playerImg)*4, al_get_bitmap_height(playerImg)*4, 0);

    sprintf(auxToString, "%d", gameState->entidades.jugador.vidas);
    al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], al_map_rgb(57, 16, 84), SCREENWIDHT / 2 + 10, SCREENHEIGHT / 2, 0, "X");
    al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], al_map_rgb(57, 16, 84), SCREENWIDHT / 2 + 40, SCREENHEIGHT / 2, 0, auxToString);

    al_flip_display();
}

void drawNextLevelScreen(estadoJuego_t *gameState){

    char auxToString[10];
    char auxToString2[10];

    al_clear_to_color(al_map_rgb(153, 195, 219));

    image_t playerImg = gameState->buffer.image[MATIASIDLESPRITE];
    al_draw_scaled_bitmap(playerImg, 0, 0, al_get_bitmap_width(playerImg), al_get_bitmap_height(playerImg),SCREENWIDHT/2 - 70, SCREENHEIGHT/2, al_get_bitmap_width(playerImg)*4, al_get_bitmap_height(playerImg)*4, 0);

    sprintf(auxToString, "%d", gameState->entidades.jugador.vidas);
    sprintf(auxToString2, "%d", gameState->gameUI.score);

    al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], al_map_rgb(57, 16, 84), SCREENWIDHT / 2 + 10, SCREENHEIGHT / 2, 0, "X");
    al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], al_map_rgb(57, 16, 84), SCREENWIDHT / 2 + 40, SCREENHEIGHT / 2, 0, auxToString);

    al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], al_map_rgb(57, 16, 84), SCREENWIDHT / 2 - 80, SCREENHEIGHT / 2 + 100, 0, "SCORE = ");
    al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], al_map_rgb(57, 16, 84), SCREENWIDHT / 2 + 50, SCREENHEIGHT / 2 + 100, 0, auxToString2);

    al_draw_text(gameState->buffer.font[SUPERMARIOFONT80], al_map_rgb(200, 16, 84), SCREENWIDHT/2 - 160, SCREENHEIGHT/5, 0, "LEVEL CLEARED");

    al_flip_display();

}

void drawPause(estadoJuego_t *gameState){

    al_clear_to_color(al_map_rgb(20, 230, 230));

    al_draw_text(gameState->buffer.font[SUPERMARIOFONT120], al_map_rgb(200, 16, 84), SCREENWIDHT/2 - 88, SCREENHEIGHT/6, 0, "PAUSE");

    al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], al_map_rgb(57, 16, 84), SCREENWIDHT / 2 - 57, SCREENHEIGHT / 2, 0, "RESUME");

    al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], al_map_rgb(57, 16, 84), SCREENWIDHT / 2 - 110, SCREENHEIGHT / 2 + 100, 0, "BACK TO MENU");

    for(int i = FONDOMENU; i <= FLECHAMENU; i++){   //Busco la imagen de la flecha del menu para dibujarla aqui
        image_t currentImg = gameState->buffer.image[i];
        int arrowPosition;

        if(i == FLECHAMENU){
            arrowPosition = gameState->pauseSelection;
            al_draw_scaled_bitmap(currentImg, 0, 0, al_get_bitmap_width(currentImg), al_get_bitmap_height(currentImg),
                                  SCREENWIDHT/2 - 175, SCREENHEIGHT/2 + 100*arrowPosition, al_get_bitmap_width(currentImg) * 3,
                                  al_get_bitmap_height(currentImg) * 4, 0);

        }
    }

    al_flip_display();
}

int initEntities(estadoJuego_t* gameState){

    int blocksCounter = 0;
    int enemiesCounter = 0;
    int blocksIndex = 0;
    int enemiesIndex = 0;
    int horizontalBlocksCounter = 0;
    char currentBlock;

    //Calculamos la cantidad de enemigos y de bloques que hay en el mapa
    for(int i = 0; i < gameState->level.levelHeight; i++){
        for(int j = 0; j < gameState->level.levelWidht; j++) {
            switch(gameState->level.level[i][j]) {
                case LADRILLO:
                case MONEDA:
                case TOPPIPE:
                case MIDDLEPIPE:
                case ALGA:
                    blocksCounter++;
                    break;

                case FASTCHEEPCHEEP:
                case SLOWCHEEPCHEEP:
                case PULPITO:
                    enemiesCounter++;
                    break;
            }
        }
    }

    //Reservamos el espacio para los bloques
    gameState->entidades.bloques = (bloque_t*) malloc(sizeof(bloque_t) * blocksCounter);
    if(gameState->entidades.bloques == NULL){
        printf("Error al reservar espacio para los bloques");
        return 1;
    }


    //Reservamos el espacio para los enemigos
    gameState->entidades.enemigos = (enemigo_t*) calloc((enemiesCounter+1), sizeof(enemigo_t));
    if(gameState->entidades.enemigos == NULL){
        printf("Error al reservar espacio para los enemigos");
        return 1;
    }
    gameState->entidades.enemigos[enemiesCounter].identificador = NULLENTITIE;

    for(int i = 0; i < gameState->level.levelHeight; i++){
        for(int j = 0; j < gameState->level.levelWidht; j++){

            //Agarramos el siguiente bloque
            currentBlock = gameState->level.level[i][j];

            //Si el contador de bloques adyacentes esta activo y el siguiente bloque no es un ladrillo
            if (horizontalBlocksCounter == 1 && currentBlock != LADRILLO){
                blocksIndex++;                  //Avanzamos el indice de bloques al siguiente
                horizontalBlocksCounter = 0;    //Terminamos de contar el grupo de bloques actuales
            }

            switch (currentBlock) {
                case LADRILLO:
                    if (horizontalBlocksCounter == 0){
                        gameState->entidades.bloques[blocksIndex].sprite = 0;
                        gameState->entidades.bloques[blocksIndex].identificador = LADRILLO;
                        gameState->entidades.bloques[blocksIndex].fisica.posx = TOWORLDPOS(j);
                        gameState->entidades.bloques[blocksIndex].fisica.posy = TOWORLDPOS(i);
                        gameState->entidades.bloques[blocksIndex].fisica.ancho = PIXELSPERUNIT;
                        gameState->entidades.bloques[blocksIndex].fisica.alto = PIXELSPERUNIT;
                        gameState->entidades.bloques[blocksIndex].fisica.velx = 0;
                        gameState->entidades.bloques[blocksIndex].fisica.vely = 0;
                        horizontalBlocksCounter = 1;
                    }
                    else{
                        gameState->entidades.bloques[blocksIndex].fisica.ancho += PIXELSPERUNIT;
                        blocksCounter--;        //Cada vez que unimos un bloque, disminuimos la cantidad de bloques totales
                    }
                    break;

                case MONEDA:
                    gameState->entidades.bloques[blocksIndex].sprite = 0;
                    gameState->entidades.bloques[blocksIndex].identificador = MONEDA;
                    gameState->entidades.bloques[blocksIndex].fisica.posx = TOWORLDPOS(j);
                    gameState->entidades.bloques[blocksIndex].fisica.posy = TOWORLDPOS(i);
                    gameState->entidades.bloques[blocksIndex].fisica.ancho = PIXELSPERUNIT;
                    gameState->entidades.bloques[blocksIndex].fisica.alto = PIXELSPERUNIT;
                    gameState->entidades.bloques[blocksIndex].fisica.velx = 0;
                    gameState->entidades.bloques[blocksIndex].fisica.vely = 0;
                    blocksIndex++;
                    break;

                case TOPPIPE:
                    gameState->entidades.bloques[blocksIndex].sprite = 0;
                    gameState->entidades.bloques[blocksIndex].identificador = TOPPIPE;
                    gameState->entidades.bloques[blocksIndex].fisica.posx = TOWORLDPOS(j);
                    gameState->entidades.bloques[blocksIndex].fisica.posy = TOWORLDPOS(i);
                    gameState->entidades.bloques[blocksIndex].fisica.ancho = PIXELSPERUNIT;
                    gameState->entidades.bloques[blocksIndex].fisica.alto = PIXELSPERUNIT;
                    gameState->entidades.bloques[blocksIndex].fisica.velx = 0;
                    gameState->entidades.bloques[blocksIndex].fisica.vely = 0;
                    blocksIndex++;
                    break;

                case MIDDLEPIPE:
                    gameState->entidades.bloques[blocksIndex].sprite = 0;
                    gameState->entidades.bloques[blocksIndex].identificador = MIDDLEPIPE;
                    gameState->entidades.bloques[blocksIndex].fisica.posx = TOWORLDPOS(j);
                    gameState->entidades.bloques[blocksIndex].fisica.posy = TOWORLDPOS(i);
                    gameState->entidades.bloques[blocksIndex].fisica.ancho = PIXELSPERUNIT;
                    gameState->entidades.bloques[blocksIndex].fisica.alto = PIXELSPERUNIT;
                    gameState->entidades.bloques[blocksIndex].fisica.velx = 0;
                    gameState->entidades.bloques[blocksIndex].fisica.vely = 0;
                    blocksIndex++;
                    break;

                case JUGADOR:
                    gameState->entidades.jugador.sobreBloque = 0;
                    gameState->entidades.jugador.estado = ALIVE;
                    gameState->entidades.jugador.vidas = 0;
                    gameState->entidades.jugador.powerUpsState = SMALL;
                    gameState->entidades.jugador.sprite = 0;
                    gameState->entidades.jugador.fisica.posx = TOWORLDPOS(j);
                    gameState->entidades.jugador.fisica.posy = TOWORLDPOS(i);
                    gameState->entidades.jugador.fisica.ancho = PIXELSPERUNIT;
                    gameState->entidades.jugador.fisica.alto = PIXELSPERUNIT;
                    gameState->entidades.jugador.fisica.velx = 0;
                    gameState->entidades.jugador.fisica.vely = 0;
                    break;

                case FASTCHEEPCHEEP:
                    gameState->entidades.enemigos[enemiesIndex].sprite = 0;
                    gameState->entidades.enemigos[enemiesIndex].estado = ALIVE;
                    gameState->entidades.enemigos[enemiesIndex].identificador = FASTCHEEPCHEEP;
                    gameState->entidades.enemigos[enemiesIndex].fisica.posx = TOWORLDPOS(j);
                    gameState->entidades.enemigos[enemiesIndex].fisica.posy = TOWORLDPOS(i);
                    gameState->entidades.enemigos[enemiesIndex].fisica.ancho = PIXELSPERUNIT;
                    gameState->entidades.enemigos[enemiesIndex].fisica.alto = PIXELSPERUNIT;
                    gameState->entidades.enemigos[enemiesIndex].fisica.velx = 0 ;             //Le puse una velocidad al cheep cheep para la izquierda
                    gameState->entidades.enemigos[enemiesIndex].fisica.vely = 0;
                    gameState->entidades.enemigos[enemiesIndex].funcionMovimiento = cheepcheep;
                    gameState->entidades.enemigos[enemiesIndex].moveAgain = 1;
                    startEnemy(&(gameState->entidades.enemigos[enemiesIndex]));
                    enemiesIndex++;
                    break;

                case SLOWCHEEPCHEEP:
                    gameState->entidades.enemigos[enemiesIndex].sprite = 0;
                    gameState->entidades.enemigos[enemiesIndex].estado = ALIVE;
                    gameState->entidades.enemigos[enemiesIndex].identificador = SLOWCHEEPCHEEP;
                    gameState->entidades.enemigos[enemiesIndex].fisica.posx = TOWORLDPOS(j);
                    gameState->entidades.enemigos[enemiesIndex].fisica.posy = TOWORLDPOS(i);
                    gameState->entidades.enemigos[enemiesIndex].fisica.ancho = PIXELSPERUNIT;
                    gameState->entidades.enemigos[enemiesIndex].fisica.alto = PIXELSPERUNIT;
                    gameState->entidades.enemigos[enemiesIndex].fisica.velx = 0 ;             //Le puse una velocidad al cheep cheep para la izquierda
                    gameState->entidades.enemigos[enemiesIndex].fisica.vely = 0;
                    gameState->entidades.enemigos[enemiesIndex].funcionMovimiento = cheepcheep;
                    gameState->entidades.enemigos[enemiesIndex].moveAgain = 1;
                    startEnemy(&(gameState->entidades.enemigos[enemiesIndex]));
                    enemiesIndex++;
                    break;

                case PULPITO:
                    gameState->entidades.enemigos[enemiesIndex].sprite = 0;
                    gameState->entidades.enemigos[enemiesIndex].estado = ALIVE;
                    gameState->entidades.enemigos[enemiesIndex].identificador = PULPITO;
                    gameState->entidades.enemigos[enemiesIndex].fisica.posx = TOWORLDPOS(j);
                    gameState->entidades.enemigos[enemiesIndex].fisica.posy = TOWORLDPOS(i);
                    gameState->entidades.enemigos[enemiesIndex].fisica.ancho = PIXELSPERUNIT;
                    gameState->entidades.enemigos[enemiesIndex].fisica.alto = PIXELSPERUNIT * 2 ;
                    gameState->entidades.enemigos[enemiesIndex].fisica.velx = 0;             //Le puse una velocidad al blooper para la izquierda
                    gameState->entidades.enemigos[enemiesIndex].fisica.vely = 0;
                    gameState->entidades.enemigos[enemiesIndex].funcionMovimiento = blooper;
                    gameState->entidades.enemigos[enemiesIndex].moveAgain = 1;
                    startEnemy(&(gameState->entidades.enemigos[enemiesIndex]));
                    enemiesIndex++;
                    break;

                case ALGA:
                    gameState->entidades.bloques[blocksIndex].sprite = 0;
                    gameState->entidades.bloques[blocksIndex].identificador = ALGA;
                    gameState->entidades.bloques[blocksIndex].fisica.posx = TOWORLDPOS(j);
                    gameState->entidades.bloques[blocksIndex].fisica.posy = TOWORLDPOS(i-2);
                    gameState->entidades.bloques[blocksIndex].fisica.ancho = PIXELSPERUNIT;
                    gameState->entidades.bloques[blocksIndex].fisica.alto = PIXELSPERUNIT * 3;
                    gameState->entidades.bloques[blocksIndex].fisica.velx = 0;
                    gameState->entidades.bloques[blocksIndex].fisica.vely = 0;
                    blocksIndex++;
                    break;
            }
        }
        horizontalBlocksCounter = 0;  //Volvemos a iniciar el contador de bloques horizontal cuando pasamos a otra fila del mapa
    }

    //Reallocamos el espacio para los bloques
    gameState->entidades.bloques = (bloque_t*) realloc(gameState->entidades.bloques , sizeof(bloque_t) * (blocksCounter+1));
    if(gameState->entidades.bloques == NULL){
        printf("Error al reallocar espacio para los bloques");
        return 1;
    }
    gameState->entidades.bloques[blocksCounter].identificador = NULLENTITIE;         //Inicializamos el ultimo elemento en nulo

    initBackUpEntities(gameState);

    return 0;
}


#endif

#if MODOJUEGO == 1

void drawPause(estadoJuego_t *gameState){
    //NO HAGO NADA, me sirve para mantener gamelogic como esta.
    //Tambien podria poner la compilacion condicional en gamelogic
}


void resetWavePosition(void){
    //NO HAGO NADA, me sirve para mantener gamelogic como esta.
    //Tambien podria poner la compilacion condicional en gamelogic
}

void drawLevel(estadoJuego_t* gameState){

    char mapLevel[16][16] = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //level Cleared
                              {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                              {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                              {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                              {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                              {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                              {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                              {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                              {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                              {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                              {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                              {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                              {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                              {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                              {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                              {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    updateCameraPosition(gameState);

    float cameraScrollX = getCameraScrollX();
    int posX = 0, posY = 0;
    int drawSize;

    int i = 0;

    while(gameState->entidades.enemigos[i].identificador != NULLENTITIE){

        if(isInsideScreenX(&gameState->entidades.enemigos[i].fisica)){
            posX = ( (int) (gameState->entidades.enemigos[i].fisica.posx - cameraScrollX) ) / PIXELSPERUNIT;
            posY = ( (int) gameState->entidades.enemigos[i].fisica.posy) / PIXELSPERUNIT;

            if(gameState->entidades.enemigos[i].identificador == FASTCHEEPCHEEP || gameState->entidades.enemigos[i].identificador == SLOWCHEEPCHEEP){
                mapLevel[posY][posX] = 1 - gameState->entidades.enemigos[i].sprite;
            }

            for (int j = 0; j < ( (int) (gameState->entidades.enemigos[i].fisica.alto / PIXELSPERUNIT) ); j++) {
                mapLevel[posY + j][posX] = 1 - gameState->entidades.enemigos[i].sprite;
            }

            drawSize = 0;

            /*  NO HARIA FALTA ESTO PARA LOS ENEMIGOS, a menos que los haya mayores a 1 bloque
             *
            if(gameState->entidades.enemigos[i].fisica.ancho / PIXELSPERUNIT <= 15){
                drawSize = gameState->entidades.enemigos[i].fisica.ancho / PIXELSPERUNIT;
            }
            else{
                drawSize = 16;
            }

            for (int j = 0; j < drawSize; j++) {

                mapLevel[posY][posX + j] = 1;
            }
            */

        }
        i++;
    }

    i = 0;

    while(gameState->entidades.bloques[i].identificador != NULLENTITIE){

        if(isInsideScreenX(&gameState->entidades.bloques[i].fisica)){

            posX = ( (int) (gameState->entidades.bloques[i].fisica.posx - cameraScrollX) ) / PIXELSPERUNIT;
            posY = ( (int) gameState->entidades.bloques[i].fisica.posy ) / PIXELSPERUNIT;

            if(gameState->entidades.bloques[i].identificador == MONEDA){
                mapLevel[posY][posX] = 1 - gameState->entidades.bloques[i].sprite;
            }
            else{
                mapLevel[posY][posX] = 1;
            }

            drawSize = 0;

            /* NO HARIA FALTA PORQUE AHORA SE INICIALIZAN BLOQUES INDIVIDUALES

            if(gameState->entidades.enemigos[i].fisica.ancho / PIXELSPERUNIT <= 15){
                drawSize = gameState->entidades.enemigos[i].fisica.ancho / PIXELSPERUNIT;
            }
            else{
                drawSize = 16;
            }

            for (int j = 0; j < drawSize; j++) {
                    mapLevel[posY][posX + j] = 1;
            }
            */

            for (int j = 0; j < ( (int) (gameState->entidades.bloques[i].fisica.alto) ) / PIXELSPERUNIT; j++) {
                    mapLevel[posY + j][posX] = 1;
            }

        }
        i++;
    }

    posX = (int)((gameState->entidades.jugador.fisica.posx - cameraScrollX)/PIXELSPERUNIT);
    posY = (int)(gameState->entidades.jugador.fisica.posy/PIXELSPERUNIT);
    mapLevel[posY][posX] = 1;

    writeDisplay(mapLevel);
}

void drawRetryScreen(estadoJuego_t *gameState){

    disp_clear();

    imprimirHighScore(gameState->entidades.jugador.vidas);

    //Ahora imprimo un corazon al lado del numero de vidas

    char retryScreen[16][16] = { {0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0}, //level Cleared
                                 {0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0},
                                 {0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0},
                                 {0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0},
                                 {0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0},
                                 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                 {0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2},
                                 {0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 2, 2, 2, 2},
                                 {0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2, 2, 2, 2},
                                 {0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2, 2, 2, 2},
                                 {0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 2, 2, 2, 2},
                                 {0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
                                 {0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0}
    };

    writeDisplay(retryScreen);
}

void drawNextLevelScreen(estadoJuego_t *gameState){

    char levelCleared[16][16] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //level Cleared
                                 {0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0},
                                 {0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0},
                                 {0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0},
                                 {0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0},
                                 {0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1},
                                 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                 {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
                                 {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
                                 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                 {1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1},
                                 {1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0},
                                 {1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1},
                                 {1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0},
                                 {1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1},
                                 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };
    writeDisplay(levelCleared);
}

void drawGameOverScreen(estadoJuego_t* gameState){

    char highScoreTextMenu[16][16] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                      {1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1},
                                      {1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1},
                                      {1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1},
                                      {1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1},
                                      {1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1},
                                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                      {1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1},
                                      {1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
                                      {1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1},
                                      {0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
                                      {1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1},
                                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    writeDisplay(highScoreTextMenu);

    sleep(1);

    imprimirHighScore(gameState->gameUI.score);

    //AHORA LE AGREGO SIN QUITAR LOS NUMEROS ESCRITOS ANTERIORMENTE, "SCORE"

    char highScore[16][16]= { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                              {1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1},
                              {1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
                              {1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1},
                              {0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
                              {1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1},
                              {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                              {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                              {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                              {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
                              {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
                              {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
                              {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
                              {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
                              {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
                              {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}
    };

    writeDisplay(highScore);
}

void drawGameOverScreenHighScore(estadoJuego_t* gameState){

    char levelCleared[16][16] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //level Cleared
                                 {0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1},
                                 {1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0},
                                 {1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1},
                                 {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0},
                                 {0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1},
                                 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                 {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
                                 {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
                                 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                 {0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1},
                                 {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1},
                                 {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1},
                                 {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0},
                                 {0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1},
                                 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };
    writeDisplay(levelCleared);

}

int initEntities(estadoJuego_t* gameState){ //PARA LA RASPI: NO AHORRA ESPACIO HACIENDO BLOQUES MAS ANCHOS

    int blocksCounter = 0;
    int enemiesCounter = 0;
    int blocksIndex = 0;
    int enemiesIndex = 0;
    char currentBlock;

    //Calculamos la cantidad de enemigos y de bloques que hay en el mapa
    for(int i = 0; i < gameState->level.levelHeight; i++){
        for(int j = 0; j < gameState->level.levelWidht; j++) {
            switch(gameState->level.level[i][j]) {
                case LADRILLO:
                case MONEDA:
                case TOPPIPE:
                case MIDDLEPIPE:
                case ALGA:
                    blocksCounter++;
                    break;

                case FASTCHEEPCHEEP:
                case SLOWCHEEPCHEEP:
                case PULPITO:
                    enemiesCounter++;
                    break;
            }
        }
    }

    //Reservamos el espacio para los bloques
    gameState->entidades.bloques = (bloque_t*) malloc(sizeof(bloque_t) * blocksCounter);
    if(gameState->entidades.bloques == NULL){
        printf("Error al reservar espacio para los bloques");
        return 1;
    }


    //Reservamos el espacio para los enemigos
    gameState->entidades.enemigos = (enemigo_t*) calloc((enemiesCounter+1), sizeof(enemigo_t));
    if(gameState->entidades.enemigos == NULL){
        printf("Error al reservar espacio para los enemigos");
        return 1;
    }
    gameState->entidades.enemigos[enemiesCounter].identificador = NULLENTITIE;

    for(int i = 0; i < gameState->level.levelHeight; i++){
        for(int j = 0; j < gameState->level.levelWidht; j++){

            //Agarramos el siguiente bloque
            currentBlock = gameState->level.level[i][j];

            switch (currentBlock) {
                case LADRILLO:

                   gameState->entidades.bloques[blocksIndex].sprite = 0;
                   gameState->entidades.bloques[blocksIndex].identificador = LADRILLO;
                   gameState->entidades.bloques[blocksIndex].fisica.posx = TOWORLDPOS(j);
                   gameState->entidades.bloques[blocksIndex].fisica.posy = TOWORLDPOS(i);
                   gameState->entidades.bloques[blocksIndex].fisica.ancho = PIXELSPERUNIT;
                   gameState->entidades.bloques[blocksIndex].fisica.alto = PIXELSPERUNIT;
                   gameState->entidades.bloques[blocksIndex].fisica.velx = 0;
                   gameState->entidades.bloques[blocksIndex].fisica.vely = 0;
                   blocksIndex++;

                   break;

                case MONEDA:
                    gameState->entidades.bloques[blocksIndex].sprite = 0;
                    gameState->entidades.bloques[blocksIndex].identificador = MONEDA;
                    gameState->entidades.bloques[blocksIndex].fisica.posx = TOWORLDPOS(j);
                    gameState->entidades.bloques[blocksIndex].fisica.posy = TOWORLDPOS(i);
                    gameState->entidades.bloques[blocksIndex].fisica.ancho = PIXELSPERUNIT;
                    gameState->entidades.bloques[blocksIndex].fisica.alto = PIXELSPERUNIT;
                    gameState->entidades.bloques[blocksIndex].fisica.velx = 0;
                    gameState->entidades.bloques[blocksIndex].fisica.vely = 0;
                    blocksIndex++;
                    break;

                case TOPPIPE:
                    gameState->entidades.bloques[blocksIndex].sprite = 0;
                    gameState->entidades.bloques[blocksIndex].identificador = TOPPIPE;
                    gameState->entidades.bloques[blocksIndex].fisica.posx = TOWORLDPOS(j);
                    gameState->entidades.bloques[blocksIndex].fisica.posy = TOWORLDPOS(i);
                    gameState->entidades.bloques[blocksIndex].fisica.ancho = PIXELSPERUNIT;
                    gameState->entidades.bloques[blocksIndex].fisica.alto = PIXELSPERUNIT;
                    gameState->entidades.bloques[blocksIndex].fisica.velx = 0;
                    gameState->entidades.bloques[blocksIndex].fisica.vely = 0;
                    blocksIndex++;
                    break;

                case MIDDLEPIPE:
                    gameState->entidades.bloques[blocksIndex].sprite = 0;
                    gameState->entidades.bloques[blocksIndex].identificador = MIDDLEPIPE;
                    gameState->entidades.bloques[blocksIndex].fisica.posx = TOWORLDPOS(j);
                    gameState->entidades.bloques[blocksIndex].fisica.posy = TOWORLDPOS(i);
                    gameState->entidades.bloques[blocksIndex].fisica.ancho = PIXELSPERUNIT;
                    gameState->entidades.bloques[blocksIndex].fisica.alto = PIXELSPERUNIT;
                    gameState->entidades.bloques[blocksIndex].fisica.velx = 0;
                    gameState->entidades.bloques[blocksIndex].fisica.vely = 0;
                    blocksIndex++;
                    break;

                case JUGADOR:
                    gameState->entidades.jugador.sobreBloque = 0;
                    gameState->entidades.jugador.estado = ALIVE;
                    gameState->entidades.jugador.vidas = 0;
                    gameState->entidades.jugador.powerUpsState = SMALL;
                    gameState->entidades.jugador.sprite = 0;
                    gameState->entidades.jugador.fisica.posx = TOWORLDPOS(j);
                    gameState->entidades.jugador.fisica.posy = TOWORLDPOS(i);
                    gameState->entidades.jugador.fisica.ancho = PIXELSPERUNIT;
                    gameState->entidades.jugador.fisica.alto = PIXELSPERUNIT;
                    gameState->entidades.jugador.fisica.velx = 0;
                    gameState->entidades.jugador.fisica.vely = 0;
                    break;

                case FASTCHEEPCHEEP:
                    gameState->entidades.enemigos[enemiesIndex].sprite = 0;
                    gameState->entidades.enemigos[enemiesIndex].estado = ALIVE;
                    gameState->entidades.enemigos[enemiesIndex].identificador = FASTCHEEPCHEEP;
                    gameState->entidades.enemigos[enemiesIndex].fisica.posx = TOWORLDPOS(j);
                    gameState->entidades.enemigos[enemiesIndex].fisica.posy = TOWORLDPOS(i);
                    gameState->entidades.enemigos[enemiesIndex].fisica.ancho = PIXELSPERUNIT;
                    gameState->entidades.enemigos[enemiesIndex].fisica.alto = PIXELSPERUNIT;
                    gameState->entidades.enemigos[enemiesIndex].fisica.velx = 0 ;             //Le puse una velocidad al cheep cheep para la izquierda
                    gameState->entidades.enemigos[enemiesIndex].fisica.vely = 0;
                    gameState->entidades.enemigos[enemiesIndex].funcionMovimiento = cheepcheep;
                    gameState->entidades.enemigos[enemiesIndex].moveAgain = 1;
                    startEnemy(&(gameState->entidades.enemigos[enemiesIndex]));
                    enemiesIndex++;
                    break;

                case SLOWCHEEPCHEEP:
                    gameState->entidades.enemigos[enemiesIndex].sprite = 0;
                    gameState->entidades.enemigos[enemiesIndex].estado = ALIVE;
                    gameState->entidades.enemigos[enemiesIndex].identificador = SLOWCHEEPCHEEP;
                    gameState->entidades.enemigos[enemiesIndex].fisica.posx = TOWORLDPOS(j);
                    gameState->entidades.enemigos[enemiesIndex].fisica.posy = TOWORLDPOS(i);
                    gameState->entidades.enemigos[enemiesIndex].fisica.ancho = PIXELSPERUNIT;
                    gameState->entidades.enemigos[enemiesIndex].fisica.alto = PIXELSPERUNIT;
                    gameState->entidades.enemigos[enemiesIndex].fisica.velx = 0 ;             //Le puse una velocidad al cheep cheep para la izquierda
                    gameState->entidades.enemigos[enemiesIndex].fisica.vely = 0;
                    gameState->entidades.enemigos[enemiesIndex].funcionMovimiento = cheepcheep;
                    gameState->entidades.enemigos[enemiesIndex].moveAgain = 1;
                    startEnemy(&(gameState->entidades.enemigos[enemiesIndex]));
                    enemiesIndex++;
                    break;

                case PULPITO:
                    gameState->entidades.enemigos[enemiesIndex].sprite = 0;
                    gameState->entidades.enemigos[enemiesIndex].estado = ALIVE;
                    gameState->entidades.enemigos[enemiesIndex].identificador = PULPITO;
                    gameState->entidades.enemigos[enemiesIndex].fisica.posx = TOWORLDPOS(j);
                    gameState->entidades.enemigos[enemiesIndex].fisica.posy = TOWORLDPOS(i);
                    gameState->entidades.enemigos[enemiesIndex].fisica.ancho = PIXELSPERUNIT;
                    gameState->entidades.enemigos[enemiesIndex].fisica.alto = PIXELSPERUNIT * 2 ;
                    gameState->entidades.enemigos[enemiesIndex].fisica.velx = 0;             //Le puse una velocidad al blooper para la izquierda
                    gameState->entidades.enemigos[enemiesIndex].fisica.vely = 0;
                    gameState->entidades.enemigos[enemiesIndex].funcionMovimiento = blooper;
                    gameState->entidades.enemigos[enemiesIndex].moveAgain = 1;
                    startEnemy(&(gameState->entidades.enemigos[enemiesIndex]));
                    enemiesIndex++;
                    break;

                case ALGA:
                    gameState->entidades.bloques[blocksIndex].sprite = 0;
                    gameState->entidades.bloques[blocksIndex].identificador = ALGA;
                    gameState->entidades.bloques[blocksIndex].fisica.posx = TOWORLDPOS(j);
                    gameState->entidades.bloques[blocksIndex].fisica.posy = TOWORLDPOS(i-2);
                    gameState->entidades.bloques[blocksIndex].fisica.ancho = PIXELSPERUNIT;
                    gameState->entidades.bloques[blocksIndex].fisica.alto = PIXELSPERUNIT * 3;
                    gameState->entidades.bloques[blocksIndex].fisica.velx = 0;
                    gameState->entidades.bloques[blocksIndex].fisica.vely = 0;
                    blocksIndex++;
                    break;
            }
        }
    }

    //Reallocamos el espacio para los bloques
    gameState->entidades.bloques = (bloque_t*) realloc(gameState->entidades.bloques , sizeof(bloque_t) * (blocksCounter+1));
    if(gameState->entidades.bloques == NULL){
        printf("Error al reallocar espacio para los bloques");
        return 1;
    }
    gameState->entidades.bloques[blocksCounter].identificador = NULLENTITIE;         //Inicializamos el ultimo elemento en nulo

    initBackUpEntities(gameState);

    return 0;
}

#endif

int cargarMapa(level_t* level, int id) {

    FILE *mapData;
    int i = 0;
    int j = 0;
    int read = 0;
    int auxCont = 0;
    int borderCount = 0;

    int error = openLevelData(&mapData, id-1);

    if (error != 1){
        countColumns(level, mapData);
        level->level = (int **) calloc( level->levelHeight, sizeof(int *));
        for (i = 0; i < level->levelHeight; i++) {
            (level->level)[i] = (int*) malloc(level->levelWidht * sizeof(int));    //FIXME: Aca si moris muchas veces tira segmentation
        }

        i = 0;
        do {
            read = fgetc(mapData);

            switch (read) {
                case MONEDA:
                case TOPPIPE:
                case MIDDLEPIPE:
                case FASTCHEEPCHEEP:
                case SLOWCHEEPCHEEP:
                case PULPITO:
                case ALGA:
                case LADRILLO:
                case JUGADOR:
                case NADA:
                    level->level[i][j] = read;
                    j++;
                    auxCont = 0;
                    break;
                case ';':
                    auxCont++;
                    break;
                case BORDE:
                    borderCount++;
                    if (borderCount == 2){
                        i++;
                        j = 0;
                        borderCount = 0;
                    }
                    auxCont = 0;
                    break;
                default:
                    break;

            }
            if (auxCont == 2){
                level->level[i][j] = NADA;
                j++;
                auxCont--;
            }
        }while (read != EOF);
    }
    else{
        printf("Error al cargar el mapa\n");
    }

    fclose(mapData);
    return error;
}

void destroyMap(estadoJuego_t* gameState){
    for(int i = 0; i < gameState->level.levelHeight; i++){
        free(gameState->level.level[i]);
    }
    free(gameState->level.level);
}

static int countColumns(level_t* level, FILE* mapData){

    int error = 0;
    int colNum = 0;
    int read = 0;
    int borderCount = 0;
    int auxCont = 0;

    do {
        read = fgetc(mapData);

        switch (read) {
            case FASTCHEEPCHEEP:
            case SLOWCHEEPCHEEP:
            case PULPITO:
            case MONEDA:
            case TOPPIPE:
            case MIDDLEPIPE:
            case ALGA:
            case LADRILLO:
            case JUGADOR:
            case NADA:
                colNum++;
                auxCont = 0;
                break;
            case ';':
                auxCont++;
                break;
            case BORDE:
                borderCount++;
                auxCont = 0;
                break;
            default:
                break;

        }

        if (auxCont == 2){
            colNum++;
            auxCont--;
        }

    }while (read != EOF);

    if (borderCount%2 == 1){
        error = 1;
    } else{
        level->levelHeight = borderCount/2;
        level->levelWidht = colNum/level->levelHeight;
    }
    fseek( mapData, 0, SEEK_SET );

    return error;
}

int wasNewHighScoreAchieved(estadoJuego_t* gameState){

    int newHighScore = 0;
    FILE* scoreFileData = fopen(getScoreFilePath(), "r+");

    if(scoreFileData == NULL){
        printf("Error al guardar el nuevo topscore\n");
    }
    else {
        for (int i = 0; i < gameState->maxTopScoreEntries && !newHighScore; i++) {
            if (gameState->bestScores[i] < gameState->gameUI.score) {
                newHighScore = 1;
            }
        }
    }

    fclose(scoreFileData);
    return newHighScore;
}

void saveNewHighScore(estadoJuego_t* gameState){

    int newHighScorePos = -1;
    FILE* scoreFileData = fopen(getScoreFilePath(), "w+");


    for(int i = 0; i < gameState->maxTopScoreEntries && newHighScorePos == -1; i++){
        if(gameState->bestScores[i] < gameState->gameUI.score){
            newHighScorePos = i;
        }
    }

    for(int i = gameState->maxTopScoreEntries-1; newHighScorePos < i; i--){
        gameState->bestScores[i] = gameState->bestScores[i-1];
        strcpy(gameState->bestScoresName[i], gameState->bestScoresName[i-1]);
    }

    gameState->bestScores[newHighScorePos] = gameState->gameUI.score;
    strcpy(gameState->bestScoresName[newHighScorePos], gameState->pPlayerName);

    fprintf(scoreFileData, "%d\n", gameState->maxTopScoreEntries);
    for (int i = 0; i < gameState->maxTopScoreEntries; i++) {
        fprintf(scoreFileData, "%d %s\n", gameState->bestScores[i], gameState->bestScoresName[i]);
    }

    fprintf(scoreFileData, "\n%s\n%s\n", "//Cantidad de valores", "//Lista de puntaje - nombre");
    fflush(scoreFileData);


    fclose(scoreFileData);
}

void initUI(gameUI_t* gameUI){
    gameUI->time = MAXLEVELTIME;
    gameUI->score = 0;
    gameUI->coins = 0;
    gameUI->level = ONE;
}

void resetEntitiesState(estadoJuego_t* gameState){

    for(int i = 0; gameState->entidades.enemigos[i].identificador != NULLENTITIE; i++){
        gameState->entidades.enemigos[i].sprite = gameState->defaultEntities.enemigos[i].sprite;
        gameState->entidades.enemigos[i].fisica.posx = gameState->defaultEntities.enemigos[i].fisica.posx;
        gameState->entidades.enemigos[i].fisica.posy = gameState->defaultEntities.enemigos[i].fisica.posy;
    }

    for(int i = 0; gameState->entidades.bloques[i].identificador != NULLENTITIE; i++){
        gameState->entidades.bloques[i].sprite = gameState->defaultEntities.bloques[i].sprite;
        gameState->entidades.bloques[i].fisica = gameState->defaultEntities.bloques[i].fisica;
    }

    gameState->entidades.jugador.sobreBloque = gameState->defaultEntities.jugador.sobreBloque;
    gameState->entidades.jugador.powerUpsState = gameState->defaultEntities.jugador.powerUpsState;
    gameState->entidades.jugador.estado = gameState->defaultEntities.jugador.estado;
    gameState->entidades.jugador.sprite = gameState->defaultEntities.jugador.sprite;
    gameState->entidades.jugador.fisica = gameState->defaultEntities.jugador.fisica;
    gameState->entidades.jugador.isMoving = gameState->defaultEntities.jugador.isMoving;

}

static void initBackUpEntities(estadoJuego_t* gameState){

    int blocksCounter = 0;
    int enemiesCounter = 0;

    //CUENTO CUANTA MEMORIA DEBERE RESERVAR
    while(gameState->entidades.bloques[blocksCounter].identificador != NULLENTITIE){
        blocksCounter++;
    }
    while(gameState->entidades.enemigos[enemiesCounter].identificador != NULLENTITIE){
        enemiesCounter++;
    }

    //RESERVO ESA CANTIDAD DE MEMORIA

    gameState->defaultEntities.bloques = (bloque_t*) malloc(sizeof(bloque_t) * (blocksCounter+1) );
    if(gameState->defaultEntities.bloques == NULL){
        printf("Error al reservar espacio para el backup de los bloques");
    }

    gameState->defaultEntities.enemigos = (enemigo_t*) malloc(sizeof(enemigo_t) * (enemiesCounter+1));
    if(gameState->defaultEntities.enemigos == NULL){
        printf("Error al reservar espacio para el backup de los enemigos");
    }

    //POR FIN HAGO EL BACKUP

    int i = 0;
    while(gameState->entidades.bloques[i].identificador != NULLENTITIE){
        gameState->defaultEntities.bloques[i] = gameState->entidades.bloques[i];
        i++;
    }

    i=0;
    while(gameState->entidades.enemigos[i].identificador != NULLENTITIE){
        gameState->defaultEntities.enemigos[i] = gameState->entidades.enemigos[i];
        i++;
    }

    gameState->defaultEntities.jugador = gameState->entidades.jugador;

}

void destroyEntities(estadoJuego_t * gameState){
    free(gameState->entidades.bloques);
    free(gameState->entidades.enemigos);
    free(gameState->defaultEntities.bloques);
    free(gameState->defaultEntities.enemigos);
}
