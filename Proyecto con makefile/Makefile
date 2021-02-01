

.SUFFIXES: .o .c 

.PHONY: TPFINAL
.PHONY: clean
#Herramientas para compilar
CC = gcc


CFLAGS =-Wall -Wextra -D MODODEJUEGO=ALLEGRO

LDLIBS = -lSDL2 -lpthread -lallegro_acodec -lallegro_audio -lallegro_color -lallegro_dialog -lallegro_image -lallegro_main -lallegro_memfile -lallegro_physfs -lallegro_primitives -lallegro_ttf -lallegro_font -lallegro

OBJRASPI = disdrv.h disdrv.o joydrv.h joydrv.o

OBJALLEGRO = main.o IEvents.o fisica.o entidades.o data.o menu.o allegroLib.o level.o gamelogic.o render.o animacion.o times.o audio.o

main: $(OBJALLEGRO) #$(OBJRASPI)
main.o: main.c menu.h #matiasBrosGame.h
IEvents.o: IEvents.c IEvents.h# matiasBrosGame.h allegro.h
fisica.o: fisica.c fisica.h# gamelogic.h
entidades.o: entidades.h entidades.c# data.h
data.o: data.c data.h# configuracion.h
menu.o: menu.c menu.h# render.h
allegroLib.o: allegroLib.c allegroLib.h# matiasBrosGame.h allegro.h
level.o: level.c level.h# render.h
gamelogic.o: gamelogic.c gamelogic.h# matiasBrosGame.h allegro.h menu.h animacion.h level.h
render.o: render.c render.h# matiasBrosGame.h allegro.h fisica.h
animacion.o: animacion.c animacion.h# times.h
times.o: times.c times.h# matiasBrosGame.h
audio.o: audio.c audio.h

clean:
	$(RM) $(OBJALLEGRO) main





