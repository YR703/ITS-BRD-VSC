#ifndef _BERECHNUNG_H
#define _BERECHNUNG_H

#define SCHLITZE 0.3

#define PHASE_A 0x00 // A = 0 B = 0
#define PHASE_B 0x02 // A = 1 B = 0
#define PHASE_C 0x03 // A = 1 B = 1
#define PHASE_D 0x01 // A = 0 B = 1


// Automatonverwaltet die Phaseuebergänge
// Kodierung der Zustände : <letzte Phase><Drehrichtung>
typedef enum {
  Start = 0, Err = 1, AForRot = 2, ABackRot = 3, ANoRot = 4, BForRot = 5,
    BBackRot = 6, BNoRot = 7, CForRot = 8, CBackRot = 9,
    CNoRot = 10, DForRot = 11, DBackRot = 12, DNoRot = 13
}
StateType;

static StateType delta[14][4] = {
   /* Start      */
  {
    ANoRot, //  PHASE_A
    DNoRot, //  PHASE_D
    BNoRot, //  PHASE_B
    CNoRot  //  PHASE_C
  },
  /* Err        */
  {
    Err,
    Err,
    Err,
    Err
  },

  /* AForRot    */
  {
    AForRot,
    DBackRot,
    BForRot,
    Err
  },
  /* ABackRot   */
  {
    ABackRot,
    DBackRot,
    BForRot,
    Err
  },
  /* ANoRot     */
  {
    ANoRot,
    DBackRot,
    BForRot,
    Err
  },

  /* BForRot    */
  {
    ABackRot,
    Err,
    BForRot,
    CForRot
  },
  /* BBackRot   */
  {
    ABackRot,
    Err,
    BBackRot,
    CForRot
  },
  /* BNoRot     */
  {
    ABackRot,
    Err,
    BNoRot,
    CForRot
  },

  /* CForRot    */
  {
    Err,
    DForRot,
    BBackRot,
    CForRot
  },
  /* CBackRot   */
  {
    Err,
    DForRot,
    BBackRot,
    CBackRot
  },
  /* CNoRot     */
  {
    Err,
    DForRot,
    BBackRot,
    CNoRot
  },

  /* DForRot    */
  {
    AForRot,
    DForRot,
    Err,
    CBackRot
  },
  /* DBackRot   */
  {
    AForRot,
    DBackRot,
    Err,
    CBackRot
  },
  /* DNoRot     */
  {
    AForRot,
    DNoRot,
    Err,
    CBackRot
  }
};

/**
 * @brief  Diese schaut sich den Input an und je nach Phasenübergang verhält es sich demenstprechend
 */

int phasen_ueberpruefung(int input, int reset);

/**
 * @brief  Diese Funktion setzt Phasenzähler zurück
 */
void reset();

/**
 * @brief  Diese Funktion gibt die Phasen pro Sekunde zurück 
 */
int get_phasen_per_second(int zeit);

/**
 * @brief  Diese Funtktion gibt die Phasen zurück 
 */
int getphasen();

/**
 * @brief  Diese Funtktion gibt den Winkel zurück 
 */
double get_winkel();

/**
 * @brief  Diese Funtktion gibt die Winkelgeschwindigkeit zurück 
 */
double get_winkelgeschw();
#endif