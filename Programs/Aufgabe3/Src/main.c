/**
  ******************************************************************************
  * @file    main.c
  * @author  Franz Korf
  * @brief   Kleines Testprogramm fuer neu erstelle Fonts.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx_hal.h"
#include "init.h"
#include "delay.h"
#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "lcd.h"
#include "fontsFLASH.h"

#include <stdio.h>
#include <stdbool.h>

// Aufgabe 3 includes
#include "input.h"          // initInput(), openNextFile(), nextChar()
#include "bmp_reader.h"     // bmp_read_and_draw()
#include "gpio.h"           // readGPIOPin()

int main(void)
{
    bool pressed = false;

    // -----------------------------------------------------
    // Hardware Initialization (from professor template)
    // -----------------------------------------------------
    initITSboard();                // ITS-Board init
    GUI_init(DEFAULT_BRIGHTNESS);  // LCD init
    TP_Init(false);                // Touch init

    // Aufgabe 3 – UART input initialization
    initInput();

    GUI_clear(WHITE);
    lcdPrintS("Aufgabe 3 – Press S0 to show BMP");

    // -----------------------------------------------------
    // Main Loop
    // -----------------------------------------------------
    while (1)
    {
        pressed = readGPIOPin(GPIOF, 0);   // Button S0

        if (pressed)
        {
            GUI_clear(WHITE);
            lcdPrintS("Loading BMP...");

            // Main drawing function (Teil A + B)
            bmp_read_and_draw();

            lcdPrintS("Finished. Press S0 for next.");
            HAL_Delay(300); // Basic debounce
        }
    }
}

// EOF
