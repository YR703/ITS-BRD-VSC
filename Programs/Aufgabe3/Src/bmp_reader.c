#include "bmp_reader.h"
#include "headers.h"
#include "palette.h"
#include "rle_decoder.h"
#include "input.h"
#include "errorhandler.h"
#include "LCD_GUI.h"
#include "fontsFLASH.h"

#include <stdio.h>

static int bitCount = 0;
static int compression = 0;
static int dataOffset = 0;
static int bmpHeight = 0;

void bmp_reset(void)
{
    rle_reset();
}

int bmp_start(BITMAPFILEHEADER *fh, BITMAPINFOHEADER *ih, RGBQUAD *pal)
{
    if (EOK != readHeaders())
    {
        lcdErrorMsg("Fehler: Header lesen");
        return NOK;
    }

    getFileHeader(fh);
    getInfoHeader(ih);

    /* BMP Signatur prüfen */
    if (fh->bfType != 0x4D42)
    {
        lcdErrorMsg("Fehler: Signatur != BM");
        return NOK;
    }

    /* Nur BITMAPINFOHEADER erlaubt */
    if (ih->biSize != 40)
    {
        lcdErrorMsg("Fehler: Headerdefekt");
        return NOK;
    }

    /* Nur 8 Bit erlaubt */
    if (ih->biBitCount != 8)
    {
        lcdErrorMsg("Fehler: BitTiefe != 8");
        return NOK;
    }

    /* Nur RLE8 erlaubt */
    if (ih->biCompression != BI_RLE8)
    {
        lcdErrorMsg("Fehler: keine RLE8 Kodierung");
        return NOK;
    }

    /* Nur eine Ebene erlaubt */
    if (ih->biPlanes != 1)
    {
        lcdErrorMsg("Fehler: biPlanes != 1");
        return NOK;
    }

    /* Palette prüfen */
    if (ih->biClrUsed > 256)
    {
        lcdErrorMsg("Fehler: Palette > 256");
        return NOK;
    }

    /* BMP-Höhe prüfen */
    if (ih->biHeight <= 0)
    {
        lcdErrorMsg("Fehler: ungueltige Hoehe");
        return NOK;
    }

    bmpHeight = ih->biHeight;

    /* bfOffBits prüfen */
    if (fh->bfOffBits < 54 || fh->bfOffBits > 65536)
    {
        lcdErrorMsg("Fehler: bfOffBits ungueltig");
        return NOK;
    }

    bitCount = ih->biBitCount;
    compression = ih->biCompression;
    dataOffset = fh->bfOffBits;

    int palCount = ih->biClrUsed ? ih->biClrUsed : 256;

    if (bitCount == 8)
    {
        if (EOK != load_palette(pal, palCount))
        {
            lcdErrorMsg("Fehler: Palette defekt");
            return NOK;
        }
    }

    /* RLE Dekodierer auf Bildhöhe einstellen */
    rle_reset_with_height(bmpHeight);

    /* OFFSET ANSPRINGEN */
    int headerSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + palCount * sizeof(RGBQUAD);
    int skip = dataOffset - headerSize;

    if (skip < 0)
    {
        lcdErrorMsg("Fehler: DataOffset");
        return NOK;
    }

    for (int i = 0; i < skip; i++)
        nextChar();

    return EOK;
}

int bmp_read_row(uint8_t *row, int width)
{
    if (bitCount == 8)
    {
        if (compression == BI_RLE8)
        {
            int res = rle_decode_row(row, width);
            if (res < 0)
            {
                lcdErrorMsg("Fehler: RLE Dekodierung");
                return -1;
            }
            return res;
        }
        else
        {
            /* Unkomprimiertes 8 Bit BMP (Padding!) */
            for (int i = 0; i < width; i++)
            {
                int c = nextChar();
                if (c == EOF)
                {
                    lcdErrorMsg("Fehler: Datenstrom kurz");
                    return -1;
                }
                row[i] = (uint8_t)c;
            }

            /* Zeilen-Padding */
            int pad = (4 - (width % 4)) % 4;
            for (int i = 0; i < pad; i++)
                nextChar();

            return 0;
        }
    }

    lcdErrorMsg("Fehler: falsches Format");
    return -1;
}
