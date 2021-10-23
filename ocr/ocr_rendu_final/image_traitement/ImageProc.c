#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"
#include <math.h>
#include <stdlib.h>
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
			av = 0.299*r + 0.59*g + 0.11*b;
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

// Allow user to rotate image with given parameter
void rotateMAN(int angle, SDL_Surface *img)
{
	SDL_LockSurface(img);

	Uint32 pixel;
	Uint8 r,g,b;

	int w = img->w;
	int h = img->h;
	int mid = (w+h)/2;
    int xs;
    int ys;

	double sinAngle = sin(M_PI * angle / 180.0);
	double cosAngle = cos(M_PI * angle / 180.0);


	SDL_Surface* result = SDL_CreateRGBSurface(0, w+h, w+h, 32,0,0,0,0);
	for(int x = 0; x < img->w;x++)
	{
		for(int y = 0; y < img->h;y++)
		{
			pixel = get_pixel(img,x,y);
			SDL_GetRGB(pixel, img-> format, &r,&g,&b);
			pixel = SDL_MapRGB(img -> format,r,g,b);
			xs = cosAngle*(x-w/2)-sinAngle*(y-h/2)+mid;
			ys = sinAngle*(x-w/2)+cosAngle*(y-h/2)+mid;
			put_pixel(result,abs(xs),abs(ys),pixel);
		}
	}

	SDL_SaveBMP(result,"RotatedImage.bmp");
	//SDL_UnlockSurface(img);
	//*img = *result;
}


// Zoom into the grid into the grid only
SDL_Surface* Zoom(SDL_Surface *img, int x1,int x2, int x3, int y1, int y4)
{
	Uint8 r,g,b;
	Uint32 pixel;

	SDL_Surface* result;

    result = SDL_CreateRGBSurface(0,abs(x2-x3),abs(y1-y4),32,0,0,0,0);

	for(int y = 0; y < abs(y1-y4); y++)
	{
		for(int x = 0; x < abs(x2-x3); x++)
		{
			pixel = get_pixel(img,x+x1,y+y1);
			SDL_GetRGB(pixel,img -> format, &r,&g,&b);
			pixel = SDL_MapRGB(img->format,r,g,b);
			put_pixel(result,x,y,pixel);
		}
	}
	SDL_SaveBMP(result,"GridOnly.bmp");
	return result;
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

    SDL_Surface* Zoomed;
    SDL_Surface* Rotated;

    char* Name = argv[1];


    init_sdl();
    
    image_surface = load_image(Name);
    screen_surface = display_image(image_surface);
    
    wait_for_keypressed();
    
    // Black screen -> smooth transition
    SDL_Surface* Black = SDL_CreateRGBSurface(0,image_surface->w,
                                                image_surface->h,32,0,0,0,0);

    
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
    
    //Isolation of the grid
    image_surface = Zoom(image_surface,320,1160,300,200,1060);
    Zoomed = load_image("GridOnly.bmp");
    update_surface(screen_surface,Black);
    update_surface(screen_surface,Zoomed);
    
    wait_for_keypressed();

  /*  // Rotate image by 90 degrees
    rotateMAN(90,image_surface);
    Rotated = load_image("RotatedImage.bmp");
    update_surface(screen_surface,Black);
    update_surface(screen_surface,Rotated);
    
    wait_for_keypressed();
*/
    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);

    return 0;
}
