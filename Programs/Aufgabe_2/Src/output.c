#include <stdio.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"

#include "gpio.h"
#include "berechnung.h"
#include "lcd.h"
#include "output.h"
#include "terminal.h"

#define MAX 8
#define MAXSTR 11

int counter = 1;
char str[MAXSTR];

/**
 * @brief  Diese Funktion gibt die Phasen auf dem Led aus
 */
void print_led() {
  for (int i = 0; i < MAX; i++) {
    if (((counter >> i) & 1) == 1) {
      setGPIOPin(LED_PORTD, i, true);
    } else {
      setGPIOPin(LED_PORTD, i, false);
    }
  }
  counter++;
}

/**
 * @brief  Diese Funktion gibt den Winkel und die Winkelgeschwindigkeit auf dem
 * Board aus
 */
void print_lcdboard(int phasen, int sumphasen, int winkel, double geschw);
 {
  char pgeschw[20];

  double_to_string(geschw, pgeschw);

  lcdGotoXY(0, 0);
  lcdPrintInt(winkel);

  lcdGotoXY(0, 10);
  lcdPrintS(pgeschw);
}

/**
 * @brief  Diese Funktion zeigt dass sich die Phasen nach hinten bewegen
 */
void led_vorwaerts() {
  setGPIOPin(LED_PORTE, D22, false);
  setGPIOPin(LED_PORTE, D23, true);
}
/**
 * @brief  Diese Funktion zeigt dass sich die Phasen nach vorne bewegen
 */
void led_rueckwaerts() {
  setGPIOPin(LED_PORTE, D23, false);
  setGPIOPin(LED_PORTE, D22, true);
}
/**
 * @brief  Diese Funktion zeigt den Fehlerzustand
 */
void led_fehler() { setGPIOPin(LED_PORTE, D21, true); }
/**
 * @brief  Diese Funktion zeigt dass sich die Phasen nicht gäendert haben
 */
void led_keine_aenderung() {
  setGPIOPin(LED_PORTE, D23, false);
  setGPIOPin(LED_PORTE, D22, false);
}
/**
 * @brief  Diese Funktion wandelt ein Double in String um
 */
char* double_to_string(double geschw, char* pgeschw) {
  sprintf(pgeschw, "%f", geschw);
  return pgeschw;
}
/**
 * @brief  Diese Funktion setzt Phasenzähler zurück
 */
void reset_counter() { counter = 0; }
/**
 * @brief  Diese Funktion löscht den Fehlerzustand
 */
void led_fehler_reset() { setGPIOPin(LED_PORTE, D21, false); }