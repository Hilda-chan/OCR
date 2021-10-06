#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"
#include <stdio.h>

void init_sdl()
{
    // Init only the video part.
    // If it fails, die with an error message.
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}
SDL_Surface* load_image(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}
SDL_Surface* display_image(SDL_Surface *img)
{
    SDL_Surface *screen;

    // Set the window to the same size as the image
    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
    if (screen == NULL)
    {
        // error management
        errx(1, "Couldn't set %dx%d video mode: %s\n",
                img->w, img->h, SDL_GetError());
    }

    // Blit onto the screen surface
    if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    // Update the screen
    SDL_UpdateRect(screen, 0, 0, img->w, img->h);

    // return the screen for further uses
    return screen;
}
void wait_for_keypressed()
{
    SDL_Event event;

    // Wait for a key to be down.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYDOWN);

    // Wait for a key to be up.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYUP);
}
void SDL_FreeSurface(SDL_Surface *surface);


int main(int argc, char *argv[])
{
	if(argc!=3)
		exit(EXIT_FAILURE);
	SDL_Surface* image_surface;
	SDL_Surface* screen_surface;

    SDL_Surface* image_surface_ref;
	
    
    
    int witdh;
	int height;



	init_sdl();
	image_surface = load_image(argv[1]);
	screen_surface = display_image(image_surface);

    image_surface_ref = load_image(argv[2]);

	witdh = image_surface->w;
	height = image_surface->h;
	
	
	
	wait_for_keypressed();
	
	Uint32 pixel;
    Uint32 pixel_ref;
    Uint32 black = 0x000000;
    Uint32 white = 0xffffff;
    Uint8 r,g,b;
    Uint8 av;
    int nb_black_pixel = 0;
    int nb_pixel = witdh * height;

	for(int x=0;x<witdh;x++)
	{
		for(int y=0;y<height;y++)
		{
			pixel = get_pixel(image_surface,x,y);
			SDL_GetRGB(pixel,image_surface->format,&r,&g,&b);
			av = 0.299*r + 0.59*g + 0.11*b;
			if(av>160)
				pixel = white;
			else pixel = black;
				//pixel = (0xFF << 24 ) | ( av << 16 ) | ( av << 8 ) | av;
		       	put_pixel(image_surface,x,y,pixel);
		}
    }
	
    update_surface(screen_surface,image_surface);
	//wait_for_keypressed();

    for(int x=0;x<witdh;x++)
	{
		for(int y=0;y<height;y++)
		{
			pixel = get_pixel(image_surface,x,y);
            pixel_ref = get_pixel(image_surface_ref,x,y);
            if(pixel != pixel_ref /*|| pixel == pixel_ref && pixel == white*/)
                pixel = white;
            else 
            {
                pixel = black;
                nb_black_pixel++;
            }
            /*else if (pixel == pixel_ref && pixel_ref == black)
                pixel = black;*/
				//pixel = (0xFF << 24 ) | ( av << 16 ) | ( av << 8 ) | av;
			put_pixel(image_surface,x,y,pixel);
		}

	}

	update_surface(screen_surface,image_surface);


	wait_for_keypressed();

	SDL_FreeSurface(image_surface);
	SDL_FreeSurface(screen_surface);

    
    return nb_black_pixel/nb_pixel;
}
