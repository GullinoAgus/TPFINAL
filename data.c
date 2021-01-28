/***************************************************************************//**
  file     data.h
  @brief    Funciones para la lectura de archivos .txt localizados en la carpeta data
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "data.h"

#define PATHFORRASPI "./cmake-build-debug/%s"   //En la raspi tenemos que acomodarnos en esta carpeta
                                                //En modo allegro no porque se compila desde ahi

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

    sprintf(efectivePath, ".%s", path[ESTADOJUEGO]);    //Se busca el path correspondiente en el arreglo path
    *gameStateData = fopen(efectivePath, "r+");         //Se abre el archivo buscado
    if(gameStateData == NULL){
        printf("Error al abrir el fichero con path: %s\n", efectivePath);
        return 1;
    }
    return 0;
}

int openLevelData(FILE **levelData, int id){

    char efectivePath[50];

    sprintf(efectivePath, ".%s", level[id]);    //Se busca el path correspondiente en el arreglo level
    *levelData = fopen(efectivePath, "r+");     //Se abre el archivo buscado
    if(*levelData == NULL){        //Error al cargar el archivo
        printf("Error al abrir el fichero con path: %s", efectivePath);
        return 1;
    }
    return 0;
}

int openSoundsFile(FILE **soundData){

    char efectivePath[50];

    sprintf(efectivePath, ".%s", path[SOUNDS]); //Se busca el path correspondiente en el arreglo path
    *soundData = fopen(efectivePath, "r+");     //Se abre el archivo buscado
    if(*soundData == NULL){        //Error al cargar el archivo
        printf("Error al abrir el fichero con path: %s\n", efectivePath);
        return 1;
    }
    return 0;
}

int openTexturesFile(FILE **texturaData){

    char efectivePath[50];

    sprintf(efectivePath, ".%s", path[TEXTURAS]);       //Se busca el path correspondiente en el arreglo path
    *texturaData = fopen(efectivePath, "r+");           //Se abre el archivo buscado
    if(*texturaData == NULL){        //Error al cargar el archivo
        printf("Error al abrir el fichero con path: %s\n", efectivePath);
        return 1;
    }
    return 0;
}

int openFontsFile(FILE **fontsData){

    char efectivePath[50];

    sprintf(efectivePath, ".%s", path[FONTS]);      //Se busca el path correspondiente en el arreglo path
    *fontsData = fopen(efectivePath, "r+");          //Se abre el archivo buscado
    if(*fontsData == NULL){        //Error al cargar el archivo
        printf("Error al abrir el fichero con path: %s", efectivePath);
        return 1;
    }
    return 0;
}

int openMenuData(FILE **imageMenuData, FILE **textMenuData){

    char efectivePath[50];
    int error = 0;

    sprintf(efectivePath, ".%s", path[MENUIMG]);    //Se busca el path correspondiente en el arreglo path
    *imageMenuData = fopen(efectivePath, "r+"); //Se abre el archivo buscado

    sprintf(efectivePath, ".%s", path[MENUTEXT]);   //Se busca el path correspondiente en el arreglo path
    *textMenuData = fopen(efectivePath, "r+");      //Se abre el archivo buscado

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

    sprintf(efectivePath, PATHFORRASPI, path[ESTADOJUEGO]); //Se busca el path correspondiente en el arreglo path
    *gameStateData = fopen(efectivePath, "r+");             //Se abre el archivo buscado
    if(gameStateData == NULL){
        printf("Error al abrir el fichero con path: %s\n", efectivePath);
        return 1;
    }
    return 0;
}

int openLevelData(FILE **levelData, int id){

    char efectivePath[50];

    sprintf(efectivePath, PATHFORRASPI, level[id]);         //Se busca el path correspondiente en el arreglo level
    *levelData = fopen(efectivePath, "r+");                 //Se abre el archivo buscado
    if(*levelData == NULL){        //Error al cargar el archivo
        printf("Error al abrir el fichero con path: %s", efectivePath);
        return 1;
    }
    return 0;
}

int openSoundsFile(FILE **soundData){

    char efectivePath[50];

    sprintf(efectivePath, PATHFORRASPI, path[SOUNDS]);         //Se busca el path correspondiente en el arreglo path
    *soundData = fopen(efectivePath, "r+");                    //Se abre el archivo buscado
    if(*soundData == NULL){        //Error al cargar el archivo
        printf("Error al abrir el fichero con path: %s\n", efectivePath);
        return 1;
    }
    return 0;
}

#endif
