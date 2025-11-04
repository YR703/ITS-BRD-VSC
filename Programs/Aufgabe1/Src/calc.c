#include "calc.h"
#include "display.h"
#include "stack.h"
#include "fehler.h"
#include "token.h"
#include <limits.h>

/**
 * @brief Führt Rechen- und Stackoperationen aus.
 */
int calc(T_token token) {
    int a, b, result;

    switch (token.tok) {
        case NUMBER:
            return push(token.val);

        case PLUS:
            if (pop(&a) == ERR_OK && pop(&b) == ERR_OK) {
                if (b > INT_MAX - a) return ERR_OVERFLOW;
                result = b + a;
                return push(result);
            }
            return ERR_STACK_EMPTY;

        case MINUS:
            if (pop(&a) == ERR_OK && pop(&b) == ERR_OK) {
                if ((a > 0 && b < INT_MIN + a) || (a < 0 && b > INT_MAX + a))
                    return ERR_OVERFLOW;
                result = b - a;
                return push(result);
            }
            return ERR_STACK_EMPTY;

        case MULT:
            if (pop(&a) == ERR_OK && pop(&b) == ERR_OK) {
                if (a != 0 && (b > INT_MAX / a || b < INT_MIN / a))
                    return ERR_OVERFLOW;

                    if (a != 0 && (b > INT_MAX / a || b < INT_MIN / a))
                        return ERR_OVERFLOW;
                    
                result = b * a;
                return push(result);
            }
            return ERR_STACK_EMPTY;

        case DIV:
            if (pop(&a) == ERR_OK && pop(&b) == ERR_OK) {
                if (a == 0)
                    return ERR_DIV_ZERO;
                result = b / a;
                return push(result);
            }
            return ERR_STACK_EMPTY;

        //  CLEAR ('C')
        case CLEAR:
            clearStack();
            clearStdout();
            return ERR_OK;

        //  PRINT TOP ('p') — propagate error correctly
        case PRT:
            return printTop();     // ✅ changed

        //  PRINT ALL ('P') — propagate error correctly
        case PRT_ALL:
            return printAll();     // ✅ changed

        // DUPLICATE TOP ('d')
        case DOUBLE:
            return duplicateTop();

        //  SWAP TOP TWO ('r')
        case SWAP:
            return swapTop();

        case OVERFLOW:
            return ERR_OVERFLOW;

        case UNEXPECTED:
            return ERR_UNKNOWN;

        default:
            return ERR_OK;
    }
}
