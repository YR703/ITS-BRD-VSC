#include <stdlib.h>

#include "init.h"
#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "fontsFLASH.h"

#include "input.h"
#include "headers.h"
#include "bmp_reader.h"
#include "lcd_output.h"
#include "scaler.h"
#include "rle_decoder.h"
#include "gpio.h"
#include "errorhandler.h"

#define S0_PORT GPIOF
#define S0_PIN  0

/* Simple button check */
int button_pressed()
{
    return readGPIOPin(S0_PORT, S0_PIN) == 0;
}

int main(void)
{
    /* Initialize board and LCD */
    initITSboard();
    GUI_init(DEFAULT_BRIGHTNESS);
    TP_Init(false);

    if (!checkVersionFlashFonts())
        LOOP_ON_ERR(true, "Flash font mismatch");

    initInput();
    GUI_clear(WHITE);

    while (1)
    {
        /* Wait for button press */
        while (!button_pressed());
        while (button_pressed());

        /* NEXT FILE */
        openNextFile();

        /* FIX 1 — LCD löschen bevor ein neues Bild kommt */
        GUI_clear(WHITE);

        BITMAPFILEHEADER fh;
        BITMAPINFOHEADER ih;
        RGBQUAD *pal = malloc(256 * sizeof(RGBQUAD));

        /* BMP Reader with full error handling */
        if (bmp_start(&fh, &ih, pal) != EOK)
        {
            /* FEHLER wurde bereits angezeigt */
            /* nach S0 drücken → weitermachen */
            while (!button_pressed());
            while (button_pressed());
            free(pal);
            continue;
        }

        int w = ih.biWidth;
        int h = ih.biHeight;

        uint8_t *row = malloc(w);

        /* CASE 1: Fits into LCD (NO SCALING) */
        if (w <= 480 && h <= 320)
        {
            /* BOTTOM → TOP zeichnen (dein korrektes Verhalten) */
            for (int y = h - 1; y >= 0; y--)
            {
                int res = bmp_read_row(row, w);
                if (res < 0)
                {
                    lcdErrorMsg("Fehler: Daten fehlerhaft");
                    break;
                }

                static uint16_t buf[480];
                for (int i = 0; i < w; i++)
                    buf[i] = rgb_to_16(pal[row[i]]);

                lcd_draw_row(0, y, buf, w);
            }
        }
        else
        {
            /* CASE 2: Needs scaling */
            uint8_t *rows[5];
            for (int i = 0; i < 5; i++)
                rows[i] = malloc(w);

            /* IMPORTANT FIX:
               Reihenfolge top-down einlesen, NICHT invertiert */
            for (int i = 0; i < 5; i++)
            {
                if (bmp_read_row(rows[i], w) < 0)
                {
                    lcdErrorMsg("Fehler: RLE Stream");
                    break;
                }
            }

            scale_image(rows, w, h, pal);

            for (int i = 0; i < 5; i++)
                free(rows[i]);
        }

        free(row);
        free(pal);

        /* Button must be released before next iteration */
        while (button_pressed());
    }

    return 0;
}
