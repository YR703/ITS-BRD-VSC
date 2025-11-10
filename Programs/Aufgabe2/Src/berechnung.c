#include <stdbool.h>

#include "berechnung.h"
#include "error_drehgeber.h"
#include "input.h"
#include "output.h"

int state = Start;
int prev = 0;
int phasen = 0;     // Phasen
int phasentmp = 0;  // Phasen pro Sekunde
int sek = 1;
double winkel = 0;

/**
 * @brief  Diese Funktion setzt den Phasenzaehler zurück
 */
void reset() {
  reset_counter();
  prev = 0;
  phasen = 0;
  phasentmp = 0;
  state = Start;
}

/**
 * @param input
 * @brief  Diese schaut sich den Input an und je nach Phasenübergang verhält es
 * sich demenstprechend
 */
int phasen_ueberpruefung(int input, int resetpressed) {
  state = delta[state][input];
  if (state == prev) {
    return 1;
  }
  prev = state;
  if (resetpressed == true) {
    reset();
  }
  switch (state) {
    case Err:
      led_keine_aenderung();
      return PHASEUEBERSPRUNGEN;
      break;
    case AForRot:
    case BForRot:
    case CForRot:
    case DForRot:
      led_vorwaerts();
      phasen++;
      phasentmp++;
      print_led();
      break;
    case ABackRot:
    case BBackRot:
    case CBackRot:
    case DBackRot:
      led_rueckwaerts();
      phasentmp++;
      phasen++;
      print_led();
      break;
  }
  return 0;
}

/**
 * @brief Falls Reset gedrückt wurde wird der Automat zurückgesetzt
 */
void reset_prev() {
  state = Start;
  prev = 0;
}

/**
 * @brief  Diese Funktion gibt die Phasen pro Sekunde zurück
 */
int get_phasen_per_second(int zeit) {
  if (1 == zeit) {
    int tmp = phasentmp;
    phasentmp = 0;
    sek++;
    return tmp;
  }
  return 0;
}

/**
 * @brief  Diese Funtktion gibt die Phasen zurück
 */
int getphasen() { return phasen; }

/**
 * @brief  Diese Funtktion gibt die Winkelgeschwindigkeit zurück
 */
double get_winkelgeschw() {
	double geschw = 0;
  geschw = phasen * SCHLITZE / sek;
  return geschw;
}

/**
 * @brief  Diese Funtktion gibt den Winkel zurück
 */
double get_winkel() {
	double winkel = 0;
  winkel = phasen * SCHLITZE;
  return winkel;
}
