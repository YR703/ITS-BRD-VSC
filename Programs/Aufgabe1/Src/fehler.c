#include "fehler.h"
#include "display.h"

void errorHandler(int err) { //rote schrift aktiveren-->Fehler
    setErrMode();

    switch (err) {
        case ERR_STACK_EMPTY:
            printStdout("Fehler: Stack ist leer.\n");
            break;

        case ERR_STACK_FULL:
            printStdout("Fehler: Stack ist voll.\n");
            break;

        case ERR_DIV_ZERO:
            printStdout("Fehler: Division durch 0!\n");
            break;

        case ERR_OVERFLOW:
            printStdout("Fehler: Bereichsüberschreitung!\n");
            break;

        case ERR_UNKNOWN:
            printStdout("Fehler: Unbekannter Fehler!\n");
            break;

        default:  //Falls kein Fehler→ normalmodus wiederherstellen
            setNormalMode();
            break;
    }
}
