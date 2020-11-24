cmake_minimum_required(VERSION 3.17)
project(TPFINAL C)

set(CMAKE_C_STANDARD 11)

add_executable(TPFINAL main.c fisica.c fisica.h entidades.h entidad.c matiasBrosGame.h)
add_executable(TPFINAL main.c data.c data.h menu.c menu.h configuracion.h allegroLib.c allegroLib.h)