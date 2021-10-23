#include "neuron.h"

neuron create_neuron(int num_of_weights)
{
	neuron neu;

	neu.acti = 0.0;
	neu.weight = (float*) malloc(num_of_weights * sizeof(float));
	neu.bias=0.0;
	neu.val = 0.0;

	neu.dacti = 0.0;
	neu.dw = (float*) malloc(num_of_weights * sizeof(float));
	neu.dbias = 0.0;
	neu.dval = 0.0;

	return neu;	
}
