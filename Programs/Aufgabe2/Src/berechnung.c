#include "berechnung.h"
#include "output.h"
#include "error_drehgeber.h"

static int phasen = 0;
static double winkel = 0.0;
static double geschw = 0.0;

int phasen_ueberpruefung(int input, int reset) {
    static int lastInput = -1;

    if (reset) {
        phasen = 0;
        lastInput = -1;
        return 0;
    }

    if (lastInput == -1) {
        lastInput = input;
        return 0;
    }

    if ((lastInput + 1) % 4 == input)
        led_vorwaerts();
    else if ((input + 1) % 4 == lastInput)
        led_rueckwaerts();
    else
        return PHASEUEBERSPRUNGEN;

    phasen++;
    lastInput = input;
    return 0;
}

int getphasen(void) { return phasen; }

double get_winkel(void) {
    winkel = phasen * 0.3;
    return winkel;
}

double get_winkelgeschw(void) {
    geschw = phasen * 0.3;
    return geschw;
}
