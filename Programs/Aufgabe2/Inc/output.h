#ifndef _OUTPUT_H
#define _OUTPUT_H

#include "terminal.h"

void print_led();
char* double_to_string(double winkel, char* geschw);
void led_vorwaerts();
void led_rueckwaerts();
void led_fehler();
void led_keine_aenderung();
void led_fehler_reset();
void print_lcdboard(int phasen, int sumphasen, int winkel, double geschw);
void reset_counter();

#endif
