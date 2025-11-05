#include "calc.h"
#include "display.h"
#include "stack.h"
#include "fehler.h"
#include "token.h"
#include <limits.h>


int calc(T_token token) {
    int a, b, result;

    switch (token.tok) {
        case NUMBER:
            return push(token.val);

        case PLUS: //Führt Addition durch: b + a
            if (pop(&a) == ERR_OK && pop(&b) == ERR_OK) {
                if (b > INT_MAX - a) return ERR_OVERFLOW;
                result = b + a;
                return push(result);
            }
            return ERR_STACK_EMPTY;

        case MINUS: //Subtraktion durch: b - a
            if (pop(&a) == ERR_OK && pop(&b) == ERR_OK) {
                if ((a > 0 && b < INT_MIN + a) || (a < 0 && b > INT_MAX + a))
                    return ERR_OVERFLOW;
                result = b - a;
                return push(result);
            }
            return ERR_STACK_EMPTY;

        case MULT: //Multiplikation durch: b * a
            if (pop(&a) == ERR_OK && pop(&b) == ERR_OK) {
                if (a != 0 && (b > INT_MAX / a || b < INT_MIN / a))
                    return ERR_OVERFLOW;

                    if (a != 0 && (b > INT_MAX / a || b < INT_MIN / a))
                        return ERR_OVERFLOW;
                    
                result = b * a;
                return push(result);
            }
            return ERR_STACK_EMPTY;

        case DIV: //Division durch: b / a
            if (pop(&a) == ERR_OK && pop(&b) == ERR_OK) {
                if (a == 0)
                    return ERR_DIV_ZERO;
                result = b / a;
                return push(result);
            }
            return ERR_STACK_EMPTY;

        //  CLEAR ('C') //Löscht den Stack und die Anzeige
        case CLEAR:
            clearStack();
            clearStdout();
            return ERR_OK;

         
        case PRT: //Gibt oberstes Element aus
            return printTop();     

          
        case PRT_ALL:
            return printAll();     // Gibt gesamten Stackinhalt aus

        
        case DOUBLE: //Verdoppelt oberstes Element
            return duplicateTop();

        //  
        case SWAP:  //Vertauscht die obersten zwei Werte
            return swapTop();

        case OVERFLOW: //Überlauf beim Eingeben einer Zahl
            return ERR_OVERFLOW;

        case UNEXPECTED: //Unerwartete oder ungültige Eingabe(e)
            return ERR_UNKNOWN;

        default:
            return ERR_OK;
    }
}
