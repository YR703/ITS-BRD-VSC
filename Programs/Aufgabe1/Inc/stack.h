

#ifndef STACK_H
#define STACK_H

//#include "display.h"
#include <stdbool.h>

// Stack-Konfiguration
#define STACK_SIZE 25

// Fehlercodes
#define STACK_OK                0
#define STACK_IST_LEER         -1
#define STACK_IST_VOLL         -2
#define DIVISION_DURCH_NULL    -3
#define BEREICHSUEBERSCHREITUNG -4

/**
 * @brief Legt einen Wert oben auf den Stack.
 * @param wert Der zu speichernde Wert.
 * @return STACK_OK oder Fehlercode.
 */
int push(int wert);


int pop(int *tmp);


int printTop(void);


int printAll(void);


void clearStack(void);


int duplicateTop(void);

/**
 * @brief Vertauscht die beiden obersten Einträge.
 * @return STACK_OK oder Fehlercode.
 */
int swapTop(void);

/**
 * @brief Prüft auf Overflow / Underflow während Rechenoperationen.
 */
int checkOverflow(long long result);

#endif
// EOF
