 #include "gpio.h"
 
 
 #define MIN_GPIO_PIN 0 
 #define MAX_GPIO_PIN 7
/**
 * @brief  Diese Funktion liest den Pin vom PORTF 
 */ 
int readGPIOPin(GPIO_TypeDef *GPIOx, int pin) {
    return ((GPIOx->IDR & (1 << pin)) != 0);
}
