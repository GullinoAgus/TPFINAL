/***************************************************************************//**
  @file     data.h
  @brief    Funciones para la lectura de archivos .txt localizados en la carpeta data
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "data.h"

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

#if MODOJUEGO == 0

enum files{MENUIMG, MENUTEXT, ESTADOJUEGO, TEXTURAS, SOUNDS, FONTS};

#elif MODOJUEGO == 1

enum files{ESTADOJUEGO, SOUNDS};

#endif

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

// static int verifyLevelData(const char* direccion); //TODO: Esta funcion no se definio

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

#if MODOJUEGO == ALLEGRO

static const char *level[] = {"./data/level1.txt", "./data/level2.txt", "./data/level3.txt"};
static const char *path[] = {"./data/imgMenuData.txt", "./data/textMenuData.txt", "./data/estadoJuegoData.txt",
                      "./data/texturesData.txt", "./data/soundsData.txt", "./data/fontsData.txt"};

#elif MODOJUEGO == RASPI

static const char *level[] = {"./cmake-build-debug/data/level1.txt", "./cmake-build-debug/data/level2.txt", "./cmake-build-debug/data/level3.txt"};
static const char *path[] = {"./cmake-build-debug/data/estadoJuegoData.txt","./cmake-build-debug/data/soundsData.txt" };

#endif

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

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

int openLevelData(FILE **levelData, int id){
    *levelData = fopen(level[id], "r");
    if(*levelData == NULL){        //Error al cargar el archivo
        printf("Error al abrir el fichero con path: %s", level[id]);
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

#if MODOJUEGO == ALLEGRO

int openTexturesFile(FILE **texturaData){
    *texturaData = fopen(path[TEXTURAS], "r");
    if(*texturaData == NULL){        //Error al cargar el archivo
        printf("Error al abrir el fichero con path: %s\n", path[TEXTURAS]);
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

#endif
