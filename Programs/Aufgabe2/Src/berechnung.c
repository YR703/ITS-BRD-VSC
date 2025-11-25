#include "berechnung.h"
#include "output.h"
#include "error_drehgeber.h"
#include "timer.h"
#include <math.h>

// Zustandsübergangstabelle
// Vereinfachte Zustandsmatrix für 14 Zustände und 4 Eingaben
const StateType delta[14][4] = {
    // Start und Fehler
    [Start]     = { ANoRot, DNoRot, BNoRot, CNoRot },
    [Err]       = { Err, Err, Err, Err },

    // Vorwärtsrotation
    [AForRot]   = { AForRot, DBackRot, BForRot, Err },
    [BForRot]   = { ABackRot, Err, BForRot, CForRot },
    [CForRot]   = { Err, DForRot, BBackRot, CForRot },
    [DForRot]   = { AForRot, DForRot, Err, CBackRot },

    // Rückwärtsrotation
    [ABackRot]  = { ABackRot, DBackRot, BForRot, Err },
    [BBackRot]  = { ABackRot, Err, BBackRot, CForRot },
    [CBackRot]  = { Err, DForRot, BBackRot, CBackRot },
    [DBackRot]  = { AForRot, DBackRot, Err, CBackRot },

    // Keine Bewegung (No Rotation)
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

void reset_system(void) {
    prev = 0;
    phasen = 0;
    state = Start;
    geschw = 0;
    winkel = 0;
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
            led_fehler();         // D21 EIN
            led_keine_aenderung(); // D22 + D23 AUS bei Fehler
            return PHASEUEBERSPRUNGEN;

        case AForRot: case BForRot: case CForRot: case DForRot:
            led_vorwaerts();      // D23 AN, D22 AUS
            phasen++;
            break;

        case ABackRot: case BBackRot: case CBackRot: case DBackRot:
            led_rueckwaerts();    // D22 AN, D23 AUS
            phasen--;
            break;

        default:
            break;
    }
    return 0;
}

int getphasen(void) { return phasen; }

double get_winkel(void) { return fabs(phasen * SCHLITZE); }

double get_winkelgeschw(uint32_t timer_ticks, double winkel, bool change)
{
    static uint32_t alt_zeit = 0;
    static double alt_winkel = 0.0;
    static double geschw_filter = 0.0;

    // Zeitdifferenz in Sekunden berechnen
    double zeit_diff = (timer_ticks - alt_zeit) / (TICKS_PER_US * 1e6);

    // Nur alle 100 ms aktualisieren
    if (zeit_diff < 0.25) return geschw_filter;

    double winkel_diff = fabs(winkel - alt_winkel);

    // Geschwindigkeit berechnen
    double geschw_neu = winkel_diff / zeit_diff;

    // Sanft glätten (80 % alte, 20 % neue Werte)
    geschw_filter = (0.8 * geschw_filter) + (0.2 * geschw_neu);

    // Begrenzen auf sinnvolle Werte
    if (geschw_filter > 300) geschw_filter = 300;
    if (geschw_filter < 0.05) geschw_filter = 0;

    alt_winkel = winkel;
    alt_zeit = timer_ticks;
    return geschw_filter;
}
