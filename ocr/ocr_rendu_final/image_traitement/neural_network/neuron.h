#ifndef NEURON_H
#define NEURON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct neuron
{
	float val;
	float *weight;
	float bias;
	float acti;

	float dval;
	float *dw;
	float dbias;
	float dacti;
} neuron;

neuron create_neuron(int num_of_weights);

#endif
