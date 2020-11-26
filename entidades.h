//
// Created by agus on 23/11/20.
//

#ifndef TPFINAL_ENTIDADES_H
#define TPFINAL_ENTIDADES_H


#define CHEEPCHEEP 1
#define PULPO 2
#define LADRILLO 1
#define ALGA 2

typedef struct {
    float posx;
    float posy;
    int ancho;
    int alto;
    float velx;
    float vely;
} fisica_t;

typedef struct {

    fisica_t fisica;
    int vidas;
    int sobreBloque;
    int tamanio;

} jugador_t;

typedef struct {

    fisica_t fisica;
    int identificador;

} enemigo_t;

typedef struct {

    fisica_t fisica;
    int identificador;

} bloque_t;

typedef struct {
    jugador_t jugador;
    enemigo_t **enemigos;
    bloque_t **bloques;
} entidades_t;





#endif //TPFINAL_ENTIDADES_H
