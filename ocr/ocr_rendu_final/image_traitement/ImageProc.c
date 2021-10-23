#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL_rotozoom.h>


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

// Color suppression into gray color
void Grayscale(SDL_Surface *image_surface)
{
    int witdh;
	int height;
    Uint8 r,g,b;
	Uint8 av;
	Uint32 pixel;

	witdh = image_surface->w;
	height = image_surface->h;
    
    for(int x=0;x<witdh;x++)
	{
		for(int y=0;y<height;y++)
		{
			pixel = get_pixel(image_surface,x,y);
			SDL_GetRGB(pixel,image_surface->format,&r,&g,&b);
			av = 0.3*r + 0.59*g + 0.11*b;
			pixel = (0xFF << 24 ) | ( av << 16 ) | ( av << 8 ) | av;
			put_pixel(image_surface,x,y,pixel);
		}

	}
    SDL_SaveBMP(image_surface,"grayscale.bmp");

}

// Make image Brighter to improve contrast
void Gamma(SDL_Surface *img)
{
	Uint32 pixel;
	Uint8 r,g,b;

	int w = img->w;
	int h = img ->h;

	for(int x=0; x<w;x++)
	{
		for(int y=0; y<h; y++)
		{
			pixel = get_pixel(img,x,y);
			SDL_GetRGB(pixel,img->format,&r,&g,&b);
			pixel = SDL_MapRGB(img->format,
            255*pow((r/255.),.1),
            255*pow((g/255.),.1),
            255*pow((b/255.),.1));
			put_pixel(img,x,y,pixel);
		}
	}
	SDL_SaveBMP(img,"NewGamma.bmp");
}

// Add More Contrast to image to improve grid detection
void contrast(SDL_Surface *img)
{
	Uint32 pixel;
	Uint8 r,g,b;

	int w = img -> w;
	int h = img -> h;


	for(int i = 0; i <w; i++)
	{
		for(int j = 0; j < h; j++)
		{
			pixel = get_pixel(img,i,j);
			SDL_GetRGB(pixel,img->format,&r,&g,&b);
			if(r>=240 || b>=240 || g>=240)
                put_pixel(img, i,j,SDL_MapRGB(img -> format, 255,255,255));
			else
			    put_pixel(img, i,j,SDL_MapRGB(img -> format, 0,0,0));
		}
	}
	SDL_SaveBMP(img, "ContrastedImage.bmp");
}


void SDL_FreeSurface(SDL_Surface *surface);

int main(int argc, char* argv[])
{
	if (argc != 2)
		return -1;
    
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;

    SDL_Surface* grayscale;
    SDL_Surface* gamma;
    SDL_Surface* Contraste;
    SDL_Surface* ZoomedRotate;

    char* Name = argv[1];

    init_sdl();
    
    image_surface = load_image(Name);
    screen_surface = display_image(image_surface);
    
    wait_for_keypressed();
    
    //Convert image to grayscale to reduce noises
    Grayscale(image_surface);
    grayscale = load_image("grayscale.bmp");
    update_surface(screen_surface, grayscale); // print new image on screen
    
    wait_for_keypressed();

    // Increase Gamma to reduce noises
    Gamma(image_surface);
    gamma = load_image("NewGamma.bmp");
    update_surface(screen_surface,gamma);
    
    wait_for_keypressed();

    // Increase contrast to locate the grid easier
    contrast(image_surface);
    Contraste = load_image("ContrastedImage.bmp");
    update_surface(screen_surface,Contraste);

    wait_for_keypressed();
    
    ZoomedRotate = rotozoomSurface(screen_surface,90,1,1);
    SDL_SaveBMP(ZoomedRotate,"ZoomedRotate.bmp");
    screen_surface = display_image(ZoomedRotate);
    update_surface(screen_surface,ZoomedRotate);
    

    wait_for_keypressed();

    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);

    return 0;
}
