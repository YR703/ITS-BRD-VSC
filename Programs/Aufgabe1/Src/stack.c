#include "stack.h"
#include "fehler.h"
#include <limits.h>
#include "display.h"


//hilfsfunktion deklarieren 
static void intToStr(int value, char *buffer);

//Stack daten
static int stack[STACK_SIZE];
static int top = 0;

//push einen  Wert auf den Stack
 
int push(int wert) {
    if (top >= STACK_SIZE) return ERR_STACK_FULL;
    stack[top++] = wert;
    return ERR_OK;
}

// Holt das oberste Element vom Stack und entfernt es
 
int pop(int *tmp) {
    if (top == 0) return ERR_STACK_EMPTY;
    *tmp = stack[--top];
    return ERR_OK;
}

//löscht alle Werte
 
void clearStack(void) {
    for (int i = 0; i < top; i++) {
        stack[i] = 0;
    }
    top = 0;
}

// Verdoppelt das oberste Zahl

int duplicateTop(void) {
    if (top == 0) return ERR_STACK_EMPTY;
    if (top >= STACK_SIZE) return ERR_STACK_FULL;
    int value = stack[top - 1];
    stack[top++] = value;
    return ERR_OK;
}

//Vertauscht die zwei obersten zaheln
 
int swapTop(void) {
    if (top < 2) return ERR_STACK_EMPTY;
    int tmp = stack[top - 1];
    stack[top - 1] = stack[top - 2];
    stack[top - 2] = tmp;
    return ERR_OK;
}

//Gibt das oberste Element des Stacks aus

int printTop(void) {
    if (top == 0) return ERR_STACK_EMPTY;

    char buf[32];
    intToStr(stack[top - 1], buf);
    printStdout(buf);
    printStdout(" ");
    return ERR_OK;
}

//gibt alle Elemente des Stacks aus
int printAll(void) {
    if (top == 0) return ERR_STACK_EMPTY;

    printStdout("Stackinhalt: ");
    for (int i = top - 1; i >= 0; i--) {
        char buf[32];
        intToStr(stack[i], buf);
        printStdout(buf);
        printStdout(" ");
    }
    return ERR_OK;
}

//hilfsfunktion:zahl in einen string konvertieren
static void intToStr(int value, char *buffer) {
    int i = 0;
    int isNegative = 0;

    if (value < 0) {
        isNegative = 1;
        value = -value;
    }

    if (value == 0) {
        buffer[i++] = '0';
    }

    while (value > 0) {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }

    if (isNegative) buffer[i++] = '-';
    buffer[i] = '\0';

    //String umdrehen
    for (int j = 0; j < i / 2; j++) {
        char temp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = temp;
    }
}
