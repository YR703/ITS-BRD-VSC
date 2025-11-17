#include "gpio.h"
#include "berechnung.h"
#include "error_drehgeber.h"
#include "lcd.h"
#include "output.h"
#include "terminal.h"

#ifndef S6
#define S6 GPIO_PIN_6
#endif

#ifndef D21
#define D21 GPIO_PIN_1
#endif


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
