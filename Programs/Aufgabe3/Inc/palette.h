#ifndef PALETTE_H
#define PALETTE_H

#include "BMP_types.h"
#include "MS_basetypes.h"

// Load palette entries (RGBQUAD array)
void palette_load(RGBQUAD *pal, int size);

// Convert palette index to 16-bit LCD color
unsigned short palette_get_color(int index);

#endif
