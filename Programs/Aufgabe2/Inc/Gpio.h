#ifndef GPIO_H
#define GPIO_H

#include "stm32f4xx_hal.h"
#include <stdbool.h>

#define BUTTON_PORT GPIOF
#define S0 GPIO_PIN_0
#define S1 GPIO_PIN_1

/**
 * @brief Reads a GPIO pin state.
 */
int readGPIOPin(GPIO_TypeDef *GPIOx, int pin);

/**
 * @brief Sets a GPIO pin high or low.
 */
void setGPIOPin(GPIO_TypeDef *port, int pin, bool state);

#endif
