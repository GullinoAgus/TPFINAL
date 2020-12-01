//
// Created by gonzalo on 23/11/20.
//

#include "menu.h"

#if MODOJUEGO == 0

#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include "IEvents.h"

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

static void drawTopScores(estadoJuego_t *gameState, bufferRecursos_t *buffer);

static int loadMenuData(){

    FILE *imgMenuData;
    FILE *textMenuData;
    int cantDeElementos = 0;
    int error = 0;
    int i;

    if(openMenuData(&imgMenuData, &textMenuData) == 1){
        error = 1;
    }
    else{

        //Cargamos la informacion de las imagenes
        fscanf(imgMenuData, "%d", &cantDeElementos);
        menu.imgQuant = cantDeElementos;
        menu.imgMenu = (image_menu_t *) malloc(sizeof(image_menu_t) * (cantDeElementos));
        if(menu.imgMenu == NULL){
            error = 1;
        }
        else {
            for (i = 0; i < cantDeElementos; i++) {
                fscanf(imgMenuData, "%f %f %f", &menu.imgMenu[i].x, &menu.imgMenu[i].y, &menu.imgMenu[i].scale);
        }

        //Cargamos la informacion de los textos
        fscanf(textMenuData, "%d", &cantDeElementos);
        menu.textQuant = cantDeElementos;
        menu.textMenu = (text_menu_t *) malloc(sizeof(text_menu_t) * (cantDeElementos));
            if(menu.textMenu == NULL){
                error = 1;
            }
            else {
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

void updateMenu (int* seleccion, char evento){

    if (evento == DOWNABAJO) {
        (*seleccion)++;
    }
    else if (evento == DOWNARRIBA){
        (*seleccion)--;
    }

    //Verifico que no se pase la flecha
    if((*seleccion) < 0){
        (*seleccion) = 0;
    }

    if((*seleccion) > 1){
        (*seleccion) = 1;
    }
}

int drawMenu(bufferRecursos_t *buffer) {

    al_clear_to_color(al_map_rgb(0, 0, 0));

    if(loadMenuData() == 1){
        return 1;
    }
    else{
        for(int i = 0; i < menu.imgQuant; i++){
            image_t currentImg = (buffer->image)[i];
            al_draw_scaled_bitmap(currentImg, 0, 0, al_get_bitmap_width(currentImg), al_get_bitmap_height(currentImg),
                                  menu.imgMenu[i].x, menu.imgMenu[i].y,al_get_bitmap_width(currentImg) * menu.imgMenu[i].scale,al_get_bitmap_height(currentImg) * menu.imgMenu[i].scale, 0);
        }

        for(int i = 0; i < menu.textQuant; i++){
            al_draw_text(buffer->font[0], al_map_rgb(menu.textMenu[i].r, menu.textMenu[i].g, menu.textMenu[i].b), menu.textMenu[i].x, menu.textMenu[i].y, 0, menu.textMenu[i].word);
        }
    }

    al_flip_display();
    return 0;
}

void destroyMenu(){
    free(menu.imgMenu);
    free(menu.textMenu);
}


int verTopScores(estadoJuego_t * gameState, bufferRecursos_t *buffer){

    int adondevamos;
    char eventoActual = 0;
    int exit_menu = 0;
    drawTopScores(gameState, buffer);

    while (!exit_menu){

        while ( esBufferVacio() == 1 ); //SE PODRIA REHACER CON SEMAFOROS, por ahora esto funciona
        eventoActual = getInputEvent();

            if (eventoActual == DOWNBOTON){

                exit_menu = 1;
                adondevamos = 0;
            }
        }

    return adondevamos;
}

static void drawTopScores(estadoJuego_t * gameState, bufferRecursos_t *buffer){

    al_clear_to_color(al_map_rgb(255, 255, 0));
    float posY = -70;
    char intToString [MAXCIFRASSCORE] =  {0};

    for(int i = 0; i < gameState->maxEntries; i++){

        sprintf(intToString, "%d", gameState->bestScores[i]);
        al_draw_text(buffer->font[0], al_map_rgb(0, 0, 0), 100, posY+=70, 0, intToString);
        al_draw_text(buffer->font[0], al_map_rgb(0, 0, 0), 200, posY, 0, gameState->bestScoresName[i]);
    }

    al_flip_display();
}

#endif

#if MODOJUEGO == 1

#include "IEvents.h"

int actualizarMenu (void){
    int adondevamos = 0;  //adonde vamos es igual a 1 si empezamos el juego e igual a 2 si vamos a ver la tabla de puntajes
    char eventoActual = 0;
    int exit_menu = 0;

    char raspimenu [16][16] = {     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} , // He aqui el menu principal de la raspi hardcodeado
                                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ,
                                    {0,1,1,1,0,1,0,0,1,1,1,0,1,0,1,0} ,
                                    {0,1,0,1,0,1,0,0,1,0,1,0,1,0,1,0} ,
                                    {0,1,1,1,0,1,0,0,1,1,1,0,1,1,1,0} ,
                                    {0,1,0,0,0,1,0,0,1,0,1,0,0,1,0,0} ,
                                    {0,1,0,0,0,1,1,0,1,0,1,0,0,1,0,0} ,
                                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ,
                                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ,
                                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ,
                                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ,
                                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ,
                                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ,
                                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ,
                                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ,
                                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ,
                };

    actualizarDisplay(raspimenu);

    while (!exit_menu){

        while ( esBufferVacio() == 1 ); //SE PODRIA REHACER CON SEMAFOROS, por ahora esto funciona
        eventoActual = getInputEvent();

        if (eventoActual == DOWNIZQUIERDA){

            //Podriamos movernos hacia otras opciones del menu
        }
        else if (eventoActual == DOWNDERECHA){

            exit_menu = 1;
            adondevamos = 2;
        }
        else if (eventoActual == DOWNBOTON){

            exit_menu = 1;
            adondevamos = 1;
        }
    }
    return adondevamos;
}

int verTopScores (void){
    int adondevamos = 0;  //adonde vamos es igual a 1 si empezamos el juego e igual a 2 si vamos a ver la tabla de puntajes
    char eventoActual = 0;
    int exit_menu = 0;

    char raspiscore [16][16] = {    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} , // He aqui el menu principal de la raspi hardcodeado
                                    {1,0,1,0,1,1,1,0,1,1,1,0,0,1,0,1} ,
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

    while (!exit_menu){

        while ( esBufferVacio() == 1 ); //SE PODRIA REHACER CON SEMAFOROS, por ahora esto funciona
        eventoActual = getInputEvent();

        if (eventoActual == DOWNDERECHA){

            //Podriamos movernos hacia otras opciones del menu
        }
        else if (eventoActual == DOWNIZQUIERDA){

            exit_menu = 1;
            adondevamos = 0;
        }
        else if (eventoActual == DOWNBOTON){

            exit_menu = 1;
            adondevamos = 3;
        }
    }
    return adondevamos;
}

int TopScore (void){
    int adondevamos = 0;  //adonde vamos es igual a 1 si empezamos el juego e igual a 2 si vamos a ver la tabla de puntajes
    char eventoActual = 0;
    int exit_menu = 0;

    char raspiscore [16][16] = {    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1} ,
                                    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0} ,
                                    {0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0} ,
                                    {0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0} ,
                                    {0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0} ,
                                    {0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0} ,
                                    {0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0} ,
                                    {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0} ,
                                    {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0} ,
                                    {0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0} ,
                                    {0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0} ,
                                    {0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0} ,
                                    {0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0} ,
                                    {0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0} ,
                                    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0} ,
                                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1} ,
                };

    actualizarDisplay(raspiscore);

    while (!exit_menu){

        while ( esBufferVacio() == 1 ); //SE PODRIA REHACER CON SEMAFOROS, por ahora esto funciona
        eventoActual = getInputEvent();

        if (eventoActual == DOWNDERECHA){

            //Podriamos movernos hacia otras opciones del menu
        }
        else if (eventoActual == DOWNBOTON){

            exit_menu = 1;
            adondevamos = 2;
        }
    }
    return adondevamos;
}

#endif

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
                         
void imprimirHighScore (int numero) {
    
    const char pos_iniciales [4][2] = {{9,1},{9,5},{9,9},{9,13}};
    
    int i=0, j=0, cont1=0, cont2=0, x=0, y=0;
    int digitos [MAXSCORELENGTH] = {0,0,0,0};
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
    
    for(cont1=0;cont1<MAXSCORELENGTH;cont1++) {
        for(j=0;j<=2;j++) {
            for(i=0;i<=4;i++) {
                raspihighscore[pos_iniciales[cont1][0]+i][pos_iniciales[cont1][1]+j]=matrices_num[digitos[cont1]][i][j];
            }
        }
    }
}
