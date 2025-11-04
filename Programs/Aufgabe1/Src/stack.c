#include "stack.h"
#include <limits.h>

static int stack[STACK_SIZE];
static int top = 0;

static void intToStr(int value, char *buffer);

/**
 * @brief Legt einen Wert auf den Stack.
 * @param wert Der Wert, der auf den Stack gelegt wird.
 * @return ERR_OK bei Erfolg, ERR_STACK_FULL wenn der Stack voll ist.
 */
int push(int wert) {
    if (top >= STACK_SIZE)
        return ERR_STACK_FULL;
    stack[top++] = wert;
    return ERR_OK;
}

/**
 * @brief Entfernt das oberste Element vom Stack.
 * @param tmp Zeiger auf eine Variable, in die der entfernte Wert gespeichert wird.
 * @return ERR_OK bei Erfolg, ERR_STACK_EMPTY wenn der Stack leer ist.
 */
int pop(int *tmp) {
    if (top == 0)
        return ERR_STACK_EMPTY;
    *tmp = stack[--top];
    return ERR_OK;
}

/**
 * @brief Löscht alle Elemente auf dem Stack.
 */
void clearStack(void) {
    for (int i = 0; i < top; i++)
        stack[i] = 0;
    top = 0;
}

/**
 * @brief Verdoppelt das oberste Element des Stacks.
 * @return ERR_OK bei Erfolg, ERR_STACK_EMPTY oder ERR_STACK_FULL bei Fehler.
 */
int duplicateTop(void) {
    if (top == 0)
        return ERR_STACK_EMPTY;
    if (top >= STACK_SIZE)
        return ERR_STACK_FULL;
    stack[top] = stack[top - 1];
    top++;
    return ERR_OK;
}

/**
 *  Vertauscht die zwei obersten Werte auf dem Stack.
 *  ERR_OK bei Erfolg, ERR_STACK_EMPTY wenn zu wenige Werte vorhanden sind.
 */
int swapTop(void) {
    if (top < 2)
        return ERR_STACK_EMPTY;
    int tmp = stack[top - 1];
    stack[top - 1] = stack[top - 2];
    stack[top - 2] = tmp;
    return ERR_OK;
}

/**
 *  Gibt das oberste Element des Stacks auf dem Display aus.
 * ERR_OK bei Erfolg, ERR_STACK_EMPTY wenn Stack leer ist.
 */
int printTop(void) {
    if (top == 0)
        return ERR_STACK_EMPTY;

    clearStdout();
    char buf[32];
    intToStr(stack[top - 1], buf);
    printStdout(buf);
    printStdout("\n");
    return ERR_OK;
}

/**
 *  Gibt alle Elemente des Stacks auf dem Display aus.
 *  ERR_OK bei Erfolg, ERR_STACK_EMPTY wenn Stack leer ist.
 */
int printAll(void) {
    if (top == 0)
        return ERR_STACK_EMPTY;

    clearStdout();
    printStdout("Stackinhalt: ");
    for (int i = top - 1; i >= 0; i--) {
        char buf[32];
        intToStr(stack[i], buf);
        printStdout(buf);
        printStdout(" ");
    }
    printStdout("\n");
    return ERR_OK;
}

/**
 * @brief Wandelt eine Ganzzahl in einen String um 
 *        Behandelt INT_MIN (-2147483648) als Sonderfall.
 */
static void intToStr(int value, char *buffer) {
    int i = 0;
    int isNeg = 0;

    // ✅ Sonderfall: INT_MIN (-2147483648) kann nicht negiert werden
    if (value == INT_MIN) {
        const char *minStr = "-2147483648";
        for (i = 0; minStr[i] != '\0'; i++) {
            buffer[i] = minStr[i];
        }
        buffer[i] = '\0';
        return;
    }

    if (value < 0) {
        isNeg = 1;
        value = -value;
    }

    if (value == 0) {
        buffer[i++] = '0';
    }

    while (value > 0) {
        buffer[i++] = (value % 10) + '0';
        value /= 10;
    }

    if (isNeg)
        buffer[i++] = '-';

    buffer[i] = '\0';

    // Dreht den String um (da die Ziffern rückwärts erzeugt wurden)
    for (int j = 0; j < i / 2; j++) {
        char tmp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = tmp;
    }
}
