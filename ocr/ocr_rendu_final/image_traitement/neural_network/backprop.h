#ifndef BACKPROP_H
#define BACKPROP_H

#include "layer.h"

void create_architecture(void);
void initialize_weights(void);
void feed_input(int i);
void train_neural_net(void);
void forward(void);
void back_prop(int p);
void update_weights(void);
void test_nn(void);

#endif
