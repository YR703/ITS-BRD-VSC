#include <stdbool.h>
#include "berechnung.h"
#include "C:\GSWS\ITS-BRD-VSC\Programs\Aufgabe2\Inc\berechnung.h"
#include "error_drehgeber.h"
#include "input.h"
#include "output.h"

int state = Start;
int prev = 0;
int phasen = 0;
int phasentmp = 0;
int sek = 1;

void reset() {
  reset_counter();
  prev = 0;
  phasen = 0;
  phasentmp = 0;
  state = Start;
}

int phasen_ueberpruefung(int input, int resetpressed) {
  state = delta[state][input];
  if (state == prev) return 1;
  prev = state;

  if (resetpressed == true) reset();

  switch (state) {
    case Err:
      led_keine_aenderung();
      return PHASEUEBERSPRUNGEN;
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
      phasen++;
      phasentmp++;
      print_led();
      break;
  }
  return 0;
}

int get_phasen_per_second(int zeit) {
  if (1 == zeit) {
    int tmp = phasentmp;
    phasentmp = 0;
    sek++;
    return tmp;
  }
  return 0;
}

int getphasen() { return phasen; }

double get_winkelgeschw() {
  return (phasen * SCHLITZE) / sek;
}

double get_winkel() {
  return phasen * SCHLITZE;
}
