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
#include <stdlib.h>
#include "input.h"
#include "headers.h"
#include "bmp_reader.h"
#include "lcd_output.h"
#include "scaler.h"
#include "rle_decoder.h"
#include "gpio.h"
#include "errorhandler.h"
#include "LCD_GUI.h"

#define S0_PORT GPIOF
#define S0_PIN  0

int button_pressed()
{
    return readGPIOPin(S0_PORT, S0_PIN) == 0;

}



int main(void)
{
    initITSboard();                     // REQUIRED or LCD stays black
    GUI_init(DEFAULT_BRIGHTNESS);       // Turn on LCD
    TP_Init(false);                     // Touch (safe to include)

    if (!checkVersionFlashFonts())
    LOOP_ON_ERR(1, "Flash font version mismatch");


    initInput();                        // UART protocol init
    GUI_clear(WHITE);                   // Clear display

    while (1)
    {
        while (!button_pressed());      // Wait for S0

        openNextFile();

        BITMAPFILEHEADER fh;
        BITMAPINFOHEADER ih;
        RGBQUAD *pal = malloc(256 * sizeof(RGBQUAD));

        if (bmp_start(&fh, &ih, pal) != EOK)
        {
            free(pal);
            continue;
        }

        int w = ih.biWidth;
        int h = ih.biHeight;

        uint8_t *row = malloc(w);

        if (w <= 480 && h <= 320)
        {
            for (int y = h - 1; y >= 0; y--)
            {
                bmp_read_row(row, w);

                static uint16_t buf[480];
                for (int i = 0; i < w; i++)
                    buf[i] = rgb_to_16(pal[row[i]]);
                    
                lcd_draw_row(0, y, buf, w);
            }
        }
        else
        {
            uint8_t *rows[5];
            for (int i = 0; i < 5; i++)
                rows[i] = malloc(w);

            for (int i = 4; i >= 0; i--)
                bmp_read_row(rows[i], w);

            scale_image(rows, w, h, pal);

            for (int i = 0; i < 5; i++)
                free(rows[i]);
        }

        free(row);
        free(pal);

        while (button_pressed());    // Wait until button released
    }
}
