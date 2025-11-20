#include "stm32f4xx_hal.h"
#include "input.h"

#define BUTTON_PORT GPIOF
#define S0 GPIO_PIN_0
#define S1 GPIO_PIN_1
#define S7 GPIO_PIN_7  // Reset button (optional)

int input_einlesen(void) {
    int s0 = HAL_GPIO_ReadPin(BUTTON_PORT, S0);
    int s1 = HAL_GPIO_ReadPin(BUTTON_PORT, S1);

    if (!s0 && !s1) return 0; // Phase A
    if (s0 && !s1)  return 1; // Phase B
    if (s0 && s1)   return 2; // Phase C
    if (!s0 && s1)  return 3; // Phase D
    return 0;
}

int inputzeit(void) {
    return 1; // Simplified: update every loop
}

int resetpressed(void) {
    int s7 = HAL_GPIO_ReadPin(BUTTON_PORT, S7);
    return s7 ? 1 : 0;
}
