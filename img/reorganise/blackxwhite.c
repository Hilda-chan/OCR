#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"
#include "display.h"

SDL_Surface* blackwhite(SDL_Surface* image_surface,SDL_Surface* screen_surface)
{
	//SDL_Surface* image_surface;
	//SDL_Surface* screen_surface;
	int witdh;
	int height;

	

//	init_sdl();
//	image_surface = load_image(img);
//	screen_surface = display_image(image_surface);
	witdh = image_surface->w;
	height = image_surface->h;

    Uint8 r,g,b;
	Uint8 av;
	Uint32 pixel;

	for(int x=0;x<witdh;x++)

{
		for(int y=0;y<height;y++)

{
			pixel = get_pixel(image_surface,x,y);
			SDL_GetRGB(pixel,image_surface->format,&r,&g,&b);
			av = 0.299*r + 0.59*g + 0.11*b;
			if(av>100)
				pixel = (0x000000);
			else pixel = (0xFFFFFF);
		    put_pixel(image_surface,x,y,pixel);
		}
	}

	wait_for_keypressed();
	update_surface(screen_surface,image_surface);
	SDL_FreeSurface(image_surface);
	SDL_FreeSurface(screen_surface);
	wait_for_keypressed();

    return image_surface;
}

SDL_Surface* blackborder(SDL_Surface* image_surface,SDL_Surface* screen_surface)
{
    int witdh;
    int height;
	witdh = image_surface->w;
	height = image_surface->h;

	Uint32 pixel;

	for(int x=0;x<witdh;x++)
{
    	for(int y=0;y<height;y++)
        {
            if(x==0 || y==0)
            {
                pixel = (0xff << 24) | (0x00) << 16 | (0x00 < 8) | 0xff;
                put_pixel(image_surface,x,y,pixel);
            }
        }
}

	wait_for_keypressed();
	update_surface(screen_surface,image_surface);
	SDL_FreeSurface(image_surface);
	SDL_FreeSurface(screen_surface);
	wait_for_keypressed();

    return image_surface;
}

