#include "palette.h"
#include "input.h"
#include "errorhandler.h"

int load_palette(RGBQUAD *pal, int count)
{
    for(int i = 0; i < count; i++)
    {
        if (1 != COMread((char*)&pal[i], sizeof(RGBQUAD), 1))
            return NOK;
    }
    return EOK;
}
