#include "berechnung.h"
#include "output.h"
#include "error_drehgeber.h"
#include "timer.h"
#include <math.h>

// Zustandsübergangstabelle
const StateType delta[14][4] = {
    [Start]     = { ANoRot, DNoRot, BNoRot, CNoRot },
    [Err]       = { Err, Err, Err, Err },

    [AForRot]   = { AForRot, DBackRot, BForRot, Err },
    [BForRot]   = { ABackRot, Err, BForRot, CForRot },
    [CForRot]   = { Err, DForRot, BBackRot, CForRot },
    [DForRot]   = { AForRot, DForRot, Err, CBackRot },

    [ABackRot]  = { ABackRot, DBackRot, BForRot, Err },
    [BBackRot]  = { ABackRot, Err, BBackRot, CForRot },
    [CBackRot]  = { Err, DForRot, BBackRot, CBackRot },
    [DBackRot]  = { AForRot, DBackRot, Err, CBackRot },

    [ANoRot]    = { ANoRot, DBackRot, BForRot, Err },
    [BNoRot]    = { ABackRot, Err, BNoRot, CForRot },
    [CNoRot]    = { Err, DForRot, BBackRot, CNoRot },
    [DNoRot]    = { AForRot, DNoRot, Err, CBackRot }
};

int state = Start;
int prev = 0;
int phasen = 0;
double winkel = 0;
double geschw = 0;

// interne Zeit- und Winkelvariablen für Geschwindigkeitsberechnung
static uint32_t alt_zeit = 0;
static double alt_winkel = 0.0;
static double geschw_filter = 0.0;

void reset_system(void) {
    prev = 0;
    phasen = 0;
    state = Start;
    winkel = 0;
    geschw = 0;
    geschw_filter = 0.0;
    alt_zeit = 0;
    alt_winkel = 0.0;
    led_keine_aenderung();
    led_fehler_reset();
}

int phasen_ueberpruefung(int input, int resetpressed) {
    state = delta[state][input];
    if (state == prev) return 1;
    prev = state;
    if (resetpressed) reset_system();

    switch (state) {
        case Err:
            led_fehler();
            led_keine_aenderung();
            return PHASEUEBERSPRUNGEN;

        case AForRot: case BForRot: case CForRot: case DForRot:
            led_vorwaerts();
            phasen++;
            break;

        case ABackRot: case BBackRot: case CBackRot: case DBackRot:
            led_rueckwaerts();
            phasen--;
            break;

        default:
            led_keine_aenderung();
            break;
    }
    return 0;
}

int getphasen(void) { return phasen; }

double get_winkel(void) { return fabs(phasen * SCHLITZE); }

double get_winkelgeschw(uint32_t timer_ticks, double winkel, bool change)
{
    double zeit_diff = (timer_ticks - alt_zeit) / (TICKS_PER_US * 1e6);
    if (zeit_diff < 0.25) return geschw_filter;

    double winkel_diff = fabs(winkel - alt_winkel);
    double geschw_neu = winkel_diff / zeit_diff;

    if (geschw_neu > 500) geschw_neu = 500;
    if (geschw_neu < 0.05) geschw_neu = 0;

    // Glättung: 70% alt, 30% neu
    geschw_filter = (0.7 * geschw_filter) + (0.3 * geschw_neu);

    alt_winkel = winkel;
    alt_zeit = timer_ticks;
    return geschw_filter;
}
