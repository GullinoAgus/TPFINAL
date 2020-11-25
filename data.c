//
// Created by gonzalo on 23/11/20.
//

#include "data.h"
#include <stdio.h>
#include <stdlib.h>

enum files{MENU, ESTADOJUEGO, TEXTURAS, SOUNDS, FONTS};
const char *path[] = {"./data/menuData.txt", "./data/estadoJuegoData.txt", "./data/texturesData.txt", "./data/soundsData.txt", "./data/fontsData.txt"};

int loadGameState(estadoJuego_t *gameState){

    int error = 0;
    FILE* file = fopen(path[ESTADOJUEGO], "r");
    if(file == NULL){
        error = 1;
    }
    else{
        int maxEntries;
        fscanf(file, "%d", &maxEntries);
        for(int i = 0; i < maxEntries; i++){
            fscanf(file, "%d %s", &gameState->bestScores[i], gameState->bestScoresName[i]);
        }
    }

    fclose(file);
    return error;
}

int openTexturesFile(FILE **texturaData){
    *texturaData = fopen(path[TEXTURAS], "r");
    if(*texturaData == NULL){        //Error al cargar el archivo
        printf("Error al abrir el fichero con path: %s", path[TEXTURAS]);
        return 1;
    }
    return 0;
}
int openSoundsFile(FILE **soundData){
    *soundData = fopen(path[SOUNDS], "r");
    if(*soundData == NULL){        //Error al cargar el archivo
        printf("Error al abrir el fichero con path: %s", path[SOUNDS]);
        return 1;
    }
    return 0;
}
int openFontsFile(FILE **fontsData){
    *fontsData = fopen(path[FONTS], "r");
    if(*fontsData == NULL){        //Error al cargar el archivo
        printf("Error al abrir el fichero con path: %s", path[FONTS]);
        return 1;
    }
    return 0;
}
int openMenuData(FILE **menuData){
    *menuData = fopen(path[MENU], "r");
    if(*menuData == NULL){        //Error al cargar el archivo
        printf("Error al abrir el fichero con path: %s", path[MENU]);
        return 1;
    }
    return 0;
}
