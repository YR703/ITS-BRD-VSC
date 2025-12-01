#ifndef LCD_OUTPUT_H
#define LCD_OUTPUT_H

#include "MS_basetypes.h"

// Teilaufgabe B: faster line-buffer writing
void lcd_start_new_line(int y);
void lcd_set_pixel_in_line(int x, unsigned short color);
void lcd_write_line(int y, int width);

#endif
