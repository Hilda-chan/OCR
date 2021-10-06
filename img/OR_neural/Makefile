# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image

all: or_logic

or_logic: pixel_operations.o or_logic.o

or_logic.o: pixel_operations.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} or_logic

# END
