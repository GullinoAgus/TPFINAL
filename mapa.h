//
// Created by agus on 27/11/20.
//

#ifndef TPFINAL_MAPA_H
#define TPFINAL_MAPA_H

#define JUGADOR 'J'
#define BORDE 'B'
#define NADA '0'
#define LADRILLO '1'
#define ALGA '2'
#define CHEEPCHEEP '3'
#define PULPITO '4'

int* leerNivel(const char* direccion);
int verificarTamanioDeNivel(const char* direccion);

#endif //TPFINAL_MAPA_H
