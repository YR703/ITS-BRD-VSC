#ifndef GPIO_H
#define GPIO_H

#define BUTTON_PORT GPIOF
#define S0 0
#define S1 1

#include "stm32f4xx_hal.h"

#include <stdbool.h>

/**
 * @brief  Diese Funktion liest den Pin vom PORTF  
 */
int readGPIOPin(GPIO_TypeDef * GPIOx, int pin);

#endif