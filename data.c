/***************************************************************************//**
  file     data.h
  @brief    Funciones para la lectura de archivos .txt localizados en la carpeta data
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "data.h"

#define PATHFORRASPI "./cmake-build-debug/%s"

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

enum files{MENUIMG, MENUTEXT, ESTADOJUEGO, TEXTURAS, SOUNDS, FONTS};


/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/


static const char *level[] = {"/data/level1.txt", "/data/level2.txt", "/data/level3.txt"};
static const char *path[] = {"/data/imgMenuData.txt", "/data/textMenuData.txt", "/data/estadoJuegoData.txt",
                      "/data/texturesData.txt", "/data/soundsData.txt", "/data/fontsData.txt"};


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


#if MODOJUEGO == ALLEGRO

int openGameStateFile(FILE **gameStateData){

    char efectivePath[50];

    sprintf(efectivePath, ".%s", path[ESTADOJUEGO]);
    *gameStateData = fopen(efectivePath, "r+");
    if(gameStateData == NULL){
        printf("Error al abrir el fichero con path: %s\n", efectivePath);
        return 1;
    }
    return 0;
}

int openLevelData(FILE **levelData, int id){

    char efectivePath[50];

    sprintf(efectivePath, ".%s", level[id]);
    *levelData = fopen(efectivePath, "r+");
    if(*levelData == NULL){        //Error al cargar el archivo
        printf("Error al abrir el fichero con path: %s", efectivePath);
        return 1;
    }
    return 0;
}

int openSoundsFile(FILE **soundData){

    char efectivePath[50];

    sprintf(efectivePath, ".%s", path[SOUNDS]);
    *soundData = fopen(efectivePath, "r+");
    if(*soundData == NULL){        //Error al cargar el archivo
        printf("Error al abrir el fichero con path: %s\n", efectivePath);
        return 1;
    }
    return 0;
}

int openTexturesFile(FILE **texturaData){

    char efectivePath[50];

    sprintf(efectivePath, ".%s", path[TEXTURAS]);
    *texturaData = fopen(efectivePath, "r+");
    if(*texturaData == NULL){        //Error al cargar el archivo
        printf("Error al abrir el fichero con path: %s\n", efectivePath);
        return 1;
    }
    return 0;
}

int openFontsFile(FILE **fontsData){

    char efectivePath[50];

    sprintf(efectivePath, ".%s", path[FONTS]);
    *fontsData = fopen(efectivePath, "r+");
    if(*fontsData == NULL){        //Error al cargar el archivo
        printf("Error al abrir el fichero con path: %s", efectivePath);
        return 1;
    }
    return 0;
}

int openMenuData(FILE **imageMenuData, FILE **textMenuData){

    char efectivePath[50];
    int error = 0;

    sprintf(efectivePath, ".%s", path[MENUIMG]);
    *imageMenuData = fopen(efectivePath, "r+");

    sprintf(efectivePath, ".%s", path[MENUTEXT]);
    *textMenuData = fopen(efectivePath, "r+");

    if(*imageMenuData == NULL){        //Error al cargar el archivo
        printf("Error al abrir el fichero con path: .%s\n", path[MENUIMG]);
        error = 1;
    }
    else if(*textMenuData == NULL){
        printf("Error al abrir el fichero con path: .%s\n", path[MENUTEXT]);
        error = 1;
    }

    return error;
}

#elif MODOJUEGO == RASPI

int openGameStateFile(FILE **gameStateData){

    char efectivePath[50];

    sprintf(efectivePath, PATHFORRASPI, path[ESTADOJUEGO]);
    *gameStateData = fopen(efectivePath, "r+");
    if(gameStateData == NULL){
        printf("Error al abrir el fichero con path: %s\n", efectivePath);
        return 1;
    }
    return 0;
}

int openLevelData(FILE **levelData, int id){

    char efectivePath[50];

    sprintf(efectivePath, PATHFORRASPI, level[id]);
    *levelData = fopen(efectivePath, "r+");
    if(*levelData == NULL){        //Error al cargar el archivo
        printf("Error al abrir el fichero con path: %s", efectivePath);
        return 1;
    }
    return 0;
}

int openSoundsFile(FILE **soundData){

    char efectivePath[50];

    sprintf(efectivePath, PATHFORRASPI, path[SOUNDS]);
    *soundData = fopen(efectivePath, "r+");
    if(*soundData == NULL){        //Error al cargar el archivo
        printf("Error al abrir el fichero con path: %s\n", efectivePath);
        return 1;
    }
    return 0;
}

#endif
