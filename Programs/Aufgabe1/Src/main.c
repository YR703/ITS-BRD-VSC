#include "display.h"
#include "scanner.h"
#include "stack.h"
#include "calc.h"
#include "fehler.h"
#include "token.h"
#include "init.h"
#include "stm32f4xx_hal.h"



int main(void) {
    initITSboard();    // Initialisiert Hardware 
    initDisplay();     // Initialisiert Displaybereich
    clearStack();      // Stack auf Anfangszustand setzen

    T_token token;
    int err;

    while (1) {  // Nächstes Token vom Touchpad lesen
        
        token = nextToken();

        //Berechnung durchführen
        err = calc(token);

        //Falls Fehlercode zurückkommt→Fehler anzeigen
        if (err != ERR_OK) {
            errorHandler(err);
        }

        //Kleine Pause, damit Display stabil bleibt
        HAL_Delay(100);
    }
}
