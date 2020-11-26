#include <stdio.h>
#include "configuracion.h"
#include "menu.h"
#include "data.h"
#include "graficos.h"
#include "entidades.h"
#include "fisica.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <pthread.h>

pthread_mutex_t lock;
int main() {

    jugador_t jug = {.fisica = {.posx = 115, .posy = 200, .ancho = 40, .alto = 40,.vely = 0, .velx = 0}};
    bloque_t blq1 = {.fisica = {.posx = 100, .posy = 500, .ancho = 100, .alto = 30,.vely = 0, .velx = 0}};
    bloque_t blq2 = {.fisica = {.posx = 300, .posy = 100, .ancho = 10, .alto = 300,.vely = 0, .velx = 0}};
    bloque_t *blq[3] = {&blq1, &blq2, NULL};

    enemigo_t *enem[] = {NULL, NULL, NULL};
    entidades_t entidades = {jug, enem, blq};

    bool done = 0;
    al_init();
    al_init_image_addon();
    ALLEGRO_DISPLAY* display = al_create_display(800, 600);

    al_install_keyboard();
    al_init_primitives_addon();

    ALLEGRO_EVENT_QUEUE* cola = al_create_event_queue();
    ALLEGRO_EVENT evento;

    al_register_event_source(cola,al_get_keyboard_event_source());
    pthread_t idfisica, idcolisiones;
    pthread_create(&idfisica, NULL, fisica, &entidades);
    pthread_create(&idcolisiones, NULL, colisiones, &entidades);

    pthread_mutex_init(&lock, NULL);
    while (!done){

        al_clear_to_color(al_map_rgb(255,255,0));
        if (!al_is_event_queue_empty(cola)){
            al_get_next_event(cola, &evento);
            pthread_mutex_lock(&lock);
            switch (evento.type) {
                case ALLEGRO_EVENT_KEY_DOWN:
                    switch (evento.keyboard.keycode) {
                        case ALLEGRO_KEY_Q:
                            done = 1;
                            break;
                        case ALLEGRO_KEY_UP:
                            entidades.jugador.fisica.vely = -0.5f;
                            entidades.jugador.sobreBloque = false;
                            break;
                        case ALLEGRO_KEY_RIGHT:
                            entidades.jugador.fisica.velx = 1.0f;
                            break;
                        case ALLEGRO_KEY_LEFT:
                            entidades.jugador.fisica.velx = -1.0f;
                            break;
                    }
                    break;
                case ALLEGRO_EVENT_KEY_UP:
                    switch (evento.keyboard.keycode) {
                        case ALLEGRO_KEY_RIGHT:
                            entidades.jugador.fisica.velx = 0;
                            break;
                        case ALLEGRO_KEY_LEFT:
                            entidades.jugador.fisica.velx = 0;
                            break;

                    }
                    break;
        }
        pthread_mutex_unlock(&lock);
    }
        al_draw_filled_rectangle(entidades.bloques[0]->fisica.posx,entidades.bloques[0]->fisica.posy, entidades.bloques[0]->fisica.posx + entidades.bloques[0]->fisica.ancho, entidades.bloques[0]->fisica.posy + entidades.bloques[0]->fisica.alto, al_map_rgb(255,0,0));
        al_draw_filled_rectangle(entidades.bloques[1]->fisica.posx,entidades.bloques[1]->fisica.posy, entidades.bloques[1]->fisica.posx + entidades.bloques[1]->fisica.ancho, entidades.bloques[1]->fisica.posy + entidades.bloques[1]->fisica.alto, al_map_rgb(255,0,0));
        al_draw_filled_rectangle(entidades.jugador.fisica.posx,entidades.jugador.fisica.posy, entidades.jugador.fisica.posx + entidades.jugador.fisica.ancho, entidades.jugador.fisica.posy + entidades.jugador.fisica.alto, al_map_rgb(0,255,0));

        al_flip_display();
    }
    al_destroy_display(display);

    return 0;
}
