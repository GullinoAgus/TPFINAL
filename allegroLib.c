/***************************************************************************//**
  @file     allegroLib.c
  @brief    Funciones de carga de datos e inicializacion relacionadas a Allegro. Tambien carga de highscores
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "allegroLib.h"
#include <stdio.h>

#if MODOJUEGO == ALLEGRO  //TODO: Estas definiciones no harian falta porque ya nos las trae matiasBrosGame.h

#include "allegro.h"
#include "render.h"

#endif

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

#if MODOJUEGO == ALLEGRO

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

    return salida;
}

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
                 error = 1;
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
        if (error){
            return -1;
        } else {
            return cantDeTexturas;
        }
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
                error = 1;
            } else {
                if ((*fuente)[i] == NULL) {
                    printf("couldn't load %s\n", path);
                    error = 1;
                }
            }
        }
    }

    fclose(fuenteData);
    if (error){
        return -1;
    } else {
        return cantDeFuentes;
    }

}

void destroyResources(bufferRecursos_t *resourcesBuffer){

    free(resourcesBuffer->image);
    endAudio();
    for(int i = 0; i < resourcesBuffer->soundQuant; i++) {
        SDL_FreeWAV(resourcesBuffer->sound[i]->bufferTrue);
    }
    free(resourcesBuffer->sound);
    //FIXME falta liberar cierta parte del audio, revisar freeAudio
    free(resourcesBuffer->font);

}

#elif MODOJUEGO == RASPI

void destroyResources(bufferRecursos_t *resourcesBuffer){

    endAudio();
    for(int i = 0; i < resourcesBuffer->soundQuant; i++) {
        if (resourcesBuffer->sound[i]->free == 1) {
            SDL_FreeWAV(resourcesBuffer->sound[i]->bufferTrue);
        }
    }
    free(resourcesBuffer->sound);
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

int cargarSonidosMenu(sonido_t **sonido) {

    int error = 0;
    int cantDeSonidos = 0;
    FILE *sonidoData;


    if (openSoundsFile(&sonidoData) == 1) {
        return -1;
    } else {
        fscanf(sonidoData, "%d", &cantDeSonidos);
        (*sonido) = (sonido_t *) malloc(sizeof(sonido_t) * cantDeSonidos);
        if (*sonido == NULL) {
            error = 1;
        } else {
            for (int i = 0; !error && i < cantDeSonidos; i++) {
                char path[40];

                fscanf(sonidoData, "./cmake-build-debug/%s", path);

                (*sonido)[i] = createAudio(path, 0, SDL_MIX_MAXVOLUME);

                if ((*sonido)[i] == NULL) {
                    printf("couldn't load %s\n", path);
                    error = 1;
                }
            }
        }
    }

    fclose(sonidoData);
    if (error) {
        return -1;
    } else {
        return cantDeSonidos;
    }
}