//
// Created by gonzalo on 23/11/20.
//

#include "data.h"
#include <stdio.h>
#include <stdlib.h>

enum files{MENUIMG, MENUTEXT, ESTADOJUEGO, TEXTURAS, SOUNDS, FONTS};
const char *path[] = {"./data/imgMenuData.txt", "./data/textMenuData.txt", "./data/estadoJuegoData.txt",
                      "./data/texturesData.txt", "./data/soundsData.txt", "./data/fontsData.txt"};

int loadGameState(estadoJuego_t *gameState){

    int error = 0;
    FILE* file = fopen(path[ESTADOJUEGO], "r");
    if(file == NULL){
        error = 1;
    }
    else{
        fscanf(file, "%d", &(gameState->maxEntries) );
        for(int i = 0; i < gameState->maxEntries; i++){
            fscanf(file, "%d", &gameState->bestScores[i]);
            fscanf(file, "%s", (gameState->bestScoresName)[i]);
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

int openMenuData(FILE **imageMenuData, FILE **textMenuData){
    *imageMenuData = fopen(path[MENUIMG], "r");
    *textMenuData = fopen(path[MENUTEXT], "r");
    int error = 0;

    if(*imageMenuData == NULL){        //Error al cargar el archivo
        printf("Error al abrir el fichero con path: %s", path[MENUIMG]);
        error = 1;
    }
    else if(*textMenuData == NULL){
        printf("Error al abrir el fichero con path: %s", path[MENUTEXT]);
        error = 1;
    }

    return error;
}
