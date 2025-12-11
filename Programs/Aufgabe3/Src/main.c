#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>  // Wichtig für EOF
 
// Projekt-Header
#include "init.h"
#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "fontsFLASH.h"
 
#include "input.h"
#include "headers.h"
#include "bmp_reader.h"
#include "lcd_output.h"
#include "scaler.h"
#include "gpio.h"
#include "errorhandler.h"
 
// Hardware Konfiguration
#define S0_PORT GPIOF
#define S0_PIN  0
 
// Display & Speicher Konfiguration
#define LCD_WIDTH  480
#define LCD_HEIGHT 320
#define MAX_BMP_WIDTH 2400
#define RING_BUFFER_SIZE 6
 
// --- Statische Puffer (um Stack-Overflow zu vermeiden) ---
static uint8_t rowBuffer[RING_BUFFER_SIZE][MAX_BMP_WIDTH];
static uint8_t *scalerRows[RING_BUFFER_SIZE];
static uint16_t outputLine[LCD_WIDTH];
 
// Taster-Hilfsfunktion
int button_pressed() {
    return readGPIOPin(S0_PORT, S0_PIN) == 0;
}
 
int main(void)
{
    // Hardware Initialisierung
    initITSboard();
    GUI_init(DEFAULT_BRIGHTNESS);
    TP_Init(false);
 
    if (!checkVersionFlashFonts()) {
        LOOP_ON_ERR(true, "Font Mismatch");
    }
 
    initInput();
    GUI_clear(BLACK);
 
    while (1)
    {
        // 1. Auf Startsignal warten (Datei öffnen)
        while (!button_pressed()); // Optional: Warten auf Taster vor Start
        while (button_pressed());
       
        openNextFile();
        GUI_clear(BLACK);
 
        // 2. Variablen für Header anlegen
        BITMAPFILEHEADER fh;
        BITMAPINFOHEADER ih;
        // Palette dynamisch allozieren (256 * 4 Bytes = 1KB)
        RGBQUAD *pal = (RGBQUAD*)malloc(256 * sizeof(RGBQUAD));
 
        if (pal == NULL) {
            lcdErrorMsg("RAM voll (Palette)");
            continue;
        }
 
        // 3. Header einlesen
        // bmp_start setzt jetzt intern compression flags und liest die Palette
        if (bmp_start(&fh, &ih, pal) != EOK) {
            free(pal);
            while (!button_pressed());
            while (button_pressed());
            continue;
        }
 
        int srcW = ih.biWidth;
        int srcH = ih.biHeight;
 
        // Sicherheitscheck Breite
        if (srcW > MAX_BMP_WIDTH) {
            lcdErrorMsg("Bild zu breit!");
            free(pal);
            continue;
        }
 
        // ---------------------------------------------------------
        // SKALIERUNGS-BERECHNUNG (Fit-to-Screen / Letterboxing)
        // ---------------------------------------------------------
        float scale_x = (float)LCD_WIDTH / (float)srcW;
        float scale_y = (float)LCD_HEIGHT / (float)srcH;
 
        // Wähle den kleineren Faktor, damit das ganze Bild passt
        float scale = (scale_x < scale_y) ? scale_x : scale_y;
 
        // Kein Upscaling (optional)
        if (scale > 1.0f) scale = 1.0f;
 
        // Effektive Bildgröße auf dem Display
        int displayImageWidth  = (int)(srcW * scale);
        int displayImageHeight = (int)(srcH * scale);
 
        // Offsets für Zentrierung (Schwarze Balken)
        int offsetX = (LCD_WIDTH - displayImageWidth) / 2;
        int offsetY = (LCD_HEIGHT - displayImageHeight) / 2;
 
        // Box-Größe (Wie viele Quellzeilen pro Zielzeile?)
        int boxSize = (int)ceilf(1.0f / scale);
        if (boxSize > RING_BUFFER_SIZE) boxSize = RING_BUFFER_SIZE;
        if (boxSize < 1) boxSize = 1;
 
        // ---------------------------------------------------------
        // STREAMING LOOP
        // ---------------------------------------------------------
        int rowsReadTotal = 0;
        int bmpStatus = 0; // 0 = OK, -1 = Fehler/EOF
 
        // Iteration über die HÖHE des Zielbildes
        for (int i = 0; i < displayImageHeight; i++)
        {
            // A. Welche Zeilen brauchen wir aus dem Quellbild?
            float srcY_start_f = (float)i / scale;
           
            int startSrcRow = (int)floorf(srcY_start_f);
            int endSrcRow   = startSrcRow + boxSize;
 
            if (endSrcRow > srcH) endSrcRow = srcH;
 
            // B. Ringpuffer nachladen
            while (rowsReadTotal < endSrcRow && bmpStatus == 0)
            {
                uint8_t *bufPtr = rowBuffer[rowsReadTotal % RING_BUFFER_SIZE];
               
                // Zeile einlesen
                int res = bmp_read_row(bufPtr, srcW);
                if (res != 0) {
                    bmpStatus = -1; // Abbruch markieren
                }
                rowsReadTotal++;
            }
 
            // C. Pointer-Array für den Scaler vorbereiten
            int validRows = 0;
            for (int r = startSrcRow; r < endSrcRow; r++)
            {
                // Nur Zeilen nutzen, die wir erfolgreich gelesen haben
                if (r < rowsReadTotal) {
                    scalerRows[validRows] = rowBuffer[r % RING_BUFFER_SIZE];
                    validRows++;
                }
            }
 
            // D. Skalieren und Zeichnen
            if (validRows > 0)
            {
                // Aufruf der Skalierungsfunktion (definiert in scaler.c)
                scale_line_box_fit(outputLine, scalerRows, validRows,
                                   srcW, scale, offsetX, displayImageWidth, pal);
               
                // Y-Position berechnen (BMP ist Bottom-Up!)
                // Wir zeichnen von unten nach oben
                int lcdY = (offsetY + displayImageHeight - 1) - i;
               
                // Zeichnen (Ganze Breite 480px wird geschrieben)
                lcd_draw_row(0, lcdY, outputLine, LCD_WIDTH);
            }
        }
 
        // Restliche Zeilen auslesen (damit UART Puffer leer ist für nächstes Bild)
        while (rowsReadTotal < srcH && bmpStatus == 0) {
            bmp_read_row(rowBuffer[0], srcW);
            rowsReadTotal++;
        }
 
        free(pal);
 
        // Warten auf User
        while (!button_pressed());
        while (button_pressed());
    }
}