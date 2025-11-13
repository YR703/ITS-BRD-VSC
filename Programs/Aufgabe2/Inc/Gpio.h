#ifndef GPIO_H
#define GPIO_H

#include "stm32f4xx_hal.h"
#include <stdbool.h>

#define BUTTON_PORT GPIOF
#define S0 0
#define S1 1

int readGPIOPin(GPIO_TypeDef *GPIOx, int pin);

#endif
