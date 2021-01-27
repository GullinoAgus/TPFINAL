/***************************************************************************//**
  file     menu.c
  @brief    Funciones relacionadas al dibujado y manejo del menu
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "menu.h"
#include "IEvents.h"
#include "render.h"

#if MODOJUEGO == ALLEGRO

#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>

#elif MODOJUEGO == RASPI

#include "raspi.h"

#endif

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#if MODOJUEGO == ALLEGRO

#define MOVEARROW(flecha) ((flecha == 1) ? 0 : ((flecha-1) * PIXELSPERUNIT * 2) )
#define UIMENUCOLOR al_map_rgb(57, 16, 84)

#endif

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

#if MODOJUEGO == ALLEGRO

typedef struct{
    int r;
    int g;
    int b;
    float x;
    float y;
    char word[MAXMENUWORDSIZE];
}text_menu_t;

typedef struct{
    float x;
    float y;
    float scale;
}image_menu_t;

typedef struct{
    int imgQuant;
    int textQuant;
    image_menu_t *imgMenu;
    text_menu_t *textMenu;
}menu_t;

#endif

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

#if MODOJUEGO == ALLEGRO

static menu_t menu;

#endif

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

#if MODOJUEGO == ALLEGRO

void drawLevelSelector(estadoJuego_t* gameState){

    char auxToString[10];
    static int lastPosition = 1;
    image_t currentImg;

    if(lastPosition < gameState->gameUI.level){
        currentImg = gameState->buffer.image[LEVELSELECTORRIGHT];
        lastPosition = gameState->gameUI.level;
    }
    else if(lastPosition > gameState->gameUI.level){
        currentImg = gameState->buffer.image[LEVELSELECTORLEFT];
        lastPosition = gameState->gameUI.level;
    }
    else{
        currentImg = gameState->buffer.image[LEVELSELECTORIDLE];
    }

    al_draw_scaled_bitmap(currentImg, 0, 0, (float)al_get_bitmap_width(currentImg), (float)al_get_bitmap_height(currentImg),
                          520, 370, (float)al_get_bitmap_width(currentImg) * 4,
                          (float)al_get_bitmap_height(currentImg) * 4, 0);
    usleep(50000);

    sprintf(auxToString, "%d", gameState->gameUI.level);
    al_draw_text(gameState->buffer.font[SUPERMARIOFONT80], UIMENUCOLOR, 637, 396, 0, auxToString);

    al_flip_display();
}

void drawMenu(estadoJuego_t *gameState) {

    //Si no hubo error al cargar la data del menu, lo dibujamos
    for(int i = FONDOMENU; i <= FLECHAMENU; i++){
        image_t currentImg = gameState->buffer.image[i];
        int arrowPosition = 1;

        //Si es la image de la flecha
        if(i == FLECHAMENU){
            arrowPosition = gameState->menuSelection;
        }

        al_draw_scaled_bitmap(currentImg, 0, 0, (float)al_get_bitmap_width(currentImg), (float)al_get_bitmap_height(currentImg),
                              menu.imgMenu[i].x, menu.imgMenu[i].y + MOVEARROW(arrowPosition), (float)al_get_bitmap_width(currentImg) * menu.imgMenu[i].scale,
                              (float)al_get_bitmap_height(currentImg) * menu.imgMenu[i].scale, 0);
    }

    for(int i = SELECTTEXT; i <= EXITTEXT; i++){
        al_draw_text(gameState->buffer.font[SUPERMARIOFONT80], al_map_rgb(menu.textMenu[i].r, menu.textMenu[i].g, menu.textMenu[i].b), menu.textMenu[i].x, menu.textMenu[i].y, 0, menu.textMenu[i].word);
    }

    al_flip_display();
}

void drawTopScores(estadoJuego_t * gameState){

    al_clear_to_color(al_map_rgb(96,118,153));
    float offsetY = 225;
    char intToString [MAXCIFRASSCORE] =  {0};

    image_t scoreTable = gameState->buffer.image[SCORETABLEIMG];
    al_draw_scaled_bitmap(scoreTable, 0, 0, (float)al_get_bitmap_width(scoreTable), (float)al_get_bitmap_height(scoreTable),
                          menu.imgMenu[SCORETABLEIMG].x, menu.imgMenu[SCORETABLEIMG].y, (float)al_get_bitmap_width(scoreTable) * menu.imgMenu[SCORETABLEIMG].scale,
                          (float)al_get_bitmap_height(scoreTable) * menu.imgMenu[SCORETABLEIMG].scale, 0);

    for(int i = 0; i < gameState->maxTopScoreEntries; i++){

        sprintf(intToString, "%d", gameState->bestScores[i]);
        al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], UIMENUCOLOR, 540, offsetY, 0, intToString);
        al_draw_text(gameState->buffer.font[SUPERMARIOFONT60], UIMENUCOLOR, 640, offsetY, 0, gameState->bestScoresName[i]);
        offsetY += 65;
    }

    al_flip_display();
}

int loadMenuData(){

    FILE *imgMenuData;
    FILE *textMenuData;
    int cantDeElementos = 0;
    int error = 0;
    int i;

    if(openMenuData(&imgMenuData, &textMenuData) == 1){
        error = 1;
    }
    else {

        //Cargamos la informacion de las imagenes
        fscanf(imgMenuData, "%d", &cantDeElementos);
        menu.imgQuant = cantDeElementos;
        menu.imgMenu = (image_menu_t *) malloc(sizeof(image_menu_t) * (cantDeElementos));
        if (menu.imgMenu == NULL) {
            error = 1;
        } else {
            for (i = 0; i < cantDeElementos; i++) {
                fscanf(imgMenuData, "%f %f %f", &menu.imgMenu[i].x, &menu.imgMenu[i].y, &menu.imgMenu[i].scale);
            }

            //Cargamos la informacion de los textos
            fscanf(textMenuData, "%d", &cantDeElementos);
            menu.textQuant = cantDeElementos;
            menu.textMenu = (text_menu_t *) malloc(sizeof(text_menu_t) * (cantDeElementos));
            if (menu.textMenu == NULL) {
                error = 1;
            } else {
                for (i = 0; i < cantDeElementos; i++) {
                    fscanf(textMenuData, "%d %d %d %f %f %s", &menu.textMenu[i].r, &menu.textMenu[i].g, &menu.textMenu[i].b,
                           &menu.textMenu[i].x, &menu.textMenu[i].y, menu.textMenu[i].word);
                }
            }
        }
    }

    fclose(imgMenuData);
    fclose(textMenuData);

    return error;
}

void destroyMenu(){
    free(menu.imgMenu);
    free(menu.textMenu);
}

#elif MODOJUEGO == RASPI

int loadMenuData(void){
    //NO HAGO NADA, me sirve para mantener gamelogic como esta.
    //Tambien podria poner la compilacion condicional en gamelogic
    return 0;
}

void drawMenu(estadoJuego_t *gameState) {

    switch(gameState->menuSelection){

        case LEVELSELECTOR: {
            char playTextMenu[16][16] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //Play selection
                                         {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                         {0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0},
                                         {0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0},
                                         {0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0},
                                         {0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0},
                                         {0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0},
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
            writeDisplay(playTextMenu);
            break;
        }


        case SCORETABLE: {
            char highScoreTextMenu[16][16] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                              {1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1},
                                              {1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1},
                                              {1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1},
                                              {1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1},
                                              {1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1},
                                              {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                              {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
                                              {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
                                              {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                              {1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1},
                                              {1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
                                              {1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1},
                                              {0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
                                              {1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1},
                                              {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
            };
            writeDisplay(highScoreTextMenu);
            break;
        }

        case EXITGAME: {

            char exitGameTextMenu[16][16] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                             {0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0},
                                             {0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0},
                                             {0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0},
                                             {0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0},
                                             {0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0},
                                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                             {0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1},
                                             {1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0},
                                             {1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1},
                                             {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0},
                                             {0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1},
                                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
            };

            writeDisplay(exitGameTextMenu);
            break;
        }
    }
}

void drawLevelSelector(estadoJuego_t* gameState){

    imprimirNumero (gameState->gameUI.level, 1);

    //AHORA LE AGREGO SIN QUITAR LOS NUMEROS ESCRITOS ANTERIORMENTE, "LEVEL"

    char levelSelected[16][16] ={{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //level Cleared
                                 {0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0},
                                 {0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0},
                                 {0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0},
                                 {0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0},
                                 {0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1},
                                 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                 {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                                 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                 {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
                                 {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
                                 {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
                                 {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
                                 {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
                                 {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
                                 {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}
    };
    writeDisplay(levelSelected);

}

void drawTopScores(estadoJuego_t * gameState){

    imprimirNumero(gameState->bestScores[0], 1);

    //AHORA LE AGREGO SIN QUITAR LOS NUMEROS ESCRITOS ANTERIORMENTE, "SCORE"

    char Highscore[16][16] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
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

    writeDisplay(Highscore);

}

void imprimirNumero (int numero, int zerosEnabled) {

    char numberAtBottom[16][16];

    if(zerosEnabled == 1) {
        numberAtBottom[16][16] = { {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
                                   {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
                                   {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
                                   {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
                                   {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
                                   {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
                                   {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
                                   {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
                                   {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
                                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
        };
    }
    else{
        numberAtBottom[16][16] = { {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
                                   {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
                                   {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
                                   {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
                                   {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
                                   {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
                                   {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
                                   {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
                                   {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
                                   {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0},
                                   {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0},
                                   {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0},
                                   {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0},
                                   {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0},
                                   {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0},
                                   {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0}
        };
    }

    const char matrices_num [10][5][3] ={{       {0,1,0},
                                                 {1,0,1},
                                                 {1,0,1},
                                                 {1,0,1},
                                                 {0,1,0}
                                         },

                                         {       {0,0,1},
                                                 {0,1,1},
                                                 {1,0,1},
                                                 {0,0,1},
                                                 {0,0,1}
                                         },

                                         {       {0,1,0},
                                                 {1,0,1},
                                                 {0,0,1},
                                                 {0,1,0},
                                                 {1,1,1}
                                         },

                                         {       {1,1,0},
                                                 {0,0,1},
                                                 {0,1,0},
                                                 {0,0,1},
                                                 {1,1,0}
                                         },

                                         {       {1,0,1},
                                                 {1,0,1},
                                                 {1,1,1},
                                                 {0,0,1},
                                                 {0,0,1}
                                         },

                                         {       {1,1,1},
                                                 {1,0,0},
                                                 {1,1,0},
                                                 {0,0,1},
                                                 {1,1,0}
                                         },

                                         {       {0,1,0},
                                                 {1,0,0},
                                                 {1,1,0},
                                                 {1,0,1},
                                                 {0,1,0}
                                         },

                                         {       {1,1,1},
                                                 {0,0,1},
                                                 {0,1,1},
                                                 {0,1,0},
                                                 {1,0,0}
                                         },

                                         {       {0,1,0},
                                                 {1,0,1},
                                                 {0,1,0},
                                                 {1,0,1},
                                                 {0,1,0}
                                         },

                                         {       {0,1,0},
                                                 {1,0,1},
                                                 {0,1,1},
                                                 {0,0,1},
                                                 {0,1,0}
                                         }
    };

    const char pos_iniciales [4][2] = {{9,1},{9,5},{9,9},{9,13}};

    int i, j, cont1;
    int digitos [MAXCANTDIGPUNTAJE] = {0,0,0,0};

    digitos[0]=(int)(numero/1000);
    digitos[1]=(int)((numero-digitos[0]*1000)/100);
    digitos[2]=(int)((numero-digitos[0]*1000-digitos[1]*100)/10);
    digitos[3]=(int)(numero-digitos[0]*1000-digitos[1]*100-digitos[2]*10);

   if(zerosEnabled == 1){
       for (cont1 = 0; cont1 < MAXCANTDIGPUNTAJE; cont1++) {
           for (j = 0; j <= 2; j++) {
               for (i = 0; i <= 4; i++) {
                   numberAtBottom[pos_iniciales[cont1][0] + i][pos_iniciales[cont1][1] +
                                                               j] = matrices_num[digitos[cont1]][i][j];
               }
           }
       }
   }
   else{
       for (j = 0; j <= 2; j++) {
           for (i = 0; i <= 4; i++) {
               numberAtBottom[pos_iniciales[3][0] + i][pos_iniciales[3][1]+j] = matrices_num[digitos[3]][i][j];
           }
       }
   }

   writeDisplay(numberAtBottom);
}

#endif

void updateMenuArrow (int* seleccion, unsigned char evento){

    if(evento == DOWNARRIBA){
        if(*seleccion <= LEVELSELECTOR) {
            *seleccion = LEVELSELECTOR;
        }
        else{
            (*seleccion)--;
        }
    }
    else if(evento == DOWNABAJO){
        if(*seleccion >= EXITGAME) {
            *seleccion = EXITGAME;
        }
        else{
            (*seleccion)++;
        }
    }
}

void updatePauseArrow (int* seleccion, unsigned char evento){

    if(evento == DOWNARRIBA){
        if(*seleccion <= RESUME) {
            *seleccion = RESUME;
        }
        else{
            (*seleccion)--;
        }
    }
    else if(evento == DOWNABAJO){
        if(*seleccion >= BACKTOMENU) {
            *seleccion = BACKTOMENU;
        }
        else{
            (*seleccion)++;
        }
    }
}
