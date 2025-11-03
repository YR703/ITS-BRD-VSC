#include "calc.h"
#include "display.h"
#include "stack.h"
#include "fehler.h"
#include <limits.h>

int calc(T_token token) {
    int a, b, result;

    switch (token.tok) {

        case NUMBER:  // wenn eine Zahl eingegeben wird, wird es auf den stack legen
            return push(token.val);

        case PLUS: // die addition zweier oberste werte auf stack(b+a)
            if (pop(&a) == STACK_OK && pop(&b) == STACK_OK) {
                if (b > INT_MAX - a) return ERR_OVERFLOW;
                result = b + a;
                return push(result);
            }
            return ERR_STACK_EMPTY;

        case MINUS: //die subtraktion zweier oberste werte auf stack(b-a)
            if (pop(&a) == STACK_OK && pop(&b) == STACK_OK) {
                if (b < INT_MIN + a || b > INT_MAX + a) return ERR_OVERFLOW;
                result = b - a;
                return push(result);
            }
            return ERR_STACK_EMPTY;

        case MULT: //die multiplaktion zweier oberste werte auf stack
            if (pop(&a) == STACK_OK && pop(&b) == STACK_OK) {
                if (a != 0 && (b > INT_MAX / a || b < INT_MIN / a)) return ERR_OVERFLOW;
                result = b * a;
                return push(result);
            }
            return ERR_STACK_EMPTY;

        case DIV: //die divison zweier oberste werte auf stack
            if (pop(&a) == STACK_OK && pop(&b) == STACK_OK) {
                if (a == 0) return ERR_DIV_ZERO;
                result = b / a;
                return push(result);
            }
            return ERR_STACK_EMPTY;

        case CLEAR: // Stack und Anzeige löschen
            clearStack();
            return ERR_OK;

        case PRT: //nur operste Element ausgeben (p)
            printTop();
            return ERR_OK;

        case PRT_ALL: //gesamt stackinhalt anzeigen (P)
            printAll();
            return ERR_OK;

        case DOUBLE: // oberste zwei werte duplikate (d)
            return duplicateTop();

        case SWAP: // zwei oberste werte vertauschen (r)
            return swapTop();

        case OVERFLOW: //beim überlauf bei zahleneingabe
            return ERR_OVERFLOW;

        case UNEXPECTED: //beim falsche Eingabe und unbekannte zeichen
            return ERR_UNKNOWN;

        default:
            return ERR_OK;
    }
}
