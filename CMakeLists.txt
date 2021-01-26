cmake_minimum_required(VERSION 3.17)
project(TPFINAL C)

set(CMAKE_C_STANDARD 11)

add_executable(TPFINAL main.c IEvents.c IEvents.h fisica.c fisica.h entidades.h entidades.c matiasBrosGame.h data.c data.h menu.c menu.h configuracion.h allegroLib.c allegroLib.h level.c level.h allegro.h raspi.h gamelogic.c gamelogic.h render.c render.h animacion.c animacion.h times.c times.h audio.c audio.h disdrv.h disdrv.o joydrv.h joydrv.o)


TARGET_LINK_LIBRARIES(TPFINAL  allegro_acodec
                                allegro_audio
                                allegro_color
                                allegro_dialog
                                allegro_image
                                allegro_main
                                allegro_memfile
                                allegro_physfs
                                allegro_primitives
                                allegro_ttf
                                allegro_font
                                allegro
                                SDL2
                                pthread
                               )