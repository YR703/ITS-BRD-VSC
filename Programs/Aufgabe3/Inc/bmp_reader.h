#ifndef BMP_READER_H
#define BMP_READER_H

#include <stdint.h>
#include "BMP_types.h"

int bmp_start(BITMAPFILEHEADER *fh, BITMAPINFOHEADER *ih, RGBQUAD *pal);
int bmp_read_row(uint8_t *row, int width);
void bmp_reset(void);

#endif
