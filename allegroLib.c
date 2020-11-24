//
// Created by gonzalo on 23/11/20.
//

#include <stdio.h>
#include "allegroLib.h"
#include "data/data.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_ttf.h>

int inicializarAllegro(ALLEGRO_DISPLAY* disp){

    int salida = 0;

    al_init();

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

// 0 si cargo todo bien, sino
int cargarTexturasMenu(img_t *textura){

        char rutaEfectiva[30];
        int error = 0;

        for(int i = 0; !error && i < CANTTEXTURAS; i++){

            int size = sprintf(rutaEfectiva, "resources/textures/%d.png", i);
            textura[i] = al_load_bitmap(rutaEfectiva);

            if(textura[i] == NULL)
            {
                printf("couldn't load %s\n", rutaEfectiva);
                error = 1;
            }
        }

        return error;
    }
}
int cargarSonidosMenu(sonido_t *sonido){

    char rutaEfectiva[30];
    int error = 0;

    for(int i = 0; !error && i < CANTTEXTURAS; i++){

        int size = sprintf(rutaEfectiva, "resources/textures/%d.png", i);
        textura[i] = al_load_bitmap(rutaEfectiva);

        if(textura[i] == NULL)
        {
            printf("couldn't load %s\n", rutaEfectiva);
            error = 1;
        }
    }

    return error;
}
int cargarFuentesMenu(fuente_t *fuente){

}