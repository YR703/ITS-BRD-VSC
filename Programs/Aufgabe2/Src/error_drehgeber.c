#include "error_drehgeber.h"
#include "gpio.h"
#include "output.h"
#include "berechnung.h"

int error_number(int fehler) {
    led_fehler();
    if (fehler == PHASEUEBERSPRUNGEN) {
        while (1) {
            int s6 = readGPIOPin(BUTTON_PORT, S6);
            if (s6) {
                HAL_GPIO_WritePin(GPIOE, D21, GPIO_PIN_RESET);

                reset();
                return 0;
            }
        }
    }
    return 0;
}
