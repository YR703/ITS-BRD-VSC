#include "gpio.h"
#include "stm32f4xx_hal.h"
#include <stdbool.h>

/**
 * @brief Reads a GPIO pin (returns 1 if high, 0 if low)
 */
int readGPIOPin(GPIO_TypeDef* GPIOx, int pin) {
    return (HAL_GPIO_ReadPin(GPIOx, pin) == GPIO_PIN_SET) ? 1 : 0;
}

/**
 * @brief Sets a GPIO pin high or low
 * @param port GPIO port (e.g., GPIOE, GPIOD)
 * @param pin  GPIO pin number (e.g., GPIO_PIN_0)
 * @param state true = HIGH, false = LOW
 */
void setGPIOPin(GPIO_TypeDef *port, int pin, bool state) {
#ifdef SIMULATION_MODE
    // Simulation output (for Rechteckgenerator testing)
    printf("[SIM] setGPIOPin: port=%p, pin=%d, state=%d\n", port, pin, state);
#else
    if (state)
        HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
#endif
}
