/***************************************************************************//**
  file      level.c
  @brief    Funciones de inicializacion, manejo y dibujado de aquello relacionado al nivel
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "level.h"
#include <stdlib.h>
#include <string.h>

//TODO: Si se rompio en la raspi es porque meti times.h y gamelogic.h aca, en caso contrario borra este comentario

#if MODOJUEGO == ALLEGRO

#include "allegro.h"


#elif MODOJUEGO == RASPI

#include "menu.h"

#endif

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define TOWORLDPOS(v) ( (v) * PIXELSPERUNIT)

#if MODOJUEGO == ALLEGRO

#define UICOLOR al_map_rgb(76,25,153)
#define UIWARNINGCOLOR al_map_rgb(255, 0, 0)
#define SKYCOLOR al_map_rgb(153, 195, 219)

#elif MODOJUEGO == RASPI

#define INSIDERASPISCREEN(posX, posY) (((0 <= (posX) && (posX) <= 15) && (0 <= (posY) && (posY) <= 15)) ? (1) : (0))

#endif

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

#if MODOJUEGO == ALLEGRO

//Estructura de la ola
typedef struct{
    int offsetX;
    int moveDelay;
    int offsetXRecord;
    float scale;
}wave_t;

#endif

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

#if MODOJUEGO == ALLEGRO

/*Dibuja el score, el timer, las vidas y las monedas en el momento que dibuja el nivel */
static void drawGameUI(estadoJuego_t *gameState);

#endif

/* Hago un backup de la salida de initEntities asi no tengo que ejecutarla de vuelta al morir*/
static void initBackUpEntities(estadoJuego_t* gameState);

/* Cuenta las columnas de un nivel para conocer el ancho del mismo */
static int countColumns(level_t* level, FILE* mapData);

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

#if MODOJUEGO == ALLEGRO

static wave_t wave;

#endif

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

#if MODOJUEGO == ALLEGRO

void drawLevel(estadoJuego_t *gameState){

    bufferRecursos_t *resourceBuffer = &gameState->buffer;
    int playerSprite;
    int flip_player = 0;
    float scrollX;
    int i = 0;


    updateCameraPosition(gameState);
    scrollX = getCameraScrollX();

    al_clear_to_color(SKYCOLOR);

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

    if(scrollX - (float)wave.offsetX + SCREENWIDHT >= al_get_bitmap_width(resourceBuffer->image[WAVESPRITE]) * wave.scale){
        wave.offsetX += (int)((float)al_get_bitmap_width(resourceBuffer->image[WAVESPRITE]) * wave.scale) - SCREENWIDHT;
    }
    al_draw_scaled_bitmap(resourceBuffer->image[WAVESPRITE], 0, 0, (float)al_get_bitmap_width(resourceBuffer->image[WAVESPRITE]), (float)al_get_bitmap_height(resourceBuffer->image[WAVESPRITE]), (float)wave.offsetX - scrollX, PIXELSPERUNIT,
                          (float)al_get_bitmap_width(resourceBuffer->image[WAVESPRITE]) * wave.scale, (float)al_get_bitmap_height(resourceBuffer->image[WAVESPRITE]) * wave.scale, 0);

    //Mientras no se hayan leido todos los bloques, dibujamos el siguiente
    bloque_t bloque;

        while(gameState->entidades.bloques[i].identificador != NULLENTITIE && wasLevelInitialized()){
        bloque = gameState->entidades.bloques[i];
        switch (bloque.identificador){
            case ALGA:
                al_draw_scaled_bitmap(resourceBuffer->image[ALGASPRITE1 + bloque.sprite], 0, 0, (float)al_get_bitmap_width(resourceBuffer->image[ALGASPRITE1]), (float)al_get_bitmap_height(resourceBuffer->image[ALGASPRITE1]), bloque.fisica.posx - scrollX, bloque.fisica.posy,
                                      (float)bloque.fisica.ancho, (float)bloque.fisica.alto, 0);
                break;

            case LADRILLO:
                for (int j = 0; j < bloque.fisica.ancho/PIXELSPERUNIT; j++) {
                    al_draw_scaled_bitmap(resourceBuffer->image[PISOSPRITE], 0, 0, (float)al_get_bitmap_width(resourceBuffer->image[PISOSPRITE]),
                                          (float)al_get_bitmap_height(resourceBuffer->image[PISOSPRITE]), bloque.fisica.posx + (float)j * PIXELSPERUNIT - scrollX,
                                          bloque.fisica.posy,PIXELSPERUNIT, PIXELSPERUNIT, 0);
                }
                break;

            case MONEDA:
                al_draw_scaled_bitmap(resourceBuffer->image[COINSPRITE1 + bloque.sprite], 0, 0, (float)al_get_bitmap_width(resourceBuffer->image[COINSPRITE1]), (float)al_get_bitmap_height(resourceBuffer->image[COINSPRITE1]), bloque.fisica.posx - scrollX, bloque.fisica.posy,
                                      (float)bloque.fisica.ancho, (float)bloque.fisica.alto, 0);
                break;

            case MUSHROOM:
                al_draw_scaled_bitmap(resourceBuffer->image[COMMONMUSHROOMSPRITE], 0, 0, (float)al_get_bitmap_width(resourceBuffer->image[COMMONMUSHROOMSPRITE]), (float)al_get_bitmap_height(resourceBuffer->image[COMMONMUSHROOMSPRITE]), bloque.fisica.posx - scrollX, bloque.fisica.posy,
                                      (float)bloque.fisica.ancho, (float)bloque.fisica.alto, 0);
                break;

            case TOPPIPE:
                al_draw_scaled_bitmap(resourceBuffer->image[PIPETOPSPRITE], 0, 0, (float)al_get_bitmap_width(resourceBuffer->image[PIPETOPSPRITE]), (float)al_get_bitmap_height(resourceBuffer->image[PIPETOPSPRITE]), bloque.fisica.posx - scrollX, bloque.fisica.posy,
                                      (float)bloque.fisica.ancho, (float)bloque.fisica.alto, 0);
                break;

            case MIDDLEPIPE:
                al_draw_scaled_bitmap(resourceBuffer->image[PIPEMIDDLESPRITE], 0, 0, (float)al_get_bitmap_width(resourceBuffer->image[PIPEMIDDLESPRITE]), (float)al_get_bitmap_height(resourceBuffer->image[PIPEMIDDLESPRITE]), bloque.fisica.posx - scrollX, bloque.fisica.posy,
                                      (float)bloque.fisica.ancho, (float)bloque.fisica.alto, 0);
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
                al_draw_scaled_bitmap(resourceBuffer->image[BLOOPERSPRITE1 + enemigo.sprite], 0, 0, (float)al_get_bitmap_width(resourceBuffer->image[BLOOPERSPRITE1]),  (float)al_get_bitmap_height(resourceBuffer->image[BLOOPERSPRITE1 + enemigo.sprite]), enemigo.fisica.posx - scrollX, enemigo.fisica.posy,
                                      (float)enemigo.fisica.ancho, (float)enemigo.fisica.alto, 0);
                break;

            case FASTCHEEPCHEEP:
                al_draw_scaled_bitmap(resourceBuffer->image[CHEEPCHEEPSPRITE1 + enemigo.sprite], 0, 0, (float)al_get_bitmap_width(resourceBuffer->image[CHEEPCHEEPSPRITE1]),  (float)al_get_bitmap_height(resourceBuffer->image[CHEEPCHEEPSPRITE1]), enemigo.fisica.posx-scrollX, enemigo.fisica.posy,
                                      (float)enemigo.fisica.ancho, (float)enemigo.fisica.alto, 0);
                break;

            case SLOWCHEEPCHEEP:
                al_draw_scaled_bitmap(resourceBuffer->image[CHEEPCHEEPSSLOWSPRITE1 + enemigo.sprite], 0, 0, (float)al_get_bitmap_width(resourceBuffer->image[CHEEPCHEEPSSLOWSPRITE1]),  (float)al_get_bitmap_height(resourceBuffer->image[CHEEPCHEEPSSLOWSPRITE1]), enemigo.fisica.posx-scrollX, enemigo.fisica.posy,
                                      (float)enemigo.fisica.ancho, (float)enemigo.fisica.alto, 0);
                break;
        }

        i++;
    }

    drawGameUI(gameState);

    //Dibujamos al jugador
    fisica_t jugador = gameState->entidades.jugador.fisica;
    if(jugador.velx >= 0) {
        flip_player = 0;
    }
    else {
        flip_player = ALLEGRO_FLIP_HORIZONTAL;
    }


    if (gameState->entidades.jugador.powerUpsState == SMALL) {
        playerSprite = MATIASIDLESPRITE + gameState->entidades.jugador.sprite;
    } else if (gameState->entidades.jugador.powerUpsState == BIG) {
        playerSprite = MATIASIDLEBIGSPRITE + gameState->entidades.jugador.sprite;
    }

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

    static char playerName[MAXPLAYERNAME] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
    static char auxString[20];

    gameState->pPlayerName = playerName;

    al_clear_to_color(SKYCOLOR);

    if(wasNewHighScoreAchieved(gameState)) {
        sprintf(auxString, "%s", "GAME OVER");
        al_draw_text(gameState->buffer.font[SUPERMARIOFONT140], al_map_rgb(200, 16, 84), (float)SCREENWIDHT/2 - 170, (float)SCREENHEIGHT/2 - 200, 0, auxString);

        sprintf(auxString, "%s", "NEW HIGH SCORE !!");
        al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], al_map_rgb(57, 16, 84), (float)SCREENWIDHT / 2 + 40, (float)SCREENHEIGHT / 2 - 75, 0, auxString);

        sprintf(auxString, "%d", gameState->gameUI.score);
        al_draw_text(gameState->buffer.font[SUPERMARIOFONT120], al_map_rgb(57, 16, 84), (float)SCREENWIDHT / 2 - 10, (float)SCREENHEIGHT / 2 + 50, 0, auxString);

        sprintf(auxString, "%s", "Enter your name:");
        al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], al_map_rgb(57, 16, 84), (float)SCREENWIDHT / 2 - 500, (float)SCREENHEIGHT / 2 + 200, 0, auxString);
        al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], al_map_rgb(57, 16, 84), (float)SCREENWIDHT / 2,(float)SCREENHEIGHT / 2 + 200, 0, playerName);
    }
    else{
        sprintf(auxString, "%s", "GAME OVER");
        al_draw_text(gameState->buffer.font[SUPERMARIOFONT140], al_map_rgb(200, 16, 84), (float)SCREENWIDHT/2 - 170, (float)SCREENHEIGHT/2 - 50, 0, auxString);
    }

    al_flip_display();
}

void drawRetryScreen(estadoJuego_t *gameState){

    char auxToString[10];

    al_clear_to_color(SKYCOLOR);

    //Dibujamos a matias
    image_t playerImg = gameState->buffer.image[MATIASIDLESPRITE];
    al_draw_scaled_bitmap(playerImg, 0, 0, (float)al_get_bitmap_width(playerImg), (float)al_get_bitmap_height(playerImg),(float)SCREENWIDHT/2 - 70, (float)SCREENHEIGHT/2, (float)al_get_bitmap_width(playerImg)*4, (float)al_get_bitmap_height(playerImg)*4, 0);

    //Al costado de matias dibujamos las vidas restantes
    sprintf(auxToString, "%d", gameState->entidades.jugador.vidas);
    al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], al_map_rgb(57, 16, 84), (float)SCREENWIDHT / 2 + 10, (float)SCREENHEIGHT / 2, 0, "X");
    al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], al_map_rgb(57, 16, 84), (float)SCREENWIDHT / 2 + 40, (float)SCREENHEIGHT / 2, 0, auxToString);

    al_flip_display();
}

void drawNextLevelScreen(estadoJuego_t *gameState){

    char auxToString[10];
    char auxToString2[10];

    al_clear_to_color(SKYCOLOR);

    image_t playerImg = gameState->buffer.image[MATIASIDLESPRITE];
    al_draw_scaled_bitmap(playerImg, 0, 0, (float)al_get_bitmap_width(playerImg), (float)al_get_bitmap_height(playerImg),(float)SCREENWIDHT/2 - 70, (float)SCREENHEIGHT/2, (float)al_get_bitmap_width(playerImg)*4, (float)al_get_bitmap_height(playerImg)*4, 0);

    sprintf(auxToString, "%d", gameState->entidades.jugador.vidas); //hacemos que las vidas sean imprimibles
    sprintf(auxToString2, "%d", gameState->gameUI.score);   //hacemos que el score sea imprimible

    //Dibujamos las vidas restantes
    al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], al_map_rgb(57, 16, 84), (float)SCREENWIDHT / 2 + 10, (float)SCREENHEIGHT / 2, 0, "X");
    al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], al_map_rgb(57, 16, 84), (float)SCREENWIDHT / 2 + 40, (float)SCREENHEIGHT / 2, 0, auxToString);

    //Dibujamos el puntaje que lleva en el momento
    al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], al_map_rgb(57, 16, 84), (float)SCREENWIDHT / 2 - 80, (float)SCREENHEIGHT / 2 + 100, 0, "SCORE = ");
    al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], al_map_rgb(57, 16, 84), (float)SCREENWIDHT / 2 + 50, (float)SCREENHEIGHT / 2 + 100, 0, auxToString2);

    al_draw_text(gameState->buffer.font[SUPERMARIOFONT80], al_map_rgb(200, 16, 84), (float)SCREENWIDHT/2 - 160, (float)SCREENHEIGHT/5, 0, "LEVEL CLEARED");

    al_flip_display();

}

void drawPause(estadoJuego_t *gameState){

    al_clear_to_color(al_map_rgb(20, 230, 230));

    //dibujo las 3 opciones del menu de pausa
    al_draw_text(gameState->buffer.font[SUPERMARIOFONT120], al_map_rgb(200, 16, 84), (float)SCREENWIDHT/2 - 88, (float)SCREENHEIGHT/6, 0, "PAUSE");
    al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], al_map_rgb(57, 16, 84), (float)SCREENWIDHT / 2 - 57, (float)SCREENHEIGHT / 2, 0, "RESUME");
    al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], al_map_rgb(57, 16, 84), (float)SCREENWIDHT / 2 - 110, (float)SCREENHEIGHT / 2 + 100, 0, "BACK TO MENU");

    for(int i = FONDOMENU; i <= FLECHAMENU; i++){   //Busco la imagen de la flecha del menu para dibujarla aqui
        image_t currentImg = gameState->buffer.image[i];
        int arrowPosition;

        if(i == FLECHAMENU){
            arrowPosition = gameState->pauseSelection;
            al_draw_scaled_bitmap(currentImg, 0, 0, (float)al_get_bitmap_width(currentImg), (float)al_get_bitmap_height(currentImg),
                                  (float)SCREENWIDHT/2 - 175, (float)SCREENHEIGHT/2 + 100.0f*(float)arrowPosition, (float)al_get_bitmap_width(currentImg) * 3,
                                  (float)al_get_bitmap_height(currentImg) * 4, 0);

        }
    }

    al_flip_display();
}

#elif MODOJUEGO == RASPI

void drawPause(estadoJuego_t *gameState){
    //NO HAGO NADA, me sirve para mantener render como esta.
    //Tambien podria poner la compilacion condicional
}


void resetWavePosition(void){
    //NO HAGO NADA, me sirve para mantener gamelogic como esta.
    //Tambien podria poner la compilacion condicional
}

void drawLevel(estadoJuego_t* gameState){

    char mapLevel[16][16] = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //Preparo el mapa vacio
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

    updateCameraPosition(gameState);    //Actualizo el scroll

    float cameraScrollX = getCameraScrollX();
    int posX, posY, actualSprite;

    //Dibujamos los enemigos
    int i = 0;
    //Reviso a todos los enemigos y los dibujo siempre y cuando esten dentro de la pantalla
    while(gameState->entidades.enemigos[i].identificador != NULLENTITIE){

        if(gameState->entidades.enemigos[i].identificador == FASTCHEEPCHEEP || gameState->entidades.enemigos[i].identificador == SLOWCHEEPCHEEP){
            actualSprite = gameState->entidades.enemigos[i].sprite;
        }
        else{
            actualSprite = 0;
        }

        posX = ( (int) (gameState->entidades.enemigos[i].fisica.posx - cameraScrollX) ) / PIXELSPERUNIT;
        posY = ( (int) gameState->entidades.enemigos[i].fisica.posy) / PIXELSPERUNIT;

        for (int j = 0; j < ( (int) (gameState->entidades.enemigos[i].fisica.alto / PIXELSPERUNIT) ); j++) {
            if (INSIDERASPISCREEN(posY + j, posX)) {
                mapLevel[posY + j][posX] = 1 - actualSprite;
            }
        }

        for (int j = 0; j < ( (int) (gameState->entidades.enemigos[i].fisica.ancho / PIXELSPERUNIT) ); j++) {
            if (INSIDERASPISCREEN(posY, posX+j)) {
                mapLevel[posY][posX+j] = 1 - actualSprite;
            }
        }

        i++;
    }


    //Reviso a todos los bloques y los dibujo siempre y cuando esten dentro de la pantalla
    i = 0;
    while(gameState->entidades.bloques[i].identificador != NULLENTITIE) {

        if (gameState->entidades.bloques[i].identificador == TOPPIPE ||
            gameState->entidades.bloques[i].identificador == MIDDLEPIPE ||
            gameState->entidades.bloques[i].identificador == MUSHROOM ||
            gameState->entidades.bloques[i].identificador == MONEDA) {
            actualSprite = gameState->entidades.bloques[i].sprite;
        } else {
            actualSprite = 0;
        }

        posX = ((int) (gameState->entidades.bloques[i].fisica.posx - cameraScrollX)) / PIXELSPERUNIT;
        posY = ((int) gameState->entidades.bloques[i].fisica.posy) / PIXELSPERUNIT;

        for (int j = 0; j < ((int) (gameState->entidades.bloques[i].fisica.alto / PIXELSPERUNIT)); j++) {
            if (INSIDERASPISCREEN(posY + j, posX)) {
                mapLevel[posY + j][posX] = 1 - actualSprite;
            }
        }

        for (int j = 0; j < ((int) (gameState->entidades.bloques[i].fisica.ancho / PIXELSPERUNIT)); j++) {
            if (INSIDERASPISCREEN( posY, posX + j)) {
                mapLevel[posY][posX + j] = 1 - actualSprite;
            }
        }

        i++;
    }

    //Dibujamos al personaje
    posX = (int)((gameState->entidades.jugador.fisica.posx - cameraScrollX)/PIXELSPERUNIT);
    posY = (int)(gameState->entidades.jugador.fisica.posy/PIXELSPERUNIT);

    for (int j = 0; j < ((int) (gameState->entidades.jugador.fisica.alto / PIXELSPERUNIT)); j++) {
        if (INSIDERASPISCREEN(posY + j, posX)) {
            mapLevel[posY + j][posX] = 1;
        }
    }

    writeDisplay(mapLevel);
}

void drawRetryScreen(estadoJuego_t *gameState){


    imprimirNumero(gameState->entidades.jugador.vidas, 0);

    //Ahora imprimo un corazon al lado del numero de vidas
    char retryScreen[16][16] = { {0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0}, //Lives
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

void drawGameOverScreenHighScore(estadoJuego_t* gameState){

    char highScoreTextMenu[16][16] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //Dibujo HighScore
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

}

void drawGameOverScreen(estadoJuego_t* gameState){

    if(wasNewHighScoreAchieved(gameState)) {

        char highScore[16][16] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //Dibujo Score
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

        //AHORA LE AGREGO SIN QUITAR LO ESCRITO ANTERIORMENTE EL PUNTAJE AL QUE LLEGO
        imprimirNumero(gameState->gameUI.score, 1);
        writeDisplay(highScore);
    }
    else{

        char gameOver[16][16] =     {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //Dibujo Game Over
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
        writeDisplay(gameOver);
    }

}

#endif

int initEntities(estadoJuego_t* gameState){

    int blocksCounter = 0;
    int enemiesCounter = 0;
    int blocksIndex = 0;
    int enemiesIndex = 0;
    int horizontalBlocksCounter = 0;
    int currentBlock;

    //Calculamos la cantidad de enemigos y de bloques que hay en el mapa
    for(int i = 0; i < gameState->level.levelHeight; i++){
        for(int j = 0; j < gameState->level.levelWidht; j++) {
            switch(gameState->level.level[i][j]) {
                case LADRILLO:
                case MUSHROOM:
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

                case MUSHROOM:
                    gameState->entidades.bloques[blocksIndex].sprite = 0;
                    gameState->entidades.bloques[blocksIndex].identificador = MUSHROOM;
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
                    gameState->entidades.enemigos[enemiesIndex].estado = SLEPT;
                    gameState->entidades.enemigos[enemiesIndex].identificador = FASTCHEEPCHEEP;
                    gameState->entidades.enemigos[enemiesIndex].fisica.posx = TOWORLDPOS(j);
                    gameState->entidades.enemigos[enemiesIndex].fisica.posy = TOWORLDPOS(i);
                    gameState->entidades.enemigos[enemiesIndex].fisica.ancho = PIXELSPERUNIT;
                    gameState->entidades.enemigos[enemiesIndex].fisica.alto = PIXELSPERUNIT;
                    gameState->entidades.enemigos[enemiesIndex].fisica.velx = 0 ;             //Le puse una velocidad al cheep cheep para la izquierda
                    gameState->entidades.enemigos[enemiesIndex].fisica.vely = 0;
                    gameState->entidades.enemigos[enemiesIndex].funcionMovimiento = cheepcheep;
                    gameState->entidades.enemigos[enemiesIndex].moveAgain = 1;
                    enemiesIndex++;
                    break;

                case SLOWCHEEPCHEEP:
                    gameState->entidades.enemigos[enemiesIndex].sprite = 0;
                    gameState->entidades.enemigos[enemiesIndex].estado = SLEPT;
                    gameState->entidades.enemigos[enemiesIndex].identificador = SLOWCHEEPCHEEP;
                    gameState->entidades.enemigos[enemiesIndex].fisica.posx = TOWORLDPOS(j);
                    gameState->entidades.enemigos[enemiesIndex].fisica.posy = TOWORLDPOS(i);
                    gameState->entidades.enemigos[enemiesIndex].fisica.ancho = PIXELSPERUNIT;
                    gameState->entidades.enemigos[enemiesIndex].fisica.alto = PIXELSPERUNIT;
                    gameState->entidades.enemigos[enemiesIndex].fisica.velx = 0 ;             //Le puse una velocidad al cheep cheep para la izquierda
                    gameState->entidades.enemigos[enemiesIndex].fisica.vely = 0;
                    gameState->entidades.enemigos[enemiesIndex].funcionMovimiento = cheepcheep;
                    gameState->entidades.enemigos[enemiesIndex].moveAgain = 1;
                    enemiesIndex++;
                    break;

                case PULPITO:
                    gameState->entidades.enemigos[enemiesIndex].sprite = 0;
                    gameState->entidades.enemigos[enemiesIndex].estado = SLEPT;
                    gameState->entidades.enemigos[enemiesIndex].identificador = PULPITO;
                    gameState->entidades.enemigos[enemiesIndex].fisica.posx = TOWORLDPOS(j);
                    gameState->entidades.enemigos[enemiesIndex].fisica.posy = TOWORLDPOS(i);
                    gameState->entidades.enemigos[enemiesIndex].fisica.ancho = PIXELSPERUNIT;
                    gameState->entidades.enemigos[enemiesIndex].fisica.alto = PIXELSPERUNIT * 2 ;
                    gameState->entidades.enemigos[enemiesIndex].fisica.velx = 0;             //Le puse una velocidad al blooper para la izquierda
                    gameState->entidades.enemigos[enemiesIndex].fisica.vely = 0;
                    gameState->entidades.enemigos[enemiesIndex].funcionMovimiento = blooper;
                    gameState->entidades.enemigos[enemiesIndex].moveAgain = 1;
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
                default:
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

int cargarMapa(level_t* level, int id) {

    FILE *mapData;
    int i;
    int j = 0;
    int read;
    int auxCont = 0;
    int borderCount = 0;

    int error = openLevelData(&mapData, id-1);

    if (error != 1){
        countColumns(level, mapData);   //Contamos las columnas del mapa
        //Reservamos la memoria para cargarlo
        level->level = (int **) calloc( level->levelHeight, sizeof(int *));
        for (i = 0; i < level->levelHeight; i++) {
            (level->level)[i] = (int*) malloc(level->levelWidht * sizeof(int));
        }

        i = 0;

        //Ahora guardamos en cada una de las posiciones reservados el correspondiente identificador de cada bloque
        do {
            read = fgetc(mapData);

            switch (read) {
                case MONEDA:
                case MUSHROOM:
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

int wasNewHighScoreAchieved(estadoJuego_t* gameState){

    int newHighScore = 0;

    for (int i = 0; i < gameState->maxTopScoreEntries && !newHighScore; i++) {
        if (gameState->bestScores[i] < gameState->gameUI.score) {
            newHighScore = 1;
        }
    }

    return newHighScore;
}

void saveNewHighScore(estadoJuego_t* gameState){

    int newHighScorePos = -1;
    FILE* scoreFileData;

    if(openGameStateFile(&scoreFileData) == 1){
        printf("Error al cargar el archivo de estadojuego para guardar el nuevo highscore\n");
    }
    else {  //Busco la posicion del nuevo highscore
        for (int i = 0; i < gameState->maxTopScoreEntries && newHighScorePos == -1; i++) {
            if (gameState->bestScores[i] < gameState->gameUI.score) {
                newHighScorePos = i;
            }
        }
            //Reemplazo el nombre viejo por el del nuevo highscore, muevo los que ya no son del top
        for (int i = gameState->maxTopScoreEntries - 1; newHighScorePos < i; i--) {
            gameState->bestScores[i] = gameState->bestScores[i - 1];
            strcpy(gameState->bestScoresName[i], gameState->bestScoresName[i - 1]);
        }

        //Guardo el nuevo highscore
        gameState->bestScores[newHighScorePos] = gameState->gameUI.score;
        strcpy(gameState->bestScoresName[newHighScorePos], gameState->pPlayerName);

        //Guardo todos los highscores en el .txt
        fprintf(scoreFileData, "%d\n", gameState->maxTopScoreEntries);
        for (int i = 0; i < gameState->maxTopScoreEntries; i++) {
            fprintf(scoreFileData, "%d %s\n", gameState->bestScores[i], gameState->bestScoresName[i]);
        }

        fprintf(scoreFileData, "\n%s\n%s\n", "//Cantidad de valores", "//Lista de puntaje - nombre");
        fflush(scoreFileData);
        fclose(scoreFileData);
    }
}

void initUI(gameUI_t* gameUI){
    gameUI->time = MAXLEVELTIME;
    gameUI->score = 0;
    gameUI->coins = 0;
    gameUI->level = 1;
}

void resetEntitiesState(estadoJuego_t* gameState){

    //Utilizo la estructura que cargamos en defaultEntities con initBackupEntities para reiniciar el nivel

    for(int i = 0; gameState->entidades.enemigos[i].identificador != NULLENTITIE; i++){
        gameState->entidades.enemigos[i].sprite = gameState->defaultEntities.enemigos[i].sprite;
        gameState->entidades.enemigos[i].estado = gameState->defaultEntities.enemigos[i].estado;
        gameState->entidades.enemigos[i].fisica.posx = gameState->defaultEntities.enemigos[i].fisica.posx;
        gameState->entidades.enemigos[i].fisica.posy = gameState->defaultEntities.enemigos[i].fisica.posy;
    }

    for(int i = 0; gameState->entidades.bloques[i].identificador != NULLENTITIE; i++){
        gameState->entidades.bloques[i].sprite = gameState->defaultEntities.bloques[i].sprite;
        gameState->entidades.bloques[i].fisica = gameState->defaultEntities.bloques[i].fisica;
    }

    gameState->entidades.jugador.sobreBloque = gameState->defaultEntities.jugador.sobreBloque;
    gameState->entidades.jugador.estado = gameState->defaultEntities.jugador.estado;
    gameState->entidades.jugador.sprite = gameState->defaultEntities.jugador.sprite;
    gameState->entidades.jugador.fisica = gameState->defaultEntities.jugador.fisica;
    gameState->entidades.jugador.isMoving = gameState->defaultEntities.jugador.isMoving;
}

void destroyEntities(estadoJuego_t * gameState){
    free(gameState->entidades.bloques);
    free(gameState->entidades.enemigos);
    free(gameState->defaultEntities.bloques);
    free(gameState->defaultEntities.enemigos);
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

#if MODOJUEGO == ALEGRO

/*Dibuja el score, el timer, las vidas y las monedas a la vez que dibuja el nivel*/
static void drawGameUI(estadoJuego_t *gameState){

    static int lastLivesQuant = MAXLIVES;
    static int lifeUpTextEnabled = 0;
    char auxToString[10];
    bufferRecursos_t *resourceBuffer = &gameState->buffer;

    if(lastLivesQuant < gameState->entidades.jugador.vidas && gameState->gameUI.score != 0){
        startTimer(LIFEUPANIM);
        lifeUpTextEnabled = 1;
        playSoundFromMemory(gameState->buffer.sound[ONEUP], gameState->buffer.sound[ONEUP]->volume);
    }
    lastLivesQuant = gameState->entidades.jugador.vidas;

    if(lifeUpTextEnabled) {
        if(isPaused(LIFEUPANIM)) {
            lifeUpTextEnabled = 0;
        }
        else {
            al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], UICOLOR, gameState->entidades.jugador.fisica.posx - 40 - getCameraScrollX(), gameState->entidades.jugador.fisica.posy - 80, 0, "LIFE UP !");
        }
    }

    //score
    sprintf(auxToString, "%d", gameState->gameUI.score);
    al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], UICOLOR, 120, 0, 0, "matias");
    al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], UICOLOR, 120, 50, 0, auxToString);

    //coins
    sprintf(auxToString, "%d", gameState->gameUI.coins);
    al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], UICOLOR, 350, 30, 0, " x ");
    al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], UICOLOR, 400, 30, 0, auxToString);
    al_draw_scaled_bitmap(resourceBuffer->image[COINSPRITE1], 0, 0, (float)al_get_bitmap_width(resourceBuffer->image[COINSPRITE1]), (float)al_get_bitmap_height(resourceBuffer->image[COINSPRITE1]), 315, 38,
                          (float)al_get_bitmap_width(resourceBuffer->image[COINSPRITE1]) * 3.0f, (float)al_get_bitmap_height(resourceBuffer->image[COINSPRITE1]) * 3.0f, 0);

    //level
    sprintf(auxToString, "%d", gameState->gameUI.level);
    al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], UICOLOR, 800, 30, 0, "level");
    al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], UICOLOR, 900, 30, 0, auxToString);


    //timer
    sprintf(auxToString, "%d", gameState->gameUI.time);
    if(gameState->gameUI.time <= HURRYUPTIME) {
        al_draw_text(gameState->buffer.font[SUPERMARIOFONT80], UIWARNINGCOLOR, 1020, 15, 0, "time");
        al_draw_text(gameState->buffer.font[SUPERMARIOFONT80], UIWARNINGCOLOR, 1120, 15, 0, auxToString);
    }
    else{
        al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], UICOLOR, 1000, 30, 0, "time");
        al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], UICOLOR, 1100, 30, 0, auxToString);
    }

}

#endif

/* Cuenta las columnas de un nivel para conocer el ancho del mismo */
static int countColumns(level_t* level, FILE* mapData){

    int error = 0;
    int colNum = 0;
    int read;
    int borderCount = 0;
    int auxCont = 0;

    do {
        read = fgetc(mapData);

        switch (read) {
            case FASTCHEEPCHEEP:
            case SLOWCHEEPCHEEP:
            case PULPITO:
            case MONEDA:
            case MUSHROOM:
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

/* Hago un backup de la salida de initEntities asi no tengo que ejecutarla de vuelta al morir*/
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
