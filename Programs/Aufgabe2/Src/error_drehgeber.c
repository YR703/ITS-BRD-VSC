#include "error_drehgeber.h"
#include "gpio.h"
#include "output.h"
#include "berechnung.h"

int error_number(int fehler) {
    led_fehler(); // D21 EIN
    if (fehler == PHASEUEBERSPRUNGEN) {
        while (1) {
            int s6 = readGPIOPin(BUTTON_PORT, S6);
            if (s6 == 0) {  // S6 gedrückt
                led_fehler_reset();   // D21 AUS
                reset_system();       // FSM & Zähler zurücksetzen
                return 0;
            }
        }
    }
    return 0;
}
