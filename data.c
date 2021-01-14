//
// Created by gonzalo on 23/11/20.
//

#include "data.h"

#if MODOJUEGO == 0

const char *level[] = {"./data/level1.txt", "./data/level2.txt", "./data/level3.txt"};

#elif MODOJUEGO == 1

const char *level[] = {"./cmake-build-debug/data/level1.txt", "./cmake-build-debug/data/level2.txt", "./cmake-build-debug/data/level3.txt"};

#endif


enum files{MENUIMG, MENUTEXT, ESTADOJUEGO, TEXTURAS, SOUNDS, FONTS};
const char *path[] = {"./data/imgMenuData.txt", "./data/textMenuData.txt", "./data/estadoJuegoData.txt",
                      "./data/texturesData.txt", "./data/soundsData.txt", "./data/fontsData.txt"};

static int verifyLevelData(const char* direccion);

const char* getScoreFilePath(){
    return path[ESTADOJUEGO];
}

int openGameStateFile(FILE **gameStateData){
    *gameStateData = fopen(path[ESTADOJUEGO], "r");
    if(gameStateData == NULL){
        return 1;
    }
    return 0;
}

int openTexturesFile(FILE **texturaData){
    *texturaData = fopen(path[TEXTURAS], "r");
    if(*texturaData == NULL){        //Error al cargar el archivo
        printf("Error al abrir el fichero con path: %s\n", path[TEXTURAS]);
        return 1;
    }
    return 0;
}
int openSoundsFile(FILE **soundData){
    *soundData = fopen(path[SOUNDS], "r");
    if(*soundData == NULL){        //Error al cargar el archivo
        printf("Error al abrir el fichero con path: %s\n", path[SOUNDS]);
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
        printf("Error al abrir el fichero con path: %s\n", path[MENUIMG]);
        error = 1;
    }
    else if(*textMenuData == NULL){
        printf("Error al abrir el fichero con path: %s\n", path[MENUTEXT]);
        error = 1;
    }

    return error;
}

int openLevelData(FILE **levelData, int id){
    *levelData = fopen(level[id], "r");
    if(*levelData == NULL){        //Error al cargar el archivo
        printf("Error al abrir el fichero con path: %s", level[id]);
        return 1;
    }
    return 0;
}
