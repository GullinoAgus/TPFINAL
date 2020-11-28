//
// Created by gonzalo on 23/11/20.
//
#include "configuracion.h"

#if MODOJUEGO == 0

#include <stdio.h>
#include "allegroLib.h"
#include "data.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_ttf.h>

int inicializarAllegro(ALLEGRO_DISPLAY **disp){

    int salida = 0;

    al_init();  //Inicializamos allegro

    *disp = al_create_display(SCREENWIDHT, SCREENHEIGHT);
    if(disp == NULL){  //inicializamos el display
        al_show_native_message_box(disp, "Error", "ERROR", "Error al inicializar el display, viva peron", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        salida = 1;
    }

    if(!al_init_image_addon())     //Inicializo el addon para manejo de imagenes, en caso de error muestro un mensaje
    {
        al_show_native_message_box(disp, "Error", "ERROR", "Error al iniciar addon de imagenes", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        salida = 1;
    }

    if(!al_init_font_addon())       //Inicializo el addon para manejo de fuentes, en caso de error muestro un mensaje
    {
        al_show_native_message_box(disp, "Error", "ERROR", "Error al iniciar addon fuentes", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        salida = 1;
    }

    if(!al_init_ttf_addon())       //Inicializo el addon para manejo de archivos .ttf de fuentes, en caso de error muestro un mensaje
    {
        al_show_native_message_box(disp, "Error", "ERROR", "Error al iniciar addon de lectura de fuentes", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        salida = 1;
    }

    if(!al_init_acodec_addon())       //Inicializo el addon para manejo de archivos de audio, en caso de error muestro un mensaje
    {
        al_show_native_message_box(disp, "Error", "ERROR", "Error al iniciar addon de codec de audio", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        salida = 1;
    }

    if (!al_install_keyboard()) {       //Inicializo el teclado, en caso de error muestro un mensaje
        al_show_native_message_box(disp, "Error", "ERROR", "Error al instalar el teclado", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        salida = 1;
    }

    if (!al_install_mouse()) {          //Inicializo el mouse, en caso de error muestro un mensaje
        al_show_native_message_box(disp, "Error", "ERROR", "Error al instalar el mouse", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        salida = 1;
    }

    if (!al_install_audio()) {          //Inicializo el audio, en caso de error muestro un mensaje
        al_show_native_message_box(disp, "Error", "ERROR", "Error al instalar el audio", NULL, ALLEGRO_MESSAGEBOX_ERROR);
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
        *sonido = (sonido_t*) malloc(sizeof(sonido_t) * cantDeSonidos);
        if(*sonido == NULL) {
            return -1;
        }
        else {
            for (int i = 0; !error && i < cantDeSonidos; i++) {
                char path[50];
                fscanf(sonidoData, "%s", path);
                (*sonido)[i] = al_load_sample(path);

                if ((*sonido)[i] == NULL) {
                    printf("couldn't load %s\n", path);
                    return -1;
                }
            }
        }
    }

    fclose(sonidoData);
    return cantDeSonidos;
}
int cargarFuentesMenu(fuente_t **fuente){
    int error = 0;
    int cantDeFuentes = 0;
    FILE *fuenteData;

    if(openFontsFile(&fuenteData) == 1) {
        return -1;
    }
    else {
        fscanf(fuenteData, "%d", &cantDeFuentes);
        *fuente = (fuente_t*) malloc(sizeof(fuente_t) * cantDeFuentes);
        for (int i = 0; !error && i < cantDeFuentes; i++) {
            char path[50];
            fscanf(fuenteData, "%s", path);
            (*fuente)[i] = al_load_font(path, 80, 0);
            if(*fuente == NULL) {
                return -1;
            }
            else {
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
void destroyResources(bufferRecursos *resourcesBuffer){

    free(resourcesBuffer->image);
    free(resourcesBuffer->sound);
    free(resourcesBuffer->font);

}

int loadGameState(estadoJuego_t *gameState){

    int error = 0;
    FILE* gameStateData;

    if(openGameStateFile(&gameStateData) == 1){
        return -1;
    }
    else{
        fscanf(gameStateData, "%d", &(gameState->maxEntries) );
        for(int i = 0; i < gameState->maxEntries; i++){
            fscanf(gameStateData, "%d", &gameState->bestScores[i]);
            fscanf(gameStateData, "%s", (gameState->bestScoresName)[i]);
        }
    }

    gameState->menuSelection = 0;

    fclose(gameStateData);
    return error;
}

#endif