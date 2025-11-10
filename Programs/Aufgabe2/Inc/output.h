#ifndef _OUTPUT_H
#define _OUTPUT_H

#include "terminal.h"
#include "stm32f4xx_hal.h"
#include "gpio.h"


/**
 * @brief  Diese Funktion gibt die Phasen auf dem Led aus
 */
void print_led();

/**
 * @brief  Diese Funktion wandelt ein Double in String um
 */
char * double_to_string(double winkel,char* geschw);

/**
 * @brief  Diese Funktion zeigt dass sich die Phasen nach hinten bewegen
 */
void led_vorwaerts();

/**
 * @brief  Diese Funktion zeigt dass sich die Phasen nach vorne bewegen
 */
void led_rueckwaerts();

/**
 * @brief  Diese Funktion zeigt den Fehlerzustand
 */
void led_fehler();

/**
 * @brief  Diese Funktion zeigt dass sich die Phasen nicht gäendert haben
 */
void led_keine_aenderung();

/**
 * @brief  Diese Funktion gibt den Winkel und die Winkelgeschwindigkeit auf dem Board aus
 */
void print_lcdboard(int phasen, int sumphasen, int winkel, double geschw);
;

/**
 * @brief  Diese Funktion löscht den Fehlerzustand
 */
void led_fehler_reset();

/**
 * @brief  Diese Funktion setzt den Counter zurück
 */
void reset_counter();
#endif