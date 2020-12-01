//
// Created by agus on 23/11/20.
//

#ifndef TPFINAL_FISICA_H
#define TPFINAL_FISICA_H


#define VELOCIDADXMAX 2
#define VELOCIDADYMAX 5
#define GRAVEDAD 0.01f
#define UTIEMPOREFRESCO 3000

typedef struct {
    float posx;
    float posy;
    int ancho;
    int alto;
    float velx;
    float vely;
} fisica_t;

_Noreturn void* fisica(void* entrada);



#endif //TPFINAL_FISICA_H
