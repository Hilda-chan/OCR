#include "backprop.h"
#include "architecture.h"
#include <math.h>

layer *L = NULL;
int num_L = 3;
int num_N[3] = {2,2,1};
float lrate = 0.15;
float input[4][2] = {{1,0},{0,0},{0,1},{1,1}};
float desired_outputs[4][1] = {{1},{0},{1},{0}};
int num_training_ex = 4;
int n=1;


void feed_input(int i)
{
    int j = 0;

    while (j < num_N[0])
    {
        L[0].N[j].activation = input[i][j];
        j++;
    }
}

void initialize_weights(void)
{

    for(int i = 0; i < num_L-1; i++)
    {
        for(int j = 0; j < num_N[i]; j++)
        {
            for(int k = 0; k < num_N[i+1]; k++)
            {
                L[i].N[j].weight[k] = ((float)rand())/((float)RAND_MAX);
                L[i].N[j].dweight[k] = 0.0;
            }
            if(i > 0)
            {
                L[i].N[j].bias = ((float)rand())/((float)RAND_MAX);
            }
        }
    }
    for (int j = 0; j < num_N[num_L-1]; j++)
    {
        L[num_L-1].N[j].bias = ((float)rand())/((float)RAND_MAX);
    }
}

void update_weights(void)
{
    for(int i = 0; i < num_L-1; i++)
    {
        for(int j = 0; j < num_N[i]; j++)
        {
            for(int k =  0; k < num_N[i+1]; k++)
            {
               L[i].N[j].weight[k] = (L[i].N[j].weight[k]) -
                 (lrate * L[i].N[j].dweight[k]);
            }
           L[i].N[j].bias = L[i].N[j].bias - (lrate * L[i].N[j].dbias);
        }
    }
}

void forward(void)
{
    for(int i = 1; i < num_L; i++)
    {
        for(int j = 0; j < num_N[i]; j++)
        {
            L[i].N[j].value = L[i].N[j].bias;

            for(int k = 0; k < num_N[i-1]; k++)
            {
                L[i].N[j].value  = L[i].N[j].value + 
                    ((L[i-1].N[k].weight[j])* (L[i-1].N[k].activation));
            }

            if(i < num_L-1)
            {
                if((L[i].N[j].value) < 0)
                {
                    L[i].N[j].activation = 0;
                }

                else
                {
                    L[i].N[j].activation = L[i].N[j].value;
                }
            }
            else
            {
                L[i].N[j].activation = 1/(1+exp(-L[i].N[j].value));
            }
        }
    }
}


void back_prop(int p)
{

    for(int j = 0; j < num_N[num_L-1]; j++)
    {
        L[num_L-1].N[j].dvalue =
            (L[num_L-1].N[j].activation - desired_outputs[p][j]) *
            (L[num_L-1].N[j].activation) * (1- L[num_L-1].N[j].activation);

        for(int k = 0; k < num_N[num_L-2]; k++)
        {
            L[num_L-2].N[k].dweight[j] =
                (L[num_L-1].N[j].dvalue * L[num_L-2].N[k].activation);
            L[num_L-2].N[k].dactivation =
                L[num_L-2].N[k].weight[j] * L[num_L-1].N[j].dvalue;
        }
        L[num_L-1].N[j].dbias = L[num_L-1].N[j].dvalue;
    }

    for(int i = num_L-2; i > 0; i--)
    {
        for(int j = 0; j < num_N[i]; j++)
        {
            if(L[i].N[j].value >= 0)
            {
                L[i].N[j].dvalue = L[i].N[j].dactivation;
            }
            else
            {
                L[i].N[j].dvalue = 0;
            }

            for(int k = 0; k < num_N[i-1]; k++)
            {
                L[i-1].N[k].dweight[j] = L[i].N[j].dvalue * L[i-1].N[k].activation;
                if(i>1)
                {
                    L[i-1].N[k].dactivation = 
                        L[i-1].N[k].weight[j] * L[i].N[j].dvalue;
                }
            }
            L[i].N[j].dbias = L[i].N[j].dvalue;
        }
    }
}

void create_architecture()
{
    L = (layer*) malloc(num_L * sizeof(layer));

    for(int i = 0; i < num_L; i++)
    {
        L[i] = create_layer(num_N[i]);
        L[i].num_N = num_N[i];

        for(int j = 0;j<num_N[i];j++)
        {
            if(i < (num_L-1)) 
            {
                L[i].N[j] = create_neuron(num_N[i+1]);
            }
        }
    }

    initialize_weights();
}


void test_nn(void) 
{
    int i = 0;
    int test = 1;
    while(test)
    {
        printf("enter logical inputs (0 or 1) (-1 to exit):\n");
        i = 0;

        while (i < num_N[0] && test)
        {
            scanf("%f",&L[0].N[i].activation);
            if (L[0].N[i].activation == -1)
                test = 0;
            ++i;
        }
        forward();
        printf("Output: %d\n", (int)round(L[num_L-1].N[0].activation));
        printf("\n");
    }
}

void train_neural_net(void)
{
    int i;

    for(int it = 0; it < 20000; it++)
    {
        for(i = 0; i < num_training_ex; i++)
        {
            feed_input(i);
            forward();
            back_prop(i);
            if(it % 4000 == 0)
                print_train(it);
            update_weights();
        }
    }
}

void print_train(int i)
{
    printf("Iteration number : %u\n", i);
    printf("Weight input layer = %f, %f\n", L[0].N[0].weight[0], L[0].N[1].weight[0]);
    printf("Weight hidden layer = %f, %f\n", L[1].N[0].weight[0], L[1].N[1].weight[0]);
    printf("Weight output layer = %f\n\n\n", L[0].N[0].weight[0]);
}

int main(void)
{
    create_architecture();

    train_neural_net();
    test_nn();

    return 0;
}
