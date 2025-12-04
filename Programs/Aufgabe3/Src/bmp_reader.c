#include "bmp_reader.h"
#include "headers.h"
#include "palette.h"
#include "rle_decoder.h"
#include "input.h"
#include "errorhandler.h"
#include <stdio.h>


static int bitCount = 0;
static int compression = 0;

void bmp_reset(void)
{
    rle_reset();
}

int bmp_start(BITMAPFILEHEADER *fh, BITMAPINFOHEADER *ih, RGBQUAD *pal)
{
    if (EOK != readHeaders())
        return NOK;

    getFileHeader(fh);
    getInfoHeader(ih);

    bitCount = ih->biBitCount;
    compression = ih->biCompression;

    int palCount = ih->biClrUsed ? ih->biClrUsed : 256;

    if (bitCount == 8)
    {
        if (EOK != load_palette(pal, palCount))
            return NOK;
    }

    rle_reset();
    return EOK;
}

int bmp_read_row(uint8_t *row, int width)
{
    if (bitCount == 8)
    {
        if (compression == BI_RLE8)
        {
            return rle_decode_row(row, width);
        }
        else
        {
            for (int i = 0; i < width; i++)
            {
                int c = nextChar();
                if (c == EOF) return -1;
                row[i] = (uint8_t)c;
            }
            return 0;
        }
    }

    return -1;
}
