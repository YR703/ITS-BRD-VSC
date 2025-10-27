#include "stack.h"
#include "display.h"
#include "display.h"
#include <limits.h>
#include <stdio.h>

static int stack[STACK_SIZE];
static int top = 0;

int push(int wert) {
    if (top >= STACK_SIZE) {
        setErrMode();
        printStdout("Fehler: Stack ist voll.\n");
        return STACK_IST_VOLL;
    }
    stack[top++] = wert;
    return STACK_OK;
}

int pop(int *tmp) {
    if (top == 0) {
        setErrMode();
        printStdout("Fehler: Stack ist leer.\n");
        return STACK_IST_LEER;
    }
    *tmp = stack[--top];
    return STACK_OK;
}

void clearStack(void) {
    for (int i = 0; i < top; i++) {
        stack[i] = 0;
    }
    top = 0;
    setNormalMode();
    clearStdout();
}

int printTop(void) {
    if (top == 0) {
        setErrMode();
        printStdout("Fehler: Stack ist leer.\n");
        return STACK_IST_LEER;
    }
    char buf[32];
    sprintf(buf, "%d\n", stack[top - 1]);
    printStdout(buf);
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
        sprintf(buf, "%d\n", stack[i]);
        printStdout(buf);
    }
    return STACK_OK;
}

int duplicateTop(void) {
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

int swapTop(void) {
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

int checkOverflow(long long result) {
    if (result > INT_MAX || result < INT_MIN) {
        setErrMode();
        printStdout("Fehler: Bereichsüberschreitung!\n");
        return BEREICHSUEBERSCHREITUNG;
    }
    return STACK_OK;
}
