#ifndef RLE_DECODER_H
#define RLE_DECODER_H

#include "BMP_types.h"

// Decodes one RLE-8 pixel index from the stream
// Returns: the palette index for the next pixel
int rle_get_next_pixel(int *x, int *y, int width, int height);

#endif
