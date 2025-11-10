#include <stdio.h>
#include <stdbool.h>

#include "stm32f4xx_hal.h"
#include "init.h"
#include "delay.h"
#include <stdbool.h>
#include "gpio.h"
#include "output.h"
#include "input.h"
#include "berechnung.h"
#include "error_drehgeber.h"
#include "timer.h"
#include "init.h"
#include "LCD_GUI.h"
#include "LCD_Demos.h"
#include "lcd.h"
#include "fontsFLASH.h"
#include "LCD_Touch.h"
#include "error.h"

/**
 * @brief  Main program
 */
int main(void) {
  int input = 0,   // Bekommt den Input
      output = 0,  // Schaut ob es ein Error gibt
      phasen = 0,  // Gibt die Phasen von 1 Sekunde Zurück
      reset = 0,   // Schaut ob S7 gedrückt worden ist
      zeit = 0,    // Schaut ob eine Sekunde vergangen ist
	    sumphasen = 0;// Die Summe der Phasen
	
  double winkel = 0, // Der Winkel
	    geschw = 0; // Die Geschwindigkeit pro Sekunde
	
  initITSboard();                // Initialisierung des ITS Boards
  GUI_init(DEFAULT_BRIGHTNESS);  // Initialisierung des LCD Boards mit Touch
  TP_Init(false);                // Initialisierung des LCD Boards mit Touch
  initTimer();

  while (1) {
    // Eingabe
		zeit = inputzeit();
    input = input_einlesen();
    reset = resetpressed();

    // Verarbeitung
    phasen = get_phasen_per_second(zeit);
		sumphasen = getphasen();
		winkel = get_winkel();
		geschw = get_winkelgeschw();
    output = phasen_ueberpruefung(input, reset);

    // Ausgabe
    if (zeit == 1) {
      print_lcdboard(phasen,sumphasen,winkel,geschw);
      }
		if (output == PHASEUEBERSPRUNGEN) {
        error_number(output);
    }
  }
}

// EOF