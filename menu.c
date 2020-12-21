//
// Created by gonzalo on 23/11/20.
//

#include "menu.h"
#include "IEvents.h"

void updateMenuArrow (int* seleccion, char evento){

    if(evento == DOWNARRIBA){
        if(*seleccion <= LEVELSELECTOR) {
            *seleccion = LEVELSELECTOR;
        }
        else{
            (*seleccion)--;
        }
    }
    else if(evento == DOWNABAJO){
        if(*seleccion >= SCORETABLE) {
            *seleccion = SCORETABLE;
        }
        else{
            (*seleccion)++;
        }
    }
}


#if MODOJUEGO == 0

#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include "IEvents.h"

#define MOVEARROW(flecha) ((flecha == 1) ? 0 : (flecha * PIXELSPERUNIT) )

typedef struct{
    float x;
    float y;
    float scale;
}image_menu_t;

typedef struct{
    int r;
    int g;
    int b;
    float x;
    float y;
    char word[MAXMENUWORDSIZE];
}text_menu_t;

typedef struct{
    int imgQuant;
    int textQuant;
    image_menu_t *imgMenu;
    text_menu_t *textMenu;
}menu_t;

static menu_t menu;

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

void drawLevelSelector(estadoJuego_t* gameState){

}

int drawMenu(estadoJuego_t *gameState) {

    int salida = 0;

    //Si no hubo error al cargar la data del menu, lo dibujamos
    if(salida == 0){
        for(int i = FONDOMENU; i <= FLECHAMENU; i++){
            image_t currentImg = gameState->buffer.image[i];
            int arrowPosition = 1;

            //Si es la image de la flecha
            if(i == FLECHAMENU){
                arrowPosition = gameState->menuSelection;
            }

            al_draw_scaled_bitmap(currentImg, 0, 0, al_get_bitmap_width(currentImg), al_get_bitmap_height(currentImg),
                                  menu.imgMenu[i].x, menu.imgMenu[i].y + MOVEARROW(arrowPosition), al_get_bitmap_width(currentImg) * menu.imgMenu[i].scale,
                                  al_get_bitmap_height(currentImg) * menu.imgMenu[i].scale, 0);
        }

        for(int i = SELECTTEXT; i <= SCORETEXT; i++){
            al_draw_text(gameState->buffer.font[SUPERMARIOFONT80], al_map_rgb(menu.textMenu[i].r, menu.textMenu[i].g, menu.textMenu[i].b), menu.textMenu[i].x, menu.textMenu[i].y, 0, menu.textMenu[i].word);
        }

        al_flip_display();
    }

    return salida;
}

void destroyMenu(){
    free(menu.imgMenu);
    free(menu.textMenu);
}

void drawTopScores(estadoJuego_t * gameState, bufferRecursos_t *buffer){

    al_clear_to_color(al_map_rgb(76, 93, 122));
    float offsetY = 225;
    char intToString [MAXCIFRASSCORE] =  {0};

    image_t scoreTable = buffer->image[SCORETABLEIMG];
    al_draw_scaled_bitmap(scoreTable, 0, 0, al_get_bitmap_width(scoreTable), al_get_bitmap_height(scoreTable),
                          menu.imgMenu[SCORETABLEIMG].x, menu.imgMenu[SCORETABLEIMG].y, al_get_bitmap_width(scoreTable) * menu.imgMenu[SCORETABLEIMG].scale,
                          al_get_bitmap_height(scoreTable) * menu.imgMenu[SCORETABLEIMG].scale, 0);

    for(int i = 0; i < gameState->maxEntries; i++){

        sprintf(intToString, "%d", gameState->bestScores[i]);
        al_draw_text(buffer->font[SUPERMARIOFONT50], al_map_rgb(57, 16, 84), 540, offsetY, 0, intToString);
        al_draw_text(buffer->font[SUPERMARIOFONT50], al_map_rgb(57, 16, 84), 640, offsetY, 0, gameState->bestScoresName[i]);
        offsetY += 65;
    }

    al_flip_display();
}

#endif

#if MODOJUEGO == 1

void imprimirHighScore (int numero) {

    char raspihighscore [16][16] = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ,
                                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ,
                                {1,1,0,1,1,0,1,1,1,0,1,1,1,0,1,1} ,
                                {1,0,0,1,0,0,1,0,1,0,1,0,1,0,1,0} ,
                                {1,1,0,1,0,0,1,0,1,0,1,1,0,0,1,1} ,
                                {0,1,0,1,0,0,1,0,1,0,1,0,1,0,1,0} ,
                                {1,1,0,1,1,0,1,1,1,0,1,0,1,0,1,1} ,
                                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ,
                                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ,
                                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ,
                                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ,
                                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ,
                                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ,
                                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ,
                                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ,
                                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
                                };


    const char matrices_num [10][5][3] ={{  {0,1,0},
                                        {1,0,1},
                                        {1,0,1},
                                        {1,0,1},
                                        {0,1,0}
                                    },

                                    {   {0,0,1},
                                        {0,1,1},
                                        {1,0,1},
                                        {0,0,1},
                                        {0,0,1}
                                    },

                                    {   {0,1,0},
                                        {1,0,1},
                                        {0,0,1},
                                        {0,1,0},
                                        {1,1,1}
                                    },

                                    {   {1,1,0},
                                        {0,0,1},
                                        {0,1,0},
                                        {0,0,1},
                                        {1,1,0}
                                    },

                                    {   {1,0,1},
                                        {1,0,1},
                                        {1,1,1},
                                        {0,0,1},
                                        {0,0,1}
                                    },

                                    {   {1,1,1},
                                        {1,0,0},
                                        {1,1,0},
                                        {0,0,1},
                                        {1,1,0}
                                    },

                                    {   {0,1,0},
                                        {1,0,1},
                                        {1,1,0},
                                        {1,0,1},
                                        {0,1,0}
                                    },

                                    {   {1,1,1},
                                        {0,0,1},
                                        {0,1,1},
                                        {0,1,0},
                                        {1,0,0}
                                    },

                                    {   {0,1,0},
                                        {1,0,1},
                                        {0,1,0},
                                        {1,0,1},
                                        {0,1,0}
                                    },

                                    {   {0,1,0},
                                        {1,0,1},
                                        {0,1,1},
                                        {0,0,1},
                                        {0,1,0}
                                    }
                                    };

    const char pos_iniciales [4][2] = {{9,1},{9,5},{9,9},{9,13}};

    int i=0, j=0, cont1=0, cont2=0, x=0, y=0;
    int digitos [MAXCIFRASSCORE] = {0,0,0,0};
    //int correccion=0;

    digitos[0]=(int)(numero/1000);
    digitos[1]=(int)((numero-digitos[0]*1000)/100);
    digitos[2]=(int)((numero-digitos[0]*1000-digitos[1]*100)/10);
    digitos[3]=(int)(numero-digitos[0]*1000-digitos[1]*100-digitos[2]*10);

    /*
    for(cont2=0;cont2<=3;cont2++) {
        if(digitos[cont2]==1)
            correccion++;
    }*/
    
    for(cont1=0;cont1<MAXCIFRASSCORE;cont1++) {
        for(j=0;j<=2;j++) {
            for(i=0;i<=4;i++) {
                raspihighscore[pos_iniciales[cont1][0]+i][pos_iniciales[cont1][1]+j]=matrices_num[digitos[cont1]][i][j];
            }
        }
    }

    actualizarDisplay(raspihighscore);
}

int drawMenu(estadoJuego_t *gameState) {

    int salida = 0;

    if (gameState->menuSelection == LEVELSELECTOR ){

        char raspiscore [16][16] = {    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} , // He aqui el menu principal de la raspi hardcodeado
                                        {1,0,0,1,1,0,1,0,1,0,1,1,0,1,0,0} , //ACA DICE LEVEL
                                        {1,0,0,1,0,0,1,0,1,0,1,0,0,1,0,0} ,
                                        {1,0,0,1,1,0,1,0,1,0,1,1,0,1,0,0} ,
                                        {1,0,0,1,0,0,1,0,1,0,1,0,0,1,0,0} ,
                                        {1,1,0,1,1,0,0,1,0,0,1,1,0,1,1,0} ,
                                        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ,
                                        {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0} ,
                                        {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1} ,
                                        {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0} ,
                                        {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1} ,
                                        {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0} ,
                                        {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1} ,
                                        {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0} ,
                                        {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1} ,
                                        {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0} ,
        };

        actualizarDisplay(raspiscore);

    }
    else if (gameState->menuSelection == SCORETABLE ){


        char raspiscore [16][16] = {    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} , // He aqui el menu principal de la raspi hardcodeado
                                        {1,0,1,0,1,1,1,0,1,1,1,0,0,1,0,1} , //ACA DICE HIGH SCORE
                                        {1,0,1,0,0,1,0,0,1,0,0,0,0,1,0,1} ,
                                        {1,1,1,0,0,1,0,0,1,0,1,1,0,1,1,1} ,
                                        {1,0,1,0,0,1,0,0,1,0,0,1,0,1,0,1} ,
                                        {1,0,1,0,1,1,1,0,1,1,1,1,0,1,0,1} ,
                                        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ,
                                        {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0} ,
                                        {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1} ,
                                        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ,
                                        {1,1,0,1,1,0,1,1,1,0,1,1,1,0,1,1} ,
                                        {1,0,0,1,0,0,1,0,1,0,1,0,1,0,1,0} ,
                                        {1,1,0,1,0,0,1,0,1,0,1,1,0,0,1,1} ,
                                        {0,1,0,1,0,0,1,0,1,0,1,0,1,0,1,0} ,
                                        {1,1,0,1,1,0,1,1,1,0,1,0,1,0,1,1} ,
                                        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ,
        };

        actualizarDisplay(raspiscore);
    }

    return salida;
}


#endif
