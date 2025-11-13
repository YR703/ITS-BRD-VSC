#include "gpio.h"

#define MIN_GPIO_PIN 0
#define MAX_GPIO_PIN 7

int readGPIOPin(GPIO_TypeDef* GPIOx, int pin) {
  return (0x01 << pin) != (GPIOx->IDR & (0x01 << pin));
}
