//
// Created by agus on 23/11/20.
//

#ifndef TPFINAL_FISICA_H
#define TPFINAL_FISICA_H


#define VELOCIDADXMAX 2
#define VELOCIDADYMAX 5
#define GRAVEDAD 0.01f
#define INERCIA 1.0f

typedef struct {
    float posx;
    float posy;
    int ancho;
    int alto;
    float velx;
    float vely;
} fisica_t;


void* fisica(void* entrada);
int isColliding(fisica_t* object1, fisica_t* object2);


#endif //TPFINAL_FISICA_H
