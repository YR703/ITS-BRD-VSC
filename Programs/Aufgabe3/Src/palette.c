/**
 * Teilaufgabe A – Palette Handling
 *
 * Loads the BMP color palette (RGBQUAD array)
 * and converts a palette index into a 16-bit LCD color.
 *
 * Beginner-friendly version: simple and readable.
 */

#include "palette.h"
#include "errorhandler.h"
#include <stdio.h>

// Local palette storage (max 256 entries for 8-bit BMP)
static RGBQUAD paletteLocal[256];
static int paletteCount = 0;


/**
 * @brief Store the palette provided by bmp_reader.c.
 *
 * @param pal  Pointer to array of RGBQUAD (from COMread)
 * @param size Number of colors (usually 256 or biClrUsed)
 */
void palette_load(RGBQUAD *pal, int size)
{
    if (size < 1) size = 256;
    if (size > 256) size = 256;

    paletteCount = size;

    for (int i = 0; i < size; i++)
    {
        paletteLocal[i] = pal[i];
    }
}


/**
 * @brief Convert an 8-bit palette index to 16-bit RGB565.
 *
 * The LCD expects:
 *   RRRRR GGGGGG BBBBB
 *   bits: 15..11 10..5  4..0
 *
 * @param index Palette entry (0..255)
 * @return 16-bit color suitable for LCD
 */
unsigned short palette_get_color(int index)
{
    if (index < 0) index = 0;
    if (index >= paletteCount) index = paletteCount - 1;

    RGBQUAD c = paletteLocal[index];

    // Extract RGB from BMP palette (B,G,R order!)
    unsigned char blue  = c.rgbBlue;
    unsigned char green = c.rgbGreen;
    unsigned char red   = c.rgbRed;

    // Convert to 5-6-5 format
    unsigned short r5 = (red   >> 3) & 0x1F; 
    unsigned short g6 = (green >> 2) & 0x3F;
    unsigned short b5 = (blue  >> 3) & 0x1F;

    // Combine into 16-bit color
    unsigned short color565 =
            (r5 << 11) |
            (g6 << 5)  |
            (b5);

    return color565;
}
