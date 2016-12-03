# PROGRAM = glideThroughSky
CC      = gcc
CFLAGS  = -g -Wall -I/usr/X11R6/include -I/usr/pkg/include
LDFLAGS = -L/usr/X11R6/lib -L/usr/pkg/lib
LDLIBS  = -lglut -lGLU -lGL -lm

glideThroughSky: main.o world.o player.o wall.o mana.o
	$(CC) $(LDFLAGS) -o glideThroughSky.out main.o world.o player.o wall.o mana.o  $(LDLIBS)

main.o:
	$(CC) $(LDFLAGS) -c src/main.c $(LDLIBS)

world.o:
	$(CC) $(LDFLAGS) -c src/world.c $(LDLIBS)

player.o:
	$(CC) $(LDFLAGS) -c src/player.c $(LDLIBS)

wall.o:
	$(CC) $(LDFLAGS) -c src/wall.c $(LDLIBS)

mana.o:
	$(CC) $(LDFLAGS) -c src/mana.c $(LDLIBS)

.PHONY: clean dist

clean:
	-rm *.o

dist: clean
	-tar -chvj -C .. -f ../$(PROGRAM).tar.bz2 $(PROGRAM)
