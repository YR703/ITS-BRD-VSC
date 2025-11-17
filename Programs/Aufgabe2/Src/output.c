#include "ITS_BRD_conf.h"    // board pin mapping (LED_PORTx, Dx)
#include "gpio.h"            // your S0/S1 input
#include "berechnung.h"
#include "lcd.h"
#include "output.h"
#include "terminal.h"
#include "C:\Users\infwtx851\GSWS\stm32cubef4\Drivers\STM32F4xx_HAL_Driver\Inc\stm32f4xx_hal_gpio.h"

#include <stdbool.h>
#include <stdio.h>

#ifndef LED_PORTD
#define LED_PORTD GPIOD
#define LED_PORTE GPIOE
#define D21 GPIO_PIN_1
#define D22 GPIO_PIN_2
#define D23 GPIO_PIN_3
#endif

// Dummy implementation for simulation (no hardware)
void setGPIOPin(GPIO_TypeDef* port, int pin, bool state) {
    printf("[SIM] GPIO action: port=%p, pin=%d, state=%d\n", port, pin, state);
}


#define MAX 8
#define MAXSTR 11
 
int counter = 1;

void print_led() {
  for (int i = 0; i < MAX; i++) {
    if (((counter >> i) & 1) == 1)
      setGPIOPin(LED_PORTD, i, true);
    else
      setGPIOPin(LED_PORTD, i, false);
  }
  counter++;
}

void print_lcdboard(int phasen, int sumphasen, int winkel, double geschw) {
  char pgeschw[20];
  double_to_string(geschw, pgeschw);

  lcdGotoXY(0, 0);
  lcdPrintInt(winkel);
  lcdGotoXY(0, 10);
  lcdPrintS(pgeschw);
}

void led_vorwaerts() {
  setGPIOPin(LED_PORTE, D22, false);
  setGPIOPin(LED_PORTE, D23, true);
}

void led_rueckwaerts() {
  setGPIOPin(LED_PORTE, D23, false);
  setGPIOPin(LED_PORTE, D22, true);
}

void led_fehler() {
  setGPIOPin(LED_PORTE, D21, true);
}

void led_keine_aenderung() {
  setGPIOPin(LED_PORTE, D23, false);
  setGPIOPin(LED_PORTE, D22, false);
}

char* double_to_string(double geschw, char* pgeschw) {
  sprintf(pgeschw, "%f", geschw);
  return pgeschw;
}

void reset_counter() { counter = 0; }

void led_fehler_reset() { setGPIOPin(LED_PORTE, D21, false); }
