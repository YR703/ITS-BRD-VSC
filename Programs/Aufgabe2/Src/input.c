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

int input_einlesen() {
  aft = TIM2->CNT;
  time += aft - bef;
  bef = aft;

  int s0Pressed = readGPIOPin(BUTTON_PORT, S0);
  int s1Pressed = readGPIOPin(BUTTON_PORT, S1);

  if (!s0Pressed && !s1Pressed) return PHASE_A;
  if (s0Pressed && !s1Pressed) return PHASE_B;
  if (s0Pressed && s1Pressed)  return PHASE_C;
  if (!s0Pressed && s1Pressed) return PHASE_D;

  return 0;
}

int inputzeit() {
  if (time >= MAX) {
    time = 0;
    return 1;
  }
  return 0;
}

bool resetpressed() {
  int s7Pressed = readGPIOPin(BUTTON_PORT, S7);
  return s7Pressed ? true : false;
}

unsigned int get_Time() {
  return time;
}
