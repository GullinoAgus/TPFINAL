//
// Created by agus on 23/11/20.
//

#ifndef TPFINAL_FISICA_H
#define TPFINAL_FISICA_H


#define VELOCIDADXMAX (0.25f * (1.0f/(FPS*4)) * 1000)
#define VELOCIDADYMAX (0.5f * (1.0f/(FPS*4)) * 1000)
#define GRAVEDAD 0.009f
#define INERCIA 0.975f

typedef struct {
    float posx;
    float posy;
    int ancho;
    int alto;
    float velx;
    float vely;
} fisica_t;


void* fisica(void* entrada);
void movePlayer(int direction, void* player);

#endif //TPFINAL_FISICA_H
