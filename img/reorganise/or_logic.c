#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"
#include <stdio.h>
#include <stdlib.h>
#include "display.h"
#include <dirent.h>

SDL_Surface* ET(char *img,char *img_ref)
{
	//creer un array avec le nom des images ref
	//creer un array avec le pourcentage de noir entre l'image test et la ref
	//parcourir l'array des images ref et appliquer la fonction sur chaque image du tableau
	//renvoyer un array avec le pourcentage
	SDL_Surface* image_surface;
	SDL_Surface* screen_surface;
    SDL_Surface* image_surface_ref;
	
    int witdh;
	int height;

	init_sdl();
	image_surface = load_image(img);
	screen_surface = display_image(image_surface);
    image_surface_ref = load_image(img_ref);

	witdh = image_surface->w;
	height = image_surface->h;
	
	Uint32 pixel;
    Uint32 pixel_ref;
    Uint32 black = 0x000000;
    Uint32 white = 0xffffff;

    for(int x=0;x<witdh;x++)
	{
		for(int y=0;y<height;y++)
		{
			pixel = get_pixel(image_surface,x,y);
            pixel_ref = get_pixel(image_surface_ref,x,y);
            if(pixel != pixel_ref)
                pixel = white;
            else 
                pixel = black;
			put_pixel(image_surface,x,y,pixel);
		}
	}

	update_surface(screen_surface,image_surface);
	SDL_FreeSurface(image_surface);
	SDL_FreeSurface(screen_surface);
    
    return image_surface;
}

float percentage(char *img)
{
    SDL_Surface* image_surface;

    float witdh;
	float height;

	init_sdl();
	image_surface = load_image(img);

	witdh = image_surface->w;
	height = image_surface->h;

	Uint32 pixel;
    Uint32 black = 0x000000;
    float nb_black_pixel = 0;
    float nb_pixel = witdh * height;

	for(int x=0;x<witdh;x++)
	{
		for(int y=0;y<height;y++)
		{
			pixel = get_pixel(image_surface,x,y);
			if(pixel == black)
				nb_black_pixel++;
		}
    }
    return nb_black_pixel/nb_pixel;
}
