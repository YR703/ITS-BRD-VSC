
#ifndef STACK_H
#define STACK_H


#include <stdbool.h>


#define STACK_SIZE 25


#define STACK_OK                0
#define STACK_IST_LEER         -1
#define STACK_IST_VOLL         -2
#define DIVISION_DURCH_NULL    -3
#define BEREICHSUEBERSCHREITUNG -4


int push(int wert);


int pop(int *tmp);


int printTop(void);


int printAll(void);


void clearStack(void);


int duplicateTop(void);


int swapTop(void);


int checkOverflow(long long result);

#endif
// EOF
