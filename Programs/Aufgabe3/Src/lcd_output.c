/**
 * Teilaufgabe B – Faster LCD Output (line buffering)
 *
 * We collect one full LCD line (row of pixels) in lineBuffer[]
 * and then send it all at once to the LCD using GUI_WriteLine().
 *
 * This is MUCH faster than drawing pixel by pixel.
 */

#include "lcd_output.h"
#include "LCD_GUI.h"        // GUI_WriteLine()
#include "MS_basetypes.h"
#include "lcd.h"

// Maximum width of the LCD (problem statement)
#define LCD_WIDTH   480

// Static line buffer
static unsigned short lineBuffer[LCD_WIDTH];

// Track the current line we are filling
static int currentLineY = 0;


/**
 * @brief Start drawing a new line (row Y on the LCD)
 */
void lcd_start_new_line(int y)
{
    currentLineY = y;

    // Optional: clear buffer (not needed, but nice for beginners)
    for (int i = 0; i < LCD_WIDTH; i++)
    {
        lineBuffer[i] = 0;
    }
}


/**
 * @brief Put one pixel into our line buffer
 * @param x     X position (0..479)
 * @param color Color in RGB565 format
 */
void lcd_set_pixel_in_line(int x, unsigned short color)
{
    if (x < 0 || x >= LCD_WIDTH)
        return;     // out of bounds → ignore

    lineBuffer[x] = color;
}


/**
 * @brief Send the whole line to the LCD at once.
 * @param y     Y coordinate
 * @param width Number of valid pixels in the line
 */
void lcd_write_line(int y, int width)
{
    if (width < 1) return;
    if (width > LCD_WIDTH) width = LCD_WIDTH;

    Coordinate pos;
    pos.x = 0;
    pos.y = y;

    // GUI_WriteLine draws one full row of pixels fast
    GUI_WriteLine(pos, width, lineBuffer);
}
