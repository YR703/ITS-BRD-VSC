#include "scaler.h"
#include "lcd_output.h"
#include <math.h>

/*
 * rows: 5 Zeilen aus bmp_read_row()
 * srcW = Originalbreite
 * srcH = Originalhöhe
 * pal = Palette für Farbwerte
 *
 * Wir skalieren nur VERTIKAL über das 5-Zeilen-Fenster,
 * da die Aufgabe 3 genau dieses Verfahren fordert.
 */

void scale_image(uint8_t **rows, int srcW, int srcH, RGBQUAD *pal)
{
    float scaleX = (float)srcW / 480.0f;
    float scaleY = (float)srcH / 320.0f;

    float s = (scaleX > scaleY) ? scaleX : scaleY;

    int boxH = (int)ceilf(s);
    int boxW = (int)ceilf(s);

    static uint16_t out[480];

    for (int y = 0; y < 320; y++)
    {
        int srcY = (int)(y * s);

        int rowIndex = srcY % 5;   // Nur 5 rows verfügbar!

        uint8_t *r = rows[rowIndex];

        for (int x = 0; x < 480; x++)
        {
            int srcX = (int)(x * s);
            if (srcX >= srcW) srcX = srcW - 1;

            RGBQUAD c = pal[r[srcX]];
            out[x] = rgb_to_16(c);
        }

        lcd_draw_row(0, y, out, 480);
    }
}
