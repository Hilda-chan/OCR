#include <stdlib.h>
#include "layer.h"

layer create_layer(int num_of_neuron)
{
	layer lay;
	lay.num_neuron = -1;
	lay.N = (struct neuron *) malloc(num_of_neuron * sizeof(struct neuron));
	return lay;
}
