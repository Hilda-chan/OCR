#include "backprop.h"
#include "layer.h"
#include "neuron.h"
#include <math.h>

layer *lay = NULL;
int num_layers = 3;
int num_neurons[3] = {2,2,1};
float alpha = 0.15;
float input[4][2] = {{0,0},{1,0},{0,1},{1,1}};
float desired_outputs[4][1] = {{0},{1},{1},{0}};
int num_training_ex = 4;
int n=1;

// Feed inputs to input layer
void feed_input(int i)
{
    int j;

    for(j=0;j<num_neurons[0];j++)
    {
        lay[0].N[j].acti = input[i][j];
    }
}

void initialize_weights(void)
{
    int i,j,k;

    for(i=0;i<num_layers-1;i++)
    {
        for(j=0;j<num_neurons[i];j++)
        {
            for(k=0;k<num_neurons[i+1];k++)
            {
                // Initialize Output Weights for each neuron
                lay[i].N[j].weight[k] = ((float)rand())/((float)RAND_MAX);
                lay[i].N[j].dw[k] = 0.0;
            }

            if(i>0)
            {
                lay[i].N[j].bias = ((float)rand())/((float)RAND_MAX);
            }
        }
    }
    for (j=0; j<num_neurons[num_layers-1]; j++)
    {
        lay[num_layers-1].N[j].bias = ((float)rand())/((float)RAND_MAX);
    }
}

void update_weights(void)
{
    int i,j,k;

    for(i=0;i<num_layers-1;i++)
    {
        for(j=0;j<num_neurons[i];j++)
        {
            for(k=0;k<num_neurons[i+1];k++)
            {
                // Update Weights
                lay[i].N[j].weight[k] = (lay[i].N[j].weight[k]) - 
                    (alpha * lay[i].N[j].dw[k]);
            }
            // Update Bias
            lay[i].N[j].bias = lay[i].N[j].bias - (alpha * lay[i].N[j].dbias);
        }
    }
}

void forward(void)
{
    int i,j,k;

    for(i=1;i<num_layers;i++)
    {
        for(j=0;j<num_neurons[i];j++)
        {
            lay[i].N[j].val = lay[i].N[j].bias;

            for(k=0;k<num_neurons[i-1];k++)
            {
                lay[i].N[j].val  = lay[i].N[j].val + 
                    ((lay[i-1].N[k].weight[j])* (lay[i-1].N[k].acti));
            }

            // Activate neuron if value positive for Hidden Layers
            if(i < num_layers-1)
            {
                if((lay[i].N[j].val) < 0)
                {
                    lay[i].N[j].acti = 0;
                }

                else
                {
                    lay[i].N[j].acti = lay[i].N[j].val;
                }
            }
            // Sigmoid Activation function for Output Layer
            else
            {
                lay[i].N[j].acti = 1/(1+exp(-lay[i].N[j].val));
            }
        }
    }
}

// Back Propogate Error
void back_prop(int p)
{
    int i,j,k;

    // Output Layer
    for(j=0;j<num_neurons[num_layers-1];j++)
    {
        lay[num_layers-1].N[j].dval =
            //cost
            (lay[num_layers-1].N[j].acti - desired_outputs[p][j]) *
            //sigmoid
            (lay[num_layers-1].N[j].acti) * (1- lay[num_layers-1].N[j].acti);

        for(k=0;k<num_neurons[num_layers-2];k++)
        {
            lay[num_layers-2].N[k].dw[j] =
                (lay[num_layers-1].N[j].dval * lay[num_layers-2].N[k].acti);
            lay[num_layers-2].N[k].dacti =
                lay[num_layers-2].N[k].weight[j] * lay[num_layers-1].N[j].dval;
        }
        lay[num_layers-1].N[j].dbias = lay[num_layers-1].N[j].dval;
    }

    // Hidden Layers
    for(i=num_layers-2;i>0;i--)
    {
        for(j=0;j<num_neurons[i];j++)
        {
            if(lay[i].N[j].val >= 0)
            {
                lay[i].N[j].dval = lay[i].N[j].dacti;
            }
            else
            {
                lay[i].N[j].dval = 0;
            }

            for(k=0;k<num_neurons[i-1];k++)
            {
                lay[i-1].N[k].dw[j] = lay[i].N[j].dval * lay[i-1].N[k].acti;
                if(i>1)
                {
                    lay[i-1].N[k].dacti = 
                        lay[i-1].N[k].weight[j] * lay[i].N[j].dval;
                }
            }
            lay[i].N[j].dbias = lay[i].N[j].dval;
        }
    }
}

// Create Neural Network Architecture
void create_architecture()
{
    int i=0,j=0;
    lay = (layer*) malloc(num_layers * sizeof(layer));

    for(i=0;i<num_layers;i++)
    {
        lay[i] = create_layer(num_neurons[i]);
        lay[i].num_neuron = num_neurons[i];

        for(j=0;j<num_neurons[i];j++)
        {
            if(i < (num_layers-1)) 
            {
                lay[i].N[j] = create_neuron(num_neurons[i+1]);
            }
        }
    }

    initialize_weights();
}


// Test the trained network
void test_nn(void) 
{
    int i;
    int test = 1;
    while(test)
    {
        printf("Enter input to test (-1 to stop):\n");
        i = 0;

        while (i<num_neurons[0] && test)
        {
            scanf("%f",&lay[0].N[i].acti);
            if (lay[0].N[i].acti == -1)
                test = 0;
            ++i;
        }
        forward();
        printf("Output: %d\n", (int)round(lay[num_layers-1].N[0].acti));
        printf("\n");
    }
}

// Train Neural Network
void train_neural_net(void)
{
    int i;
    int it=0;

    // Gradient Descent
    for(it=0;it<20000;it++)
    {
        for(i=0;i<num_training_ex;i++)
        {
            feed_input(i);
            forward();
            back_prop(i);
            update_weights();
        }
    }
}

int main(void)
{
    create_architecture();

    train_neural_net();
    test_nn();

    return 0;
}
