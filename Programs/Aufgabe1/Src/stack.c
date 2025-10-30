#include "stack.h"
#include "display.h"
#include "display.h"
#include <limits.h> //für INT_MAX, INT_MIN 
#include <stdbool.h>
#include <stdio.h>      //für bool-Typ

static int stack[STACK_SIZE];   //speicher werte auf dem Satck "25"
static int top = 0; //zeigauf die nächste freie position auf dem Stack

static void inToStr(int value, char *buffer) {
    int i = 0;
    bool isNegative = false;

    if (value < 0){
        isNegative = true;
        value = -value;
    }

    if(value == 0){
        buffer[i++] = '0';
    }

    while (value > 0){
        int digit = value %10;
        buffer[i++] = '0' + digit;
        value /= 10;
    }

    if(isNegative){
        buffer[i++] = '-';
    }

    buffer[i] = '\0';


for (int j = 0; j < i / 2; j++) {
char temp = buffer[j];
buffer[j] = buffer[i - 1 - j];
buffer[i - 1 - j] = temp;
  }
}




int push(int wert) {    
    if (top >= STACK_SIZE) {    //falls stack voll
        setErrMode(); //von display.c
        printStdout("Fehler: Stack ist voll.\n"); //stack ist voll fehlermeldung
        return STACK_IST_VOLL;
    }
    stack[top++] = wert;
    return STACK_OK;
}

int pop(int *tmp) {     //hier wird die oberste 2 werrte geholt werden
    if (top == 0) {
        setErrMode();
        printStdout("Fehler: Stack ist leer.\n");   //stack ist leer fehlermeldung
        return STACK_IST_LEER;
    }
    *tmp = stack[--top];
    return STACK_OK;
}

void clearStack(void) {     //stack und die Anzeige löschen 
    for (int i = 0; i < top; i++) {
        stack[i] = 0;
    }
    top = 0;
    setNormalMode(); //von display.c
    clearStdout();
}

int printTop(void) {        //oberste stackwert anzeigen aber nicht löschen 
    if (top == 0) {
        setErrMode();
        printStdout("Fehler: Stack ist leer.\n");
        return STACK_IST_LEER;
    }
    char buf[32];               
    inToStr(stack[top - 1], buf);
    printStdout(buf);
    printStdout("\n");
    return STACK_OK;
}

int printAll(void) {
    if (top == 0) {
        setErrMode();
        printStdout("Stack ist leer.\n");
        return STACK_IST_LEER;
    }
    setNormalMode();
    printStdout("Stackinhalt:\n");
    for (int i = top - 1; i >= 0; i--) {
        char buf[32];
        inToStr(stack[i], buf);
        printStdout(buf);
        printStdout(" ");
    }
    return STACK_OK;
}

int duplicateTop(void) {        //die obersten wert wird dupliziert 
    if (top == 0) {
        setErrMode();
        printStdout("Fehler: Stack ist leer.\n");
        return STACK_IST_LEER;
    }
    if (top >= STACK_SIZE) {
        setErrMode();
        printStdout("Fehler: Stack ist voll.\n");
        return STACK_IST_VOLL;
    }
    int value = stack[top - 1];
    stack[top++] = value;
    return STACK_OK;
}

int swapTop(void) {     //hier wird die oberste zwei werte vertauschen 
    if (top < 2) {
        setErrMode();
        printStdout("Fehler: Zu wenige Werte auf dem Stack.\n");
        return STACK_IST_LEER;
    }
    int tmp = stack[top - 1];
    stack[top - 1] = stack[top - 2];
    stack[top - 2] = tmp;
    return STACK_OK;
}

int checkOverflow(long long result) {       //überlaufprüfung für Rechenopertion 
    if (result > INT_MAX || result < INT_MIN) {
        setErrMode();
        printStdout("Fehler: Bereichsueberschreitung!\n");
        return BEREICHSUEBERSCHREITUNG;
    }
    return STACK_OK;
}
