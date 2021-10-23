#ifndef LAYER_H
#define LAYER_H

#include "neuron.h"

typedef struct layer
{
	int num_neuron;
	struct neuron *N;
}layer;

layer create_layer(int num_of_neuron);

#endif
