#include <stdio.h>
#include "gpio.h"
#include "berechnung.h"
#include "lcd.h"
#include "output.h"
#include "terminal.h"
#include "C:\GSWS\ITS-BRD-VSC\Programs\Aufgabe2\Inc\gpio.h"

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
