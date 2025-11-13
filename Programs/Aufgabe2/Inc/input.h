#ifndef _INPUT_H
#define _INPUT_H

#include "gpio.h"
#include <stdbool.h>

int input_einlesen();
int inputzeit();
bool resetpressed();
unsigned int get_Time();

#endif
