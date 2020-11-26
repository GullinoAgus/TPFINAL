cmake_minimum_required(VERSION 3.17)
project(TPFINAL C)

set(CMAKE_C_STANDARD 11)

add_executable(TPFINAL main.c fisica.c fisica.h entidades.h entidades.c)

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
        pthread)