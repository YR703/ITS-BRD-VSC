#include "display.h"  //ausgabe und fehlermeldung
#include "scanner.h"  //liest eingabe vom touchpad
#include "stack.h"    //unsere Stackfunktion
#include "stm32f4xx_hal.h" //ITs-brd hardware
#include "init.h"           //initiallsierung des Boards
#include "token.h"          //Tocken Definaiton
//#include <limits.h>

int main(void) {
    initITSboard();    // Initialisierung des ITS Boards
    initDisplay();     // Display + Touch initialisieren
    clearStack();      // Stack initialisieren

    T_token token;
    int a, b;
    long long result;
//Endlose Schleife
    while (1) {
        token = nextToken(); //scanner liest hier die nächste Eingabe dauerhaft

        switch (token.tok) {
            case NUMBER:        //zahl auf Stack legen 
                push(token.val);
                break;

            case PLUS: //beim Addition 
                if ((pop(&a) == STACK_OK) && (pop(&b) == STACK_OK)) { //zwei oberste Zahl holen
                    result = (long long)b + a;
                    if (checkOverflow(result) == STACK_OK)  //overflow prüfen dann das Ergebnis wieder pushen
                        push((int)result);
                }
                break;

            case MINUS:  //beim Subtraktion 
                if ((pop(&a) == STACK_OK) && (pop(&b) == STACK_OK)) {
                    result = (long long)b - a;
                    if (checkOverflow(result) == STACK_OK)
                        push((int)result);
                }
                break;

            case MULT: //Multiplaktion
                if ((pop(&a) == STACK_OK) && (pop(&b) == STACK_OK)) {
                    result = (long long)b * a;
                    if (checkOverflow(result) == STACK_OK)
                        push((int)result);
                }
                break;

            case DIV:
                if ((pop(&a) == STACK_OK) && (pop(&b) == STACK_OK)) {
                    if (a == 0) {
                        setErrMode(); //Die diviosn durch 0 
                        printStdout("Fehler: Division durch 0!\n");
                    } else {
                        result = b / a;     //ganzzahlige divison
                        push((int)result);
                    }
                }
                break;

            case PRT:
                printTop();  //p-->obersten Stack zahl anzeigen
                break;

            case PRT_ALL:   //P-->gesamten Stack anzeigen
                printAll();
                break;

            case CLEAR:     //Stack löschen
                clearStack();
                break;

            case DOUBLE:
                duplicateTop();
                break;

            case SWAP:      //r-->die obere 2 werte vertauschen
                swapTop();
                break;

            case UNEXPECTED:        //die unbekannte eingabe 
                setErrMode();
                printStdout("Unerwartete Eingabe!\n");
                break;

            case OVERFLOW:      //eingabeüberlauf
                setErrMode();
                printStdout("Zahlenueberlauf bei Eingabe!\n");
                break;
        }
        HAL_Delay(100); //immer eine kleine Pause um mehrfachauslösungen zu vermeiden
    }
}
