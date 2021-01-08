//
// Created by gonzalo on 23/11/20.
//

#include "data.h"

enum files{MENUIMG, MENUTEXT, ESTADOJUEGO, TEXTURAS, SOUNDS, FONTS};
const char *path[] = {"./data/imgMenuData.txt", "./data/textMenuData.txt", "./data/estadoJuegoData.txt",
                      "./data/texturesData.txt", "./data/soundsData.txt", "./data/fontsData.txt"};
const char *level[] = {"./data/level1.txt", "./data/level2.txt", "./data/level3.txt"};


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
        printf("Error al abrir el fichero con path: %\n", path[TEXTURAS]);
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

/*
int* openLevelData(int id){

    int* mapa;
    int cantBloques = verifyLevelData(level[id]);
    int contadorChequeo = 0;

    if (cantBloques <= 0){
        return NULL;
    }else {
        mapa = calloc(cantBloques, sizeof(int));
        FILE* nivel = fopen(level[id],"r");
        int lectura;

        do {
            lectura = fgetc(nivel);
            if (LECTURAVALIDA(lectura)){
                *mapa++ = lectura;
                contadorChequeo++;
            }
        }while (lectura != EOF);
        if (cantBloques != contadorChequeo){
            free(mapa);
            mapa = NULL;
        }
        fclose(nivel);
    }

    return (mapa - cantBloques);
}

static int verifyLevelData(const char* direccion){

    FILE* nivel = fopen(direccion, "r");
    if (nivel == NULL){
        fclose(nivel);
        return 0;
    }
    char car = 0;
    int celdaVacia = 0;
    int contadorCar = 0;
    int contadorLineas = 0;
    int contadorBordes = 0;
    int hayJugador = 0;

    do {
        car = fgetc(nivel);
        celdaVacia++;
        if (car != ';' && car != '\n' && car != EOF){
            celdaVacia = 0;
            contadorCar++;
            if (car == JUGADOR){
                hayJugador++;
            }else if (car == BORDE){
                contadorBordes++;
            }
        }
        if (celdaVacia == 2){
            celdaVacia = 1;
            contadorCar++;
        }
        if (car == '\n' || car == EOF){
            contadorLineas++;
        }
    }while (car != EOF);

    if (contadorLineas != 16){
        contadorCar = 0;
    } else if( hayJugador != 1){
        contadorCar = -1;
    } else if (((contadorBordes/2) != contadorLineas) || contadorBordes%2 !=0) {
        contadorCar = -2;
    }

    fclose(nivel);
    return contadorCar;
}
 */
