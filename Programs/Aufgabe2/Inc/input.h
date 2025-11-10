#ifndef _INPUT_H
#define _INPUT_H

#include <stdbool.h>

/**
 * @brief  Diese Funktion liest die Eingabe ein
 */
int input_einlesen();

/**
 * @brief  Diese Funktion Schaut ob 1 Sekunde vergangen ist
 */
int inputzeit();

/**
 * @brief  Diese Funktion gibt zurück ob S7 gedrückt wurde
 */
bool resetpressed();

/**
 * @brief  Diese Funktion gibt die Zeit zurück
 */
unsigned int get_Time();

#endif