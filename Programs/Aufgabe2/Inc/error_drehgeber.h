#ifndef _ERROR_H
#define _ERROR_H

#include "terminal.h"

#define PHASEUEBERSPRUNGEN - 1

/**
 * @brief  Diese Funktion geht mit dem Fehler um und bleibt solange dort bis der Fehler zurückgesetzt wird
 */

int error_number(int fehler);

#endif