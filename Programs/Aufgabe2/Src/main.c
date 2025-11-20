#include "stm32f4xx_hal.h"
#include "init.h"
#include "lcd.h"
#include "LCD_GUI.h"
#include "fontsFLASH.h"
#include "LCD_Touch.h"
#include "input.h"
#include "berechnung.h"
#include "output.h"
#include "error_drehgeber.h"
#include "timer.h"
#include <stdio.h>

#define OUTPUT_SIZE 8

int main(void) {
    int input = 0, output = 0, phasen = 0, reset = 0;
    double winkel = 0, geschw = 0;
    int last_phasen = -1;
    int print_idx = 0;
    uint32_t timer_ticks = 0;

    char old_winkel[OUTPUT_SIZE];
    char old_geschw[OUTPUT_SIZE];

    char buf_winkel[OUTPUT_SIZE];
    char buf_geschw[OUTPUT_SIZE];

    // --- Initialisierung ---
    initITSboard();
    GUI_init(DEFAULT_BRIGHTNESS);
    initTimer();

    // --- Bildschirm vorbereiten ---
    lcdGotoXY(0, 0);
    lcdSetFont(16);
    lcdPrintS("Drehgeber gestartet...");
    HAL_Delay(1000);

    lcdGotoXY(0, 0);
    lcdPrintS("Winkel (Grad):");

    lcdGotoXY(0, 2);
    lcdPrintS("Geschwindigkeit (Grad/s):");

    // --- Hauptschleife ---
    while (1) {
        input = input_einlesen();
        reset = resetpressed();
        output = phasen_ueberpruefung(input, reset);
        timer_ticks = getTimeStamp();

        phasen = getphasen();
        winkel = get_winkel();
        geschw = get_winkelgeschw(timer_ticks, winkel, output == 0);

        if(print_idx == 0)
        {
          snprintf(buf_winkel, OUTPUT_SIZE, "%7.1f", winkel);
          snprintf(buf_geschw, OUTPUT_SIZE, "%7.1f", geschw);
        }

        if(buf_winkel[print_idx] != old_winkel[print_idx])
        {
          old_winkel[print_idx] = buf_winkel[print_idx];
          lcdGotoXY(26 + print_idx, 0);
          lcdPrintC(buf_winkel[print_idx]);
        }

        if(buf_geschw[print_idx] != old_geschw[print_idx])
        {
          old_geschw[print_idx] = buf_geschw[print_idx];
          lcdGotoXY(26 + print_idx, 2);
          lcdPrintC(buf_geschw[print_idx]);
        }

        print_idx++;
        if(print_idx == OUTPUT_SIZE - 1)
        {
          print_idx = 0;
        }

        if (output == PHASEUEBERSPRUNGEN)
            error_number(output);
        
        led_counter(phasen);
    }
}
