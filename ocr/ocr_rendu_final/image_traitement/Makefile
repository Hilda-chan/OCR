# SDL Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -g -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image -lm

all: ImageProc

ImageProc: pixel_operations.o ImageProc.o

ImageProc.o: pixel_operations.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} ImageProc 
# END
