/**
 * Teilaufgabe A – RLE-8 Decoder
 *
 * This module reads the pixel indexes from the BMP stream
 * using the RLE-8 compression rules.
 *
 * It returns ONE palette index per call.
 *
 * IMPORTANT:
 *  - (x, y) are updated inside the function
 *  - returns -1 when the bitmap is finished
 */

#include "rle_decoder.h"
#include "input.h"         // nextChar()
#include "errorhandler.h"
#include <stdio.h>

// internal state for absolute mode
static int absCount = 0;      // number of absolute pixel values left
static int absPaddingSkip = 0;


/**
 * @brief Get the next palette index from RLE-8 compressed BMP data.
 *
 * @param x Pointer to current x position
 * @param y Pointer to current y position
 * @param width BMP width in pixels
 * @param height BMP height in pixels
 *
 * @return palette index (0–255), or -1 when image is complete
 */
int rle_get_next_pixel(int *x, int *y, int width, int height)
{
    // -----------------------------------------
    // 1. If we are in absolute mode, continue
    // -----------------------------------------
    if (absCount > 0)
    {
        absCount--;

        int colorIndex = nextChar();

        // If absolute mode just finished, skip padding byte if needed
        if (absCount == 0 && absPaddingSkip)
        {
            nextChar();          // skip alignment byte
            absPaddingSkip = 0;
        }

        // Move pixel forward
        (*x)++;
        if (*x >= width)
        {
            *x = 0;
            (*y)--;

            if (*y < 0)
                return -1;
        }

        return colorIndex;
    }

    // ------------------------------
    // 2. Read the next RLE command
    // ------------------------------
    int count = nextChar();
    int value = nextChar();

    // Error: unexpected end
    if (count == EOF || value == EOF)
        return -1;

    // --------------------------------------------------
    // SPECIAL CASE: count == 0 → Escape codes
    // --------------------------------------------------
    if (count == 0)
    {
        // 00 00 → End of line
        if (value == 0)
        {
            *x = 0;
            (*y)--;

            if (*y < 0) return -1;      // Finished image

            return rle_get_next_pixel(x, y, width, height);
        }

        // 00 01 → End of bitmap
        else if (value == 1)
        {
            return -1;
        }

        // 00 02 dx dy → delta move
        else if (value == 2)
        {
            int dx = nextChar();
            int dy = nextChar();

            *x += dx;
            *y -= dy;

            if (*y < 0) return -1;

            return rle_get_next_pixel(x, y, width, height);
        }

        // 00 nn … → absolute mode
        else
        {
            absCount = value;               // number of literal pixels
            absPaddingSkip = (value & 1);   // if odd, skip 1 padding byte
            return rle_get_next_pixel(x, y, width, height);
        }
    }

    // --------------------------------------------------
    // ENCODED MODE: count > 0
    // meaning: repeat pixel 'value' count times
    // --------------------------------------------------
    int colorIndex = value;

    // Move position forward for THIS call
    (*x)++;

    if (*x >= width)
    {
        *x = 0;
        (*y)--;

        if (*y < 0) return -1;
    }

    // To handle the REMAINING count values, we “inject” them into absCount logic.
    absCount = count - 1;     // we already used one pixel of the run
    absPaddingSkip = 0;       // encoded mode never has padding

    return colorIndex;
}
