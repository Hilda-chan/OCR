#include <stdio.h>
#include <SDL/SDL.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[])
{
	//charge image
	SDL_surface* image = load_image(path);
	//Get format
	SDL_PixelFormat *fmt = image->format;
	//width and height
	int width = image->w;
	int height = image->h;
	//Initialization pixel and r,g,b for grayscale
	Uint32 pixel;
	Uint8 *r,*g,*b;
	//Inputs, outputs, weights, biases and cost
	double in[w*h],out[9],weight[],bias[],cost[];

	//fill in with grayscale values of each pixel
	for (int y = 0; y<height; ++y)
	{
		for (int x = 0; x<width; ++x)
		{

			SDL_GetRGB(pixel,fmt,&r,&g,&b);
			in[height*x+width] = r;
			printf("%l",r);
		}
	}

}
