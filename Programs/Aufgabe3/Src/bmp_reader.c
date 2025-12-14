#include "bmp_reader.h"
#include "headers.h"
#include "input.h"
#include "errorhandler.h"
#include "LCD_GUI.h"
#include "fontsFLASH.h"
#include <stdio.h>

// -----------------------------------------------------------------------
// DEBUG & OVERRIDE SETTINGS
// -----------------------------------------------------------------------
#define FORCE_RLE_DECODE 0

// -----------------------------------------------------------------------
// STATIC STATE
// -----------------------------------------------------------------------
static int g_width = 0;
static int g_compression = 0;
static int g_bitCount = 0;
static bool g_rle_eof = false;

// -----------------------------------------------------------------------
// RESET
// -----------------------------------------------------------------------
void bmp_reset(void)
{
    g_width = 0;
    g_compression = 0;
    g_bitCount = 0;
    g_rle_eof = false;
}

// -----------------------------------------------------------------------
// START & HEADER ANALYSE
// -----------------------------------------------------------------------
int bmp_start(BITMAPFILEHEADER *fh, BITMAPINFOHEADER *ih, RGBQUAD *pal)
{
    bmp_reset();
    char debugBuf[64];

    // 1. Header lesen (nutzt COMread intern)
    if (readHeaders() != EOK) {
        lcdErrorMsg("Header Read Error");
        return NOK;
    }

    getFileHeader(fh);
    getInfoHeader(ih);

    g_width       = ih->biWidth;
    g_compression = ih->biCompression;
    g_bitCount    = ih->biBitCount;

    // -------------------------------------------------------------
    // FEHLER 1: Nur 8 Bit BMP unterstützt
    // -------------------------------------------------------------
    if (g_bitCount != 8)
    {
        lcdErrorMsg("Fehler: Nur 8-Bit BMP!");
        return NOK;
    }

    // -------------------------------------------------------------
    // FEHLER 2: Nur RLE8 (BI_RLE8) erlaubt
    // -------------------------------------------------------------
    if (g_compression != BI_RLE8)
    {
        lcdErrorMsg("Fehler: Nicht RLE8 komprimiert!");
        return NOK;
    }

    // -------------------------------------------------------------
    // DEBUG-AUSGABE AUF LCD
    // -------------------------------------------------------------
    GUI_clear(BLACK);
    snprintf(debugBuf, sizeof(debugBuf),
             "W:%d H:%d C:%d B:%d",
             (int)ih->biWidth, (int)ih->biHeight,
             (int)ih->biCompression, (int)ih->biBitCount);

    Coordinate pos = {0, 0};
    GUI_disStr(pos, debugBuf, &Font16, WHITE, RED);

    // -------------------------------------------------------------
    // PALETTE LESEN
    // -------------------------------------------------------------
    int colors = ih->biClrUsed;
    if (colors == 0) colors = 256;

    for (int i = 0; i < colors; i++)
    {
        int b   = nextChar();
        int g   = nextChar();
        int r   = nextChar();
        int res = nextChar();

        if (b == EOF || g == EOF || r == EOF)
        {
            lcdErrorMsg("EOF in Palette");
            return NOK;
        }

        pal[i].rgbBlue     = (uint8_t)b;
        pal[i].rgbGreen    = (uint8_t)g;
        pal[i].rgbRed      = (uint8_t)r;
        pal[i].rgbReserved = (uint8_t)res;
    }

    // -------------------------------------------------------------
    // GAP SKIPPING — zum Datenoffset springen
    // -------------------------------------------------------------
    int currentBytesRead = 14 + 40 + (colors * 4);
    int targetOffset     = fh->bfOffBits;
    int gap              = targetOffset - currentBytesRead;

    if (gap < 0)
    {
        lcdErrorMsg("Fehler: Negatives Gap");
        return NOK;
    }

    for (int i = 0; i < gap; i++)
    {
        if (nextChar() == EOF)
        {
            lcdErrorMsg("EOF in Gap");
            return NOK;
        }
    }

    return EOK;
}

// -----------------------------------------------------------------------
// RLE8 DECODER (KORRIGIERT)
// -----------------------------------------------------------------------
static int read_row_rle(uint8_t *row, int width)
{
    if (g_rle_eof)
        return 0;

    int x = 0;

    while (1)
    {
        int b1 = nextChar();
        int b2 = nextChar();

        if (b1 == EOF || b2 == EOF)
            return -1;

        // ENCODED MODE
        if (b1 > 0)
        {
            int count = b1;
            int colorIndex = b2;

            for (int i = 0; i < count; i++)
            {
                if (x < width)
                    row[x] = (uint8_t)colorIndex;
                x++;
            }
        }
        else
        {
            // ESCAPE MODE
            if (b2 == 0)
                return 0; // End of line

            if (b2 == 1)
            {
                g_rle_eof = true;
                return 0;
            }

            if (b2 == 2)
            {
                int dx = nextChar();
                int dy = nextChar();

                x += dx;
                // dy wird ignoriert (BMP bottom-up row-wise streaming)
            }
            else
            {
                // ABSOLUTE MODE
                int count = b2;

                for (int i = 0; i < count; i++)
                {
                    int val = nextChar();
                    if (val == EOF) return -1;

                    if (x < width)
                        row[x] = (uint8_t)val;
                    x++;
                }

                // Padding
                if (count % 2 != 0)
                    nextChar();
            }
        }
    }
}

// -----------------------------------------------------------------------
// RAW READER (falls nicht RLE) — wird nicht benutzt, aber bleibt drin
// -----------------------------------------------------------------------
static int read_row_raw(uint8_t *row, int width)
{
    for (int i = 0; i < width; i++)
    {
        int c = nextChar();
        if (c == EOF)
            return -1;

        row[i] = (uint8_t)c;
    }

    int padding = (4 - (width % 4)) % 4;

    for (int i = 0; i < padding; i++)
    {
        if (nextChar() == EOF)
            return -1;
    }

    return 0;
}

// -----------------------------------------------------------------------
// MAIN READ FUNCTION
// -----------------------------------------------------------------------
int bmp_read_row(uint8_t *row, int width)
{
    if (g_compression == BI_RLE8)
        return read_row_rle(row, width);

    return read_row_raw(row, width);
}
