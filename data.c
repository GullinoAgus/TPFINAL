//
// Created by gonzalo on 23/11/20.
//

#include "data.h"
#include <stdio.h>

enum ficheros{MENU, JUEGO};

//const char** data = {"/data/menuData", "otra cosa :v"};

typedef struct{
    int x;
    int y;
    int scale;
}dataMenu_t;

/*dataMenu_t* loadMenuData(){
    FILE* file = fopen(data[MENU]);
    int quant = 0;  //Almacena la cantidad de lineas que tiene el archivo
    int aux; //Variable para guardar temporalmente los valores de la data

    while(){
        fgets(file, "%d %d %d", aux, aux, aux);
        quant++;
    }
}
*/