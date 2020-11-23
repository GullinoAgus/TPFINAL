//
// Created by agus on 23/11/20.
//

#ifndef TPFINAL_ENTIDADES_H
#define TPFINAL_ENTIDADES_H

#define JUGADOR 0
#define ENEMIGO 1
#define LADRILLO 2
#define ALGA 3

typedef struct {
    int posx;
    int posy;
    int ancho;
    int alto;
    int velx;
    int vely;
} fisica_t;

typedef struct {

} jugador_t;

typedef struct {

} enemigo_t;

typedef struct {

} bloque_t;

typedef struct {
    int identificador;
    fisica_t fisica;
    void* propiedades;
} entidad_t;





#endif //TPFINAL_ENTIDADES_H
