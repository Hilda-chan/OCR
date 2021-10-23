#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"
#include <stdio.h>
#include <stdlib.h>
#include "blackxwhite.h"
#include "or_logic.h"
#include "display.h"
    
int main(int argc,char *argv[])

{
    if(argc!=2)
        return EXIT_FAILURE;
    
    
    SDL_Surface* image_surface = load_image(argv[1]);
    SDL_Surface* screen_surface= display_image(image_surface);
    SDL_Surface* image_blackxwhite;
    SDL_Surface* image_filtered;
   


    image_blackxwhite = blackwhite(image_surface,screen_surface);
    image_filtered = ET(image_blackxwhite);
    image_filtered = blackborder(image_filtered);

    

    return 0;

}









































