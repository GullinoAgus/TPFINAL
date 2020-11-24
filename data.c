//
// Created by gonzalo on 23/11/20.
//

#include "data.h"
#include <stdio.h>

enum ficheros{MENU, ESTADOJUEGO};
const char** data = {"/data/menuData.txt", "/data/estadoJuegoData.txt"};

typedef struct{
    int x;
    int y;
    int scale;
}dataMenu_t;


dataMenu_t loadMenuData(){
    FILE* file = fopen(data[MENU]);

    while(){
        fgets(file, "%d %d %d", aux, aux, aux);
        quant++;
    }
}


int loadGameState(estadoJuego_t *gameState){
    FILE* file = fopen(data[ESTADOJUEGO]);
    if(file == NULL){
        return 1;
    }
    else{
        int maxEntries = fscanf(file, "%d");
        for(int i = 0; i < maxEntries; i++){
            fscanf(file, "%d %s", gameState.bestScores[i], gameState.bestScoresName[i]);
        }
    }
}
