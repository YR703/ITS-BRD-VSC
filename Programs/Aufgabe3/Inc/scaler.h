#ifndef SCALER_H
#define SCALER_H

#include <stdint.h>
#include "BMP_types.h"

void scale_image(uint8_t **rows, int srcW, int srcH, RGBQUAD *pal);

#endif
