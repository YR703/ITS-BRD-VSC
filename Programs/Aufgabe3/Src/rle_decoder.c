#include "rle_decoder.h"
#include "input.h"
#include "LCD_GUI.h"
#include "fontsFLASH.h"
#include <stdio.h>

static int curX = 0;
static int curY = 0;
static int height = 0;
static int ended = 0;

/* Fehlerausgabe */
static void rleError(const char *msg)
{
    GUI_clear(WHITE);
    Coordinate tl = { 10, 10 };
    GUI_disStr(tl, msg, &Font16, WHITE, RED);
}

void rle_reset_with_height(int h)
{
    height = h;
    curX = 0;
    curY = 0;
    ended = 0;
}

void rle_reset(void)
{
    curX = 0;
    curY = 0;
    ended = 0;
}

int rle_decode_row(uint8_t *row, int width)
{
    if (ended)
    {
        for (int i = 0; i < width; i++)
            row[i] = 0;
        return 1;
    }

    /* Zeile löschen */
    for (int i = 0; i < width; i++)
        row[i] = 0;

    curX = 0;

    while (curY < height)
    {
        int c1 = nextChar();
        int c2 = nextChar();

        if (c1 == EOF || c2 == EOF)
        {
            rleError("Fehler: RLE EOF");
            return -1;
        }

        /* ENCODED MODE */
        if (c1 > 0)
        {
            uint8_t color = (uint8_t)c2;

            for (int i = 0; i < c1; i++)
            {
                if (curX < width)
                    row[curX] = color;
                curX++;
            }
        }
        else
        {
            /* ESCAPE CODES */

            /* END OF LINE */
            if (c2 == 0)
            {
                curY++;
                return 0;
            }

            /* END OF BITMAP */
            else if (c2 == 1)
            {
                ended = 1;
                return 0;
            }

            /* DELTA MOVE */
            else if (c2 == 2)
            {
                int dx = nextChar();
                int dy = nextChar();

                if (dx == EOF || dy == EOF)
                {
                    rleError("Fehler: RLE Delta EOF");
                    return -1;
                }

                curX += dx;
                curY += dy;

                if (curX >= width) curX = width - 1;
                if (curY >= height)
                {
                    ended = 1;
                    return 0;
                }
            }

            /* ABSOLUTE MODE */
            else
            {
                int count = c2;
                int padding = (count & 1);

                for (int i = 0; i < count; i++)
                {
                    int val = nextChar();
                    if (val == EOF)
                    {
                        rleError("Fehler: ABS EOF");
                        return -1;
                    }

                    if (curX < width)
                        row[curX] = (uint8_t)val;

                    curX++;
                }

                if (padding)
                    nextChar();
            }
        }

        /* Zeile voll → nächste Zeile */
        if (curX >= width)
        {
            curY++;
            return 0;
        }
    }

    return 0;
}
