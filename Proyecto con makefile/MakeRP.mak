

.SUFFIXES: .o .c 

.PHONY: TPFINAL
.PHONY: clean
#Herramientas para compilar
CC = gcc


CFLAGS =-Wall -Wextra -D MODOJUEGO=RASPI

LDLIBS = -lSDL2 -lpthread

OBJRASPI = disdrv.o joydrv.o

OBJALLEGRO = main.o IEvents.o fisica.o entidades.o data.o menu.o allegroLib.o level.o gamelogic.o render.o animacion.o times.o

main: $(OBJALLEGRO) $(OBJRASPI) audio.o
main.o: main.c menu.h matiasBrosGame.h raspi.h configuracion.h
IEvents.o: IEvents.c IEvents.h matiasBrosGame.h raspi.h configuracion.h
fisica.o: fisica.c fisica.h gamelogic.h matiasBrosGame.h raspi.h configuracion.h
entidades.o: entidades.h entidades.c data.h matiasBrosGame.h raspi.h configuracion.h
data.o: data.c data.h matiasBrosGame.h raspi.h configuracion.h
menu.o: menu.c menu.h render.h matiasBrosGame.h raspi.h configuracion.h
allegroLib.o: allegroLib.c allegroLib.h matiasBrosGame.h raspi.h configuracion.h
level.o: level.c level.h render.h matiasBrosGame.h raspi.h configuracion.h
gamelogic.o: gamelogic.c gamelogic.h menu.h animacion.h level.h matiasBrosGame.h raspi.h configuracion.h
render.o: render.c render.h fisica.h matiasBrosGame.h raspi.h configuracion.h
animacion.o: animacion.c animacion.h times.h matiasBrosGame.h raspi.h configuracion.h
times.o: times.c times.h matiasBrosGame.h raspi.h matiasBrosGame.h raspi.h configuracion.h

clean:
	$(RM) $(OBJALLEGRO)





