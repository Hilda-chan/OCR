# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image

all: main
main: pixel_operations.o blackxwhite.o or_logic.o display.o

blackxwhite.o: pixel_operations.h blackxwhite.h display.h

or_logic.o: pixel_operations.h or_logic.h display.h

display.o: display.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} main

# END
