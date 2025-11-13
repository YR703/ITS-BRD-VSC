#ifndef _BERECHNUNG_H
#define _BERECHNUNG_H

#define SCHLITZE 0.3

#define PHASE_A 0x00
#define PHASE_B 0x02
#define PHASE_C 0x03
#define PHASE_D 0x01

typedef enum {
  Start = 0, Err = 1, AForRot = 2, ABackRot = 3, ANoRot = 4,
  BForRot = 5, BBackRot = 6, BNoRot = 7, CForRot = 8, CBackRot = 9,
  CNoRot = 10, DForRot = 11, DBackRot = 12, DNoRot = 13
} StateType;

extern StateType delta[14][4];

int phasen_ueberpruefung(int input, int reset);
void reset();
int get_phasen_per_second(int zeit);
int getphasen();
double get_winkel();
double get_winkelgeschw();

#endif
