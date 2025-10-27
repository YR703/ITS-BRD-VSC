/**
 * @file stack.h
 * @author 
 * @brief Stack-Modul für den RPN-Taschenrechner.
 *        Dieses Modul implementiert die Stack-Verwaltung
 *        inklusive Push, Pop, Clear, Print, Duplicate und Swap.
 */

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

/**
 * @brief Entfernt den obersten Wert vom Stack und speichert ihn in *tmp.
 * @param tmp Pointer zur Speicherung des Werts.
 * @return STACK_OK oder Fehlercode.
 */
int pop(int *tmp);

/**
 * @brief Gibt den obersten Wert des Stacks aus, ohne ihn zu entfernen.
 * @return STACK_OK oder Fehlercode.
 */
int printTop(void);

/**
 * @brief Gibt den gesamten Stack-Inhalt aus, ohne ihn zu verändern.
 * @return STACK_OK oder Fehlercode.
 */
int printAll(void);

/**
 * @brief Löscht alle Einträge im Stack.
 */
void clearStack(void);

/**
 * @brief Dupliziert den obersten Eintrag des Stacks.
 * @return STACK_OK oder Fehlercode.
 */
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
