#ifndef GPIO_H
#define GPIO_H
#define LED_PORTD GPIOD
#define LED_PORTE GPIOE
#define BUTTON_PORT GPIOE

#define D21 GPIO_PIN_5
#define D22 GPIO_PIN_6
#define D23 GPIO_PIN_7
#define S6 GPIO_PIN_4
#define S7 GPIO_PIN_7


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