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

typedef struct{
    float x;
    float y;
    float scale;
}menu_t;

static menu_t *menu;
static int menuElementsSize;

static int loadMenuData(){
    FILE *menuData;
    int cantDeElementos = 0;
    int error = 0;
    int i;

    if(openMenuData(&menuData) == 1){
        error = 1;
    }
    else{
        fscanf(menuData, "%d", &cantDeElementos);
        menuElementsSize = cantDeElementos;
        menu = (menu_t*) malloc(sizeof(menu_t) * (cantDeElementos));
        if(menu == NULL){
            error = 1;
        }
        else {
            for (i = 0; i < cantDeElementos; i++) {
                fscanf(menuData, "%f %f %f", &menu[i].x, &menu[i].y, &menu[i].scale);
            }
        }
    }

    fclose(menuData);
    return error;
}

int drawMenu(bufferRecursos *buffer) {

    if(loadMenuData() == 1){
        return 1;
    }
    else{
        for(int i = 0; i < menuElementsSize; i++){
            image_t currentImg = (buffer->image)[i];
            al_draw_scaled_bitmap(currentImg, 0, 0, al_get_bitmap_width(currentImg), al_get_bitmap_height(currentImg), menu[i].x, menu[i].y, al_get_bitmap_width(currentImg) * menu[i].scale, al_get_bitmap_height(currentImg) * menu[i].scale, 0);
        }
    }

    //Falta dibujar las fuentes

    return 0;

}

static void playMenuSound(){

}

/*
void updateMenu(bufferRecursos *buffer) {
    loadMenuData();
}
 */
