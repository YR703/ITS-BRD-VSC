#include <stdio.h>
#include <stdbool.h>

#include "stm32f4xx_hal.h"
#include "init.h"
#include "delay.h"
#include "C:\GSWS\ITS-BRD-VSC\Programs\Aufgabe2\Inc\gpio.h"
#include "output.h"
#include "C:\GSWS\ITS-BRD-VSC\Programs\Aufgabe2\Inc\output.h"
#include "input.h"
#include "berechnung.h"
#include "error_drehgeber.h"
#include "timer.h"
#include "lcd.h"

int main(void) {
  int input = 0, output = 0, phasen = 0, reset = 0, zeit = 0, sumphasen = 0;
  double winkel = 0, geschw = 0;

  initITSboard();
  GUI_init(DEFAULT_BRIGHTNESS);
  TP_Init(false);
  initTimer();

  while (1) {
    zeit = inputzeit();
    input = input_einlesen();
    reset = resetpressed();

    phasen = get_phasen_per_second(zeit);
    sumphasen = getphasen();
    winkel = get_winkel();
    geschw = get_winkelgeschw();
    output = phasen_ueberpruefung(input, reset);

    if (zeit == 1)
      print_lcdboard(phasen, sumphasen, winkel, geschw);

    if (output == PHASEUEBERSPRUNGEN)
      error_number(output);
  }
}
