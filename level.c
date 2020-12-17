//
// Created by gonzalo on 27/11/20.
//

#include "level.h"
#include "data.h"
#include "entidades.h"
#include <stdlib.h>
#include "allegro.h"
#include "allegroLib.h"

#define TOWORLDPOS(v) ( (v) * PIXELSPERUNIT)

static int countColumns(level_t* level, FILE* mapData);

int cargarMapa(level_t* level , int id) {

    FILE *mapData;
    int i = 0;
    int j = 0;
    char read;
    int auxCont;
    int borderCount = 0;


    int error = openLevelData(&mapData, id);

    if (error != 1){
        countColumns(level, mapData);
        level->level = (int **) calloc( level->levelHeight, sizeof(int *));
        for (i = 0; i < level->levelHeight; i++) {
            (level->level)[i] = (int *) malloc(level->levelWidht * sizeof(int));
        }

        i = 0;
        do {
            read = fgetc(mapData);

            switch (read) {
                case FASTCHEEPCHEEP:
                case SLOWCHEEPCHEEP:
                case PULPITO:
                case ALGA:
                case LADRILLO:
                case JUGADOR:
                case NADA:
                    level->level[i][j] = read;
                    j++;
                    auxCont = 0;
                    break;
                case ';':
                    auxCont++;
                    break;
                case BORDE:
                    borderCount++;
                    if (borderCount == 2){
                        i++;
                        j = 0;
                        borderCount = 0;
                    }
                    auxCont = 0;
                    break;
                default:
                    break;

            }
            if (auxCont == 2){
                level->level[i][j] = NADA;
                j++;
                auxCont--;
            }
        }while (read != EOF);
    }
    else{
        printf("Error al cargar el mapa\n");
    }

    fclose(mapData);
    return error;
}


static int countColumns(level_t* level, FILE* mapData){

    int error = 0;
    int colNum = 0;
    char read = 0;
    int borderCount = 0;
    int auxCont;

    do {
        read = fgetc(mapData);

        switch (read) {
            case FASTCHEEPCHEEP:
            case SLOWCHEEPCHEEP:
            case PULPITO:
            case ALGA:
            case LADRILLO:
            case JUGADOR:
            case NADA:
                colNum++;
                auxCont = 0;
                break;
            case ';':
                auxCont++;
                break;
            case BORDE:
                borderCount++;
                auxCont = 0;
                break;
            default:
                break;

        }

        if (auxCont == 2){
            colNum++;
            auxCont--;
        }

    }while (read != EOF);

    if (borderCount%2 == 1){
        error = 1;
    } else{
        level->levelHeight = borderCount/2;
        level->levelWidht = colNum/level->levelHeight;
    }
    fseek( mapData, 0, SEEK_SET );
    return error;
}

void drawLevel(estadoJuego_t *gameState){

    bufferRecursos_t *resourceBuffer = &gameState->buffer;
    int flip_player = 0;
    int i = 0;
    al_clear_to_color(al_map_rgb(76, 93, 122));

    fisica_t jugador = gameState->entidades.jugador.fisica;

    if(jugador.velx >= 0) {
        flip_player = 0;
    }
    else {
        flip_player = ALLEGRO_FLIP_HORIZONTAL;
    }
    al_draw_scaled_bitmap(resourceBuffer->image[MATIASIDLESPRITE], 0, 0, al_get_bitmap_width(resourceBuffer->image[MATIASIDLESPRITE]),  al_get_bitmap_height(resourceBuffer->image[MATIASIDLESPRITE]),
                          jugador.posx, jugador.posy, jugador.ancho, jugador.alto, flip_player);

    al_draw_bitmap(resourceBuffer->image[WAVESPRITE], 0, PIXELSPERUNIT, 0);


    //Mientras no se hayan leido todos los bloques, dibujamos el siguiente
    bloque_t bloque;
    while(gameState->entidades.bloques[i].identificador != NULLENTITIE){
        bloque = gameState->entidades.bloques[i];
        switch (bloque.identificador){
            case ALGA:
                al_draw_scaled_bitmap(resourceBuffer->image[ALGASPRITE1], 0, 0, al_get_bitmap_width(resourceBuffer->image[ALGASPRITE1]), al_get_bitmap_height(resourceBuffer->image[ALGASPRITE1]), bloque.fisica.posx, bloque.fisica.posy,
                                      bloque.fisica.ancho, bloque.fisica.alto, 0);
                break;

            case LADRILLO:
                for (int j = 0; j < bloque.fisica.ancho/PIXELSPERUNIT; j++) {
                    al_draw_scaled_bitmap(resourceBuffer->image[PISOSPRITE], 0, 0, al_get_bitmap_width(resourceBuffer->image[PISOSPRITE]),
                                          al_get_bitmap_height(resourceBuffer->image[PISOSPRITE]), bloque.fisica.posx + j * PIXELSPERUNIT,
                                          bloque.fisica.posy,PIXELSPERUNIT, PIXELSPERUNIT, 0);
                }
                break;
        }

        i++;
    }

    i = 0;
    while(gameState->entidades.enemigos[i].identificador != NULLENTITIE){
        enemigo_t enemigo = gameState->entidades.enemigos[i];
        switch (enemigo.identificador){
            case PULPITO:
                al_draw_scaled_bitmap(resourceBuffer->image[BLOOPERSPRITE1 + enemigo.sprite], 0, 0, al_get_bitmap_width(resourceBuffer->image[BLOOPERSPRITE1]),  al_get_bitmap_height(resourceBuffer->image[BLOOPERSPRITE1]), enemigo.fisica.posx, enemigo.fisica.posy,
                                      enemigo.fisica.ancho, enemigo.fisica.alto, 0);
                break;

            case FASTCHEEPCHEEP:
                al_draw_scaled_bitmap(resourceBuffer->image[CHEEPCHEEPSPRITE1 + enemigo.sprite], 0, 0, al_get_bitmap_width(resourceBuffer->image[CHEEPCHEEPSPRITE1]),  al_get_bitmap_height(resourceBuffer->image[CHEEPCHEEPSPRITE1]), enemigo.fisica.posx, enemigo.fisica.posy,
                                      enemigo.fisica.ancho, enemigo.fisica.alto, 0);
                break;

            case SLOWCHEEPCHEEP:
                al_draw_scaled_bitmap(resourceBuffer->image[CHEEPCHEEPSSLOWSPRITE1 + enemigo.sprite], 0, 0, al_get_bitmap_width(resourceBuffer->image[CHEEPCHEEPSSLOWSPRITE1]),  al_get_bitmap_height(resourceBuffer->image[CHEEPCHEEPSSLOWSPRITE1]), enemigo.fisica.posx, enemigo.fisica.posy,
                                      enemigo.fisica.ancho, enemigo.fisica.alto, 0);
                break;
        }

        i++;
    }

    al_flip_display();
}

void drawUI(estadoJuego_t* gameState){

    //score
    al_draw_text(gameState->buffer.font[SUPERMARIOFONT50], al_map_rgb(255, 255, 255), 50, 50, 0, "Matias");

    //coins
    al_draw_text(gameState->buffer.font[SUPERMARIOFONT50], al_map_rgb(255, 255, 255), 200, 50, 0, " x ");

    //world - level
    al_draw_text(gameState->buffer.font[SUPERMARIOFONT50], al_map_rgb(255, 255, 255), 400, 50, 0, " - ");

    //timer
    al_draw_text(gameState->buffer.font[SUPERMARIOFONT50], al_map_rgb(255, 255, 255), 500, 50, 0, " time ");

    al_flip_display();
}

void initUI(gameUI_t* gameUI){
    gameUI->time = MAXLEVELTIME;
    gameUI->score = 0;
    gameUI->coins = 0;
    gameUI->level = 1;
    gameUI->world = 1;
}

int initEntities(estadoJuego_t* gameState){

    int blocksCounter = 0;
    int enemiesCounter = 0;
    int blocksIndex = 0;
    int enemiesIndex = 0;
    int horizontalBlocksCounter = 0;
    char currentBlock;

    //Calculamos la cantidad de enemigos y de bloques que hay en el mapa
    for(int i = 0; i < gameState->level.levelHeight; i++){
        for(int j = 0; j < gameState->level.levelWidht; j++) {
            switch(gameState->level.level[i][j]) {
                case LADRILLO:
                case ALGA:
                    blocksCounter++;
                    break;

                case FASTCHEEPCHEEP:
                case SLOWCHEEPCHEEP:
                case PULPITO:
                    enemiesCounter++;
                    break;
            }
        }
    }

    //Reservamos el espacio para los bloques
    gameState->entidades.bloques = (bloque_t*) malloc(sizeof(bloque_t) * blocksCounter);
    if(gameState->entidades.bloques == NULL){
        printf("Error al reservar espacio para los bloques");
        return 1;
    }


    //Reservamos el espacio para los enemigos
    gameState->entidades.enemigos = (enemigo_t*) malloc(sizeof(enemigo_t) * (enemiesCounter+1));
    if(gameState->entidades.enemigos == NULL){
        printf("Error al reservar espacio para los enemigos");
        return 1;
    }
    gameState->entidades.enemigos[enemiesCounter].identificador = NULLENTITIE;

    for(int i = 0; i < gameState->level.levelHeight; i++){
        for(int j = 0; j < gameState->level.levelWidht; j++){

            //Agarramos el siguiente bloque
            currentBlock = gameState->level.level[i][j];

            //Si el contador de bloques adyacentes esta activo y el siguiente bloque no es un ladrillo
            if (horizontalBlocksCounter == 1 && currentBlock != LADRILLO){
                blocksIndex++;                  //Avanzamos el indice de bloques al siguiente
                horizontalBlocksCounter = 0;    //Terminamos de contar el grupo de bloques actuales
            }

            switch (currentBlock) {
                case LADRILLO:
                    if (horizontalBlocksCounter == 0){
                        gameState->entidades.bloques[blocksIndex].sprite = 0;
                        gameState->entidades.bloques[blocksIndex].identificador = LADRILLO;
                        gameState->entidades.bloques[blocksIndex].fisica.posx = TOWORLDPOS(j);
                        gameState->entidades.bloques[blocksIndex].fisica.posy = TOWORLDPOS(i);
                        gameState->entidades.bloques[blocksIndex].fisica.ancho = PIXELSPERUNIT;
                        gameState->entidades.bloques[blocksIndex].fisica.alto = PIXELSPERUNIT;
                        gameState->entidades.bloques[blocksIndex].fisica.velx = 0;
                        gameState->entidades.bloques[blocksIndex].fisica.vely = 0;
                        horizontalBlocksCounter = 1;
                    }
                    else{
                        gameState->entidades.bloques[blocksIndex].fisica.ancho += PIXELSPERUNIT;
                        blocksCounter--;        //Cada vez que unimos un bloque, disminuimos la cantidad de bloques totales
                    }
                    break;

                case JUGADOR:
                    gameState->entidades.jugador.vidas = 3;
                    gameState->entidades.jugador.sobreBloque = 0;
                    gameState->entidades.jugador.estado = ALIVE;                                //VIVO?
                    gameState->entidades.jugador.sprite = 0;
                    gameState->entidades.jugador.fisica.posx = TOWORLDPOS(j);
                    gameState->entidades.jugador.fisica.posy = TOWORLDPOS(i);
                    gameState->entidades.jugador.fisica.ancho = PIXELSPERUNIT;
                    gameState->entidades.jugador.fisica.alto = PIXELSPERUNIT;
                    gameState->entidades.jugador.fisica.velx = 0;
                    gameState->entidades.jugador.fisica.vely = 0;
                    break;

                case FASTCHEEPCHEEP:
                    gameState->entidades.enemigos[enemiesIndex].sprite = 0;
                    gameState->entidades.enemigos[enemiesIndex].estado = ALIVE;
                    gameState->entidades.enemigos[enemiesIndex].identificador = FASTCHEEPCHEEP;
                    gameState->entidades.enemigos[enemiesIndex].fisica.posx = TOWORLDPOS(j);
                    gameState->entidades.enemigos[enemiesIndex].fisica.posy = TOWORLDPOS(i);
                    gameState->entidades.enemigos[enemiesIndex].fisica.ancho = PIXELSPERUNIT;
                    gameState->entidades.enemigos[enemiesIndex].fisica.alto = PIXELSPERUNIT;
                    gameState->entidades.enemigos[enemiesIndex].fisica.velx = 0 ;             //Le puse una velocidad al cheep cheep para la izquierda
                    gameState->entidades.enemigos[enemiesIndex].fisica.vely = 0;
                    gameState->entidades.enemigos[enemiesIndex].funcionMovimiento = cheepcheep;
                    startEnemy(&(gameState->entidades.enemigos[enemiesIndex]));
                    enemiesIndex++;
                    break;

                case SLOWCHEEPCHEEP:
                    gameState->entidades.enemigos[enemiesIndex].sprite = 0;
                    gameState->entidades.enemigos[enemiesIndex].estado = ALIVE;
                    gameState->entidades.enemigos[enemiesIndex].identificador = SLOWCHEEPCHEEP;
                    gameState->entidades.enemigos[enemiesIndex].fisica.posx = TOWORLDPOS(j);
                    gameState->entidades.enemigos[enemiesIndex].fisica.posy = TOWORLDPOS(i);
                    gameState->entidades.enemigos[enemiesIndex].fisica.ancho = PIXELSPERUNIT;
                    gameState->entidades.enemigos[enemiesIndex].fisica.alto = PIXELSPERUNIT;
                    gameState->entidades.enemigos[enemiesIndex].fisica.velx = 0 ;             //Le puse una velocidad al cheep cheep para la izquierda
                    gameState->entidades.enemigos[enemiesIndex].fisica.vely = 0;
                    gameState->entidades.enemigos[enemiesIndex].funcionMovimiento = cheepcheep;
                    startEnemy(&(gameState->entidades.enemigos[enemiesIndex]));
                    enemiesIndex++;
                    break;

                case PULPITO:
                    gameState->entidades.enemigos[enemiesIndex].sprite = 0;
                    gameState->entidades.enemigos[enemiesIndex].estado = ALIVE;
                    gameState->entidades.enemigos[enemiesIndex].identificador = PULPITO;
                    gameState->entidades.enemigos[enemiesIndex].fisica.posx = TOWORLDPOS(j);
                    gameState->entidades.enemigos[enemiesIndex].fisica.posy = TOWORLDPOS(i);
                    gameState->entidades.enemigos[enemiesIndex].fisica.ancho = PIXELSPERUNIT;
                    gameState->entidades.enemigos[enemiesIndex].fisica.alto = PIXELSPERUNIT * 2 ;
                    gameState->entidades.enemigos[enemiesIndex].fisica.velx = 0;             //Le puse una velocidad al blooper para la izquierda
                    gameState->entidades.enemigos[enemiesIndex].fisica.vely = 0;
                    gameState->entidades.enemigos[enemiesIndex].funcionMovimiento = blooper;
                    startEnemy(&(gameState->entidades.enemigos[enemiesIndex]));
                    enemiesIndex++;
                    break;

                case ALGA:
                    gameState->entidades.bloques[blocksIndex].sprite = 0;
                    gameState->entidades.bloques[blocksIndex].identificador = ALGA;
                    gameState->entidades.bloques[blocksIndex].fisica.posx = TOWORLDPOS(j);
                    gameState->entidades.bloques[blocksIndex].fisica.posy = TOWORLDPOS(i-2);
                    gameState->entidades.bloques[blocksIndex].fisica.ancho = PIXELSPERUNIT;
                    gameState->entidades.bloques[blocksIndex].fisica.alto = PIXELSPERUNIT * 3;
                    gameState->entidades.bloques[blocksIndex].fisica.velx = 0;
                    gameState->entidades.bloques[blocksIndex].fisica.vely = 0;
                    blocksIndex++;
                    break;
            }
        }
        horizontalBlocksCounter = 0;  //Volvemos a iniciar el contador de bloques horizontal cuando pasamos a otra fila del mapa
    }

    //Reallocamos el espacio para los bloques
    gameState->entidades.bloques = (bloque_t*) realloc(gameState->entidades.bloques , sizeof(bloque_t) * (blocksCounter+1));
    if(gameState->entidades.bloques == NULL){
        printf("Error al reallocar espacio para los bloques");
        return 1;
    }
    gameState->entidades.bloques[blocksCounter].identificador = NULLENTITIE;         //Inicializamos el ultimo elemento en nulo

    return 0;
}