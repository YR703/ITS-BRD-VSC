#include "rle_decoder.h"
#include "input.h"
#include <stdio.h>

static int ended = 0;

void rle_reset(void)
{
    ended = 0;
}

int rle_decode_row(uint8_t *row, int width)
{
    if (ended) 
    {
        for (int i = 0; i < width; i++)
            row[i] = 0;
        return 1;
    }

    int x = 0;
    int done = 0;

    while (!done)
    {
        int c1 = nextChar();
        int c2 = nextChar();

        if (c1 == EOF || c2 == EOF) 
            return -1;

        if (c1 > 0)
        {
            for (int i = 0; i < c1 && x < width; i++)
                row[x++] = (uint8_t)c2;

            if (x >= width)
                done = 1;
        }
        else
        {
            if (c2 == 0)
            {
                while (x < width)
                    row[x++] = 0;
                done = 1;
            }
            else if (c2 == 1)
            {
                ended = 1;
                while (x < width)
                    row[x++] = 0;
                done = 1;
            }
            else if (c2 == 2)
            {
                nextChar();
                nextChar();
            }
            else
            {
                int count = c2;
                for (int i = 0; i < count; i++)
                {
                    int v = nextChar();
                    if (v == EOF) return -1;
                    if (x < width)
                        row[x++] = (uint8_t)v;
                }
                if (count & 1)
                    nextChar();
            }
        }
    }

    return 0;
}
