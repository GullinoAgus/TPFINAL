//
// Created by gonzalo on 23/11/20.
//

#include "menu.h"
#include "data.h"
#include "allegroLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
// #include IOEvents.h

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

int drawMenu(bufferRecursos *buffer) {

    if(loadMenuData() == 1){
        return 1;
    }
    else{
        for(int i = 0; i < menu.imgQuant; i++){
            image_t currentImg = (buffer->image)[i];
            al_draw_scaled_bitmap(currentImg, 0, 0, al_get_bitmap_width(currentImg), al_get_bitmap_height(currentImg), menu.imgMenu[i].x, menu.imgMenu[i].y,al_get_bitmap_width(currentImg) * menu.imgMenu[i].scale, al_get_bitmap_height(currentImg) * menu.imgMenu[i].scale, 0);
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

static void playMenuSound(){

}

/*
void updateMenu(bufferRecursos *buffer) {
    loadMenuData();
}
 */
/*
void actualizarMenu (void){
    static int posXflecha = menu[2].x ;
    static int posYflecha = menu[2].y ;
    char eventoActual = 0;

    while ((eventoActual = getInputEvent() ) != 0){

        if (eventoActual == DOWNABAJO){


        }
        else if (eventoActual == DOWNARRIBA){


        }
        else if (eventoActual == DOWNBOTON){


        }
    }
}
*/