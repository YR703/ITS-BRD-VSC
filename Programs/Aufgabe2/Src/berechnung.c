#include "berechnung.h"
#include "output.h"
#include "error_drehgeber.h"
#include "berechnung.h"
#include "timer.h"

// Definition of the state transition table
StateType delta[14][4] = {
  // Start
  {ANoRot, DNoRot, BNoRot, CNoRot},
  // Err
  {Err, Err, Err, Err},
  // AForRot
  {AForRot, DBackRot, BForRot, Err},
  // ABackRot
  {ABackRot, DBackRot, BForRot, Err},
  // ANoRot
  {ANoRot, DBackRot, BForRot, Err},
  // BForRot
  {ABackRot, Err, BForRot, CForRot},
  // BBackRot
  {ABackRot, Err, BBackRot, CForRot},
  // BNoRot
  {ABackRot, Err, BNoRot, CForRot},
  // CForRot
  {Err, DForRot, BBackRot, CForRot},
  // CBackRot
  {Err, DForRot, BBackRot, CBackRot},
  // CNoRot
  {Err, DForRot, BBackRot, CNoRot},
  // DForRot
  {AForRot, DForRot, Err, CBackRot},
  // DBackRot
  {AForRot, DBackRot, Err, CBackRot},
  // DNoRot
  {AForRot, DNoRot, Err, CBackRot}
};


int state = Start;
int prev = 0;
int phasen = 0;
double winkel = 0;
double geschw = 0;

void reset() {
    prev = 0;
    phasen = 0;
    state = Start;
}

int phasen_ueberpruefung(int input, int resetpressed) {
    state = delta[state][input];
    if (state == prev) return 1;
    prev = state;
    if (resetpressed) reset();

    switch (state) {
        case Err:
            led_keine_aenderung();
            return PHASEUEBERSPRUNGEN;
        case AForRot: case BForRot: case CForRot: case DForRot:
            led_vorwaerts();
            phasen++;
            break;
        case ABackRot: case BBackRot: case CBackRot: case DBackRot:
            led_rueckwaerts();
            phasen++;
            break;
    }
    return 0;
}

int getphasen() { return phasen; }

double get_winkel() { return phasen * SCHLITZE; }

double get_winkelgeschw(uint32_t timer_ticks, double winkel, bool change)
{
    static uint32_t alt_zeit = 0;
    static double alt_winkel = 0.0;

    double zeit_diff = (timer_ticks - alt_zeit) / (TICKS_PER_US * 1000 * 1000.0);
    if((change && zeit_diff > 0.25) || zeit_diff > 0.5)
    {
        double winkel_diff = winkel - alt_winkel;
        if(zeit_diff != 0)
        {
            geschw = winkel_diff / zeit_diff;
        }

        alt_winkel = winkel;
        alt_zeit = timer_ticks;
    }

    return geschw;
}
