#include "SDL/SDL.h"
#include <stdlib.h>
#include <stdio.h>
#include "pixel_operations.h"
#include <string.h>


void save_cells(SDL_Surface* img){
    int w = img -> w;
    int h = img -> h;
    int step_w = w / 9;
    for(int y = 0; y < 9; y++)
    {
        for(int x = 0; x < 9; x++)
        {
            int x1 = x*step_w;
            int x2 = (x+1)*step_w;
            SDL_Surface* cell = Zoom(img, x1, x2, x1, y*step_h, (y+1)*step_h);
            int nb = x + 9*y;
            char* path = (char*)malloc(15 * sizeof(char) + 1);
	    *path = '\0';
	    strcat(path, "img/cell_");
	    char s[2];
	    s[0] = nb % 10;
	    s[1] = nb / 10;
	    strcat(path, s);
	    strcat(path, ".bmp");
            SDL_save(cell,path);
}
