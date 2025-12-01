/**
 * Teilaufgabe A – BMP Reader
 *
 * This file reads the BMP headers, palette, and pixel data,
 * and draws the image pixel-by-pixel on the LCD.
 * 
 * Beginner-friendly, simple implementation.
 */

#include "bmp_reader.h"
#include "headers.h"       // basicChecks()
#include "input.h"         // COMread(), nextChar()
#include "palette.h"       // palette_load(), palette_get_color()
#include "rle_decoder.h"   // rle_get_next_pixel()
#include "lcd.h"           // GUI_drawPoint
#include "errorhandler.h"
#include <stdio.h>
#include "LCD_GUI.h"

#define LCD_MAX_X  480
#define LCD_MAX_Y  320


// Local copies of the BMP headers
static BITMAPFILEHEADER fileHeader;
static BITMAPINFOHEADER fileInfo;


/**
 * @brief  Reads one BMP file and draws it pixel by pixel.
 *         (Teilaufgabe A – no line buffering, no scaling)
 */
void bmp_read_and_draw(void)
{
    // Clear display first
    GUI_clear(WHITE);

    // 1. Request next file from Python sender
    openNextFile();

    // 2. Read BITMAPFILEHEADER (14 bytes)
    ERR_HANDLER(
        1 != COMread((char*)&fileHeader, sizeof(BITMAPFILEHEADER), 1),
        "Cannot read BITMAPFILEHEADER"
    );

    // 3. Read BITMAPINFOHEADER (40 bytes)
    ERR_HANDLER(
        1 != COMread((char*)&fileInfo, sizeof(BITMAPINFOHEADER), 1),
        "Cannot read BITMAPINFOHEADER"
    );

    // 4. Validate headers (provided by professor)
    basicChecks(&fileHeader, &fileInfo);

    // We only support 8-bit RLE compressed BMPs
    if (fileInfo.biBitCount != 8 || fileInfo.biCompression != BI_RLE8)
    {
        lcdPrintS("Only 8-bit RLE BMP supported in Teil A");
        return;
    }

    // 5. Read color palette
    int paletteSize = fileInfo.biClrUsed;
    if (paletteSize == 0)
        paletteSize = 256;        // default palette size for 8-bit BMP

    RGBQUAD paletteEntries[256];

    ERR_HANDLER(
        1 != COMread((char*)paletteEntries, paletteSize * sizeof(RGBQUAD), 1),
        "Cannot read palette"
    );

    // load palette into palette.c storage
    palette_load(paletteEntries, paletteSize);

    // Starting pixel position (BMPs are stored bottom-up)
    int x = 0;
    int y = fileInfo.biHeight - 1;     // bottom line

    // 6. Decode and draw all pixels
    while (1)
    {
        int colorIndex = rle_get_next_pixel(&x, &y, fileInfo.biWidth, fileInfo.biHeight);

        if (colorIndex == -1)
        {
            // End of bitmap
            break;
        }

        // Convert palette index → LCD 16-bit color
        unsigned short col565 = palette_get_color(colorIndex);

        // We only draw if inside LCD bounds
        if (x >= 0 && x < LCD_MAX_X && y >= 0 && y < LCD_MAX_Y)
        {
            Coordinate pos = { x, LCD_MAX_Y - 1 - y };  
            // Flip vertically for BMP bottom-up

            GUI_drawPoint(pos, col565, DOT_PIXEL_1X1, DOT_FILL_AROUND);
        }
    }

    // Finished drawing
    lcdPrintS("BMP loaded (Teil A)");
}
