#include "scaler.h"
#include "lcd_output.h"
#include <math.h>

void scale_image(uint8_t **rows, int srcW, int srcH, RGBQUAD *pal)
{
    float sx = 480.0f / srcW;
    float sy = 320.0f / srcH;
    float s = sx < sy ? sx : sy;

    int boxW = (int)ceilf(1.0f / s);
    int boxH = (int)ceilf(1.0f / s);

    static uint16_t out[480];

    for (int y = 0; y < 320; y++)
    {
        int sy1 = (int)(y / sy);
        int sy2 = sy1 + boxH;
        if (sy2 >= 5) sy2 = 4;

        for (int x = 0; x < 480; x++)
        {
            int sx1 = (int)(x / sx);
            int sx2 = sx1 + boxW;
            if (sx2 >= srcW) sx2 = srcW - 1;

            int R = 0, G = 0, B = 0, cnt = 0;

            for (int yy = sy1; yy <= sy2; yy++)
            {
                uint8_t *r = rows[yy];
                for (int xx = sx1; xx <= sx2; xx++)
                {
                    RGBQUAD c = pal[r[xx]];
                    R += c.rgbRed;
                    G += c.rgbGreen;
                    B += c.rgbBlue;
                    cnt++;
                }
            }

            RGBQUAD avg;
            avg.rgbRed = R / cnt;
            avg.rgbGreen = G / cnt;
            avg.rgbBlue = B / cnt;

            out[x] = rgb_to_16(avg);
        }

        lcd_draw_row(0, y, out, 480);
    }
}
