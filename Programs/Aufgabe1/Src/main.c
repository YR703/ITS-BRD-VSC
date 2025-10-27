#include "display.h"
#include "scanner.h"
#include "stack.h"
#include "stm32f4xx_hal.h"
#include "init.h"
#include "token.h"
//#include <limits.h>

int main(void) {
    initITSboard();    // Initialisierung des ITS Boards
    initDisplay();     // Display + Touch initialisieren
    clearStack();      // Stack initialisieren

    T_token token;
    int a, b, err;
    long long result;

    while (1) {
        token = nextToken();

        switch (token.tok) {
            case NUMBER:
                push(token.val);
                break;

            case PLUS:
                if ((pop(&a) == STACK_OK) && (pop(&b) == STACK_OK)) {
                    result = (long long)b + a;
                    if (checkOverflow(result) == STACK_OK)
                        push((int)result);
                }
                break;

            case MINUS:
                if ((pop(&a) == STACK_OK) && (pop(&b) == STACK_OK)) {
                    result = (long long)b - a;
                    if (checkOverflow(result) == STACK_OK)
                        push((int)result);
                }
                break;

            case MULT:
                if ((pop(&a) == STACK_OK) && (pop(&b) == STACK_OK)) {
                    result = (long long)b * a;
                    if (checkOverflow(result) == STACK_OK)
                        push((int)result);
                }
                break;

            case DIV:
                if ((pop(&a) == STACK_OK) && (pop(&b) == STACK_OK)) {
                    if (a == 0) {
                        setErrMode();
                        printStdout("Fehler: Division durch 0!\n");
                    } else {
                        result = b / a;
                        push((int)result);
                    }
                }
                break;

            case PRT:
                printTop();
                break;

            case PRT_ALL:
                printAll();
                break;

            case CLEAR:
                clearStack();
                break;

            case DOUBLE:
                duplicateTop();
                break;

            case SWAP:
                swapTop();
                break;

            case UNEXPECTED:
                setErrMode();
                printStdout("Unerwartete Eingabe!\n");
                break;

            case OVERFLOW:
                setErrMode();
                printStdout("Zahlenüberlauf bei Eingabe!\n");
                break;
        }
        HAL_Delay(100);
    }
}
