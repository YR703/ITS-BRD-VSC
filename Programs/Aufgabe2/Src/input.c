#include <stdbool.h>

#include "gpio.h"
#include "input.h"
#include "berechnung.h"
#include "output.h"
#include "timer.h"
#define MAX 90000000
unsigned int aft = 0;
unsigned int bef = 0;
unsigned int time = 0;
/**
 * @brief  Diese Funktion liest die Eingabe ein
 */
int input_einlesen() {
  aft = TIM2->CNT;
  time += aft - bef;
  bef = aft;
  int s0Pressed = readGPIOPin(BUTTON_PORT, S0);  // Wurde S0 Gedrückt ?
  int s1Pressed = readGPIOPin(BUTTON_PORT, S1);  // Wurde S1 Gedrückt ?

  bool a = !s0Pressed && !s1Pressed;
  if (a) {
    return PHASE_A;
  }
  bool b = s0Pressed && !s1Pressed;
  if (b) {
    return PHASE_B;
  }
  bool c = s0Pressed && s1Pressed;  // drive actors/outputs
  if (c) {
    return PHASE_C;
  }
  bool d = !s0Pressed && s1Pressed;
  if (d) {
    return PHASE_D;
  }

  return 0;
}

int inputzeit() {
  if (time >= MAX) {
    time = 0;
    return 1;
  }
  return 0;
}

/**
 * @brief  Diese Funktion gibt zurück ob S7 gedrückt wurde
 */
bool resetpressed() {
  int s7Pressed = readGPIOPin(BUTTON_PORT, S7);  // Wurde S7 Gedrückt ?
  bool reset = s7Pressed;
  if (reset) {
    return true;
  }
  return false;
}

/**
 * @brief  Diese Funktion gibt die Zeit zurück
 */
unsigned int get_Time() { return time; }