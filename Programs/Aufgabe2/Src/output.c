#include "output.h"
#include "lcd.h"
#include "berechnung.h"
#include "gpio.h"
#include <stdio.h>

void led_vorwaerts() {
  HAL_GPIO_WritePin(GPIOE, D22, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOE, D23, GPIO_PIN_SET);
}

void led_rueckwaerts() {
  HAL_GPIO_WritePin(GPIOE, D23, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOE, D22, GPIO_PIN_SET);
}

void led_fehler() {
  HAL_GPIO_WritePin(GPIOE, D21, GPIO_PIN_SET);
}

void led_keine_aenderung() {
  HAL_GPIO_WritePin(GPIOE, D23, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOE, D22, GPIO_PIN_RESET);
}

void led_counter(int phasen) {
  GPIOD->BSRR = (phasen & 0xFF) | ((~phasen) << 16);
}
