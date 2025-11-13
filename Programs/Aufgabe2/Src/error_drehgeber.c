#include "gpio.h"
#include "berechnung.h"
#include "error_drehgeber.h"
#include "lcd.h"
#include "output.h"
#include "terminal.h"

int error_number(int fehler) {
  led_fehler();
  if (fehler == PHASEUEBERSPRUNGEN) {
    int s6Pressed = 0;
    while (1) {
      s6Pressed = readGPIOPin(BUTTON_PORT, S6);
      if (s6Pressed == true) {
        setGPIOPin(GPIOE, D21, false);
        reset();
        return 0;
      }
    }
  }
  return 0;
}
