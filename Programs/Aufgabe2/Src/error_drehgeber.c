#include "stm32f4xx_hal.h"
#include "error_drehgeber.h"
#include "output.h"
#include "lcd.h"

void error_number(int fehler) {
    led_fehler();
    lcdGotoXY(0, 140);
    lcdPrintS("Fehler erkannt!");

    while (1) {
        HAL_Delay(500);
        led_fehler_reset();
        HAL_Delay(500);
        led_fehler();
    }
}
