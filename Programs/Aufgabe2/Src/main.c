#include <stdio.h>
#include <stdbool.h>
#include "input.h"
#include "berechnung.h"
#include "output.h"
#include "error_drehgeber.h"

// ------------------------------------------------------------
// Simulation-only helpers
// ------------------------------------------------------------
void initITSboard() {
    printf("ITS Board Simulation Initialized.\n");
}
void initTimer() {
    printf("Timer Simulation Started.\n");
}

// ------------------------------------------------------------
// Main simulation program
// ------------------------------------------------------------
int main(void) {
    int input = 0, output = 0, phasen = 0, reset = 0, zeit = 0, sumphasen = 0;
    double winkel = 0, geschw = 0;

    initITSboard();
    initTimer();

    printf("\n=== Drehgeber (Polling) Simulation ===\n");
    printf("Enter S0/S1 states manually to simulate rotation.\n");
    printf("Use Ctrl+C to stop.\n\n");

    while (1) {
        zeit = inputzeit();
        input = input_einlesen();     // ask user or simulate S0/S1
        reset = resetpressed();       // check simulated reset button

        phasen = get_phasen_per_second(zeit);
        sumphasen = getphasen();
        winkel = get_winkel();
        geschw = get_winkelgeschw();
        output = phasen_ueberpruefung(input, reset);

        if (zeit == 1) {
            printf("Phases: %d | Sum: %d | Angle: %.2f° | Speed: %.2f°/s\n",
                   phasen, sumphasen, winkel, geschw);
        }

        if (output == PHASEUEBERSPRUNGEN) {
            printf("⚠️  Phase error detected!\n");
            error_number(output);
        }
    }

    return 0;
}
