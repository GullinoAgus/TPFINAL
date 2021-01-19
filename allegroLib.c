//
// Created by gonzalo on 23/11/20.
//
#include "configuracion.h"
#include "allegroLib.h"
#include <stdio.h>

#if MODOJUEGO == 0


int inicializarAllegro(){

    int salida = 0;

    al_init();  //Inicializamos allegro

    if(!al_init_image_addon())     //Inicializo el addon para manejo de imagenes, en caso de error muestro un mensaje
    {
        salida = 1;
    }

    if(!al_init_font_addon())       //Inicializo el addon para manejo de fuentes, en caso de error muestro un mensaje
    {
        salida = 1;
    }

    if(!al_init_ttf_addon())       //Inicializo el addon para manejo de archivos .ttf de fuentes, en caso de error muestro un mensaje
    {
        salida = 1;
    }

    if(!al_init_acodec_addon())       //Inicializo el addon para manejo de archivos de audio, en caso de error muestro un mensaje
    {
        salida = 1;
    }

    if (!al_install_keyboard()) {       //Inicializo el teclado, en caso de error muestro un mensaje
        salida = 1;
    }

    if (!al_install_mouse()) {          //Inicializo el mouse, en caso de error muestro un mensaje
        salida = 1;
    }

    if (!al_install_audio()) {          //Inicializo el audio, en caso de error muestro un mensaje
        salida = 1;
    }

    return salida;
}

// 0 si se cargo bien, sino 1
int cargarTexturasMenu(image_t **textura){

        int error = 0;
        int cantDeTexturas = 0;
        FILE *texturaData;

        if(openTexturesFile(&texturaData) == 1) {
            return -1;
        }
        else {
            fscanf(texturaData, "%d", &cantDeTexturas);
            *textura = malloc(sizeof(image_t) * cantDeTexturas);
            if(*textura == NULL) {
                 return -1;
            }
            else{
                for (int i = 0; !error && i < cantDeTexturas; i++) {
                    char path[50];
                    fscanf(texturaData, "%s", path);
                    (*textura)[i] = al_load_bitmap(path);

                    if ((*textura)[i] == NULL) {
                        printf("couldn't load %s\n", path);
                        error = 1;
                    }
                }
            }
        }

        fclose(texturaData);
        return cantDeTexturas;
}
int cargarSonidosMenu(sonido_t **sonido){

    int error = 0;
    int cantDeSonidos = 0;
    FILE *sonidoData;

    if(openSoundsFile(&sonidoData) == 1) {
        return -1;
    }
    else {
        fscanf(sonidoData, "%d", &cantDeSonidos);
        (*sonido) = (sonido_t*) malloc(sizeof(sonido_t) * cantDeSonidos);
        if(*sonido == NULL) {
            return -1;
        }
        else {
            for (int i = 0; !error && i < cantDeSonidos; i++) {
                char path[50];
                fscanf(sonidoData, "%s", path);
                (*sonido)[i] = createAudio(path, 0, SDL_MIX_MAXVOLUME);

                if ((*sonido)[i] == NULL) {
                    printf("couldn't load %s\n", path);
                    return -1;
                }
            }
        }
    }
    (**sonido)->loop = 1;
    fclose(sonidoData);
    return cantDeSonidos;
}
int cargarFuentesMenu(fuente_t **fuente) {
    int error = 0;
    int cantDeFuentes = 0;
    FILE *fuenteData;

    if (openFontsFile(&fuenteData) == 1) {
        return -1;
    } else {
        fscanf(fuenteData, "%d", &cantDeFuentes);
        *fuente = (fuente_t *) malloc(sizeof(fuente_t) * cantDeFuentes);
        for (int i = 0; !error && i < cantDeFuentes; i++) {
            char path[50];
            int fontSize = 0;
            fscanf(fuenteData, "%s %d", path, &fontSize);
            (*fuente)[i] = al_load_font(path, fontSize, 0);
            if (*fuente == NULL) {
                return -1;
            } else {
                if ((*fuente)[i] == NULL) {
                    printf("couldn't load %s\n", path);
                    return -1;
                }
            }
        }
    }

    fclose(fuenteData);
    return cantDeFuentes;

}

void destroyResources(bufferRecursos_t *resourcesBuffer){

    free(resourcesBuffer->image);
    free(resourcesBuffer->sound);
    free(resourcesBuffer->font);

}

#endif

int loadGameState(estadoJuego_t *gameState){

    int error = 0;
    FILE* gameStateData;

    if(openGameStateFile(&gameStateData) == 1){
        return -1;
    }
    else{
        fscanf(gameStateData, "%d", &(gameState->maxTopScoreEntries) );
        for(int i = 0; i < gameState->maxTopScoreEntries; i++){
            fscanf(gameStateData, "%d", &gameState->bestScores[i]);
            fscanf(gameStateData, "%s", (gameState->bestScoresName)[i]);
        }
    }

    gameState->entidades.enemigos = NULL;
    gameState->entidades.bloques = NULL;
    gameState->menuSelection = 0;

    fclose(gameStateData);
    return error;
}