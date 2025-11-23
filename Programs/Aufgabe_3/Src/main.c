/*
 * main.c
 * Lösung für Teilaufgabe a - Angepasst an deine LCD_GUI.h
 */

#include <stdio.h>
#include <stdbool.h>
#include "main.h"
#include "input.h"
#include "headers.h"
#include "BMP_types.h"
#include "errorhandler.h"
#include "LCD_GUI.h" 

// Globale Variable für die Farbpalette (max 256 Farben bei 8-Bit BMP)
static RGBQUAD palette[256];

// --- HILFSFUNKTIONEN ---

/**
 * Konvertiert eine 24-Bit Farbe (aus der BMP Palette) in 16-Bit (für das LCD).
 */
uint16_t convertColor(RGBQUAD color) {
    uint16_t r = (color.rgbRed >> 3) & 0x1F;   // 5 Bit Rot
    uint16_t g = (color.rgbGreen >> 2) & 0x3F; // 6 Bit Grün
    uint16_t b = (color.rgbBlue >> 3) & 0x1F;  // 5 Bit Blau

    return (r << 11) | (g << 5) | b;
}

/**
 * Zeichnet einen Pixel sicher auf das Display.
 * Passt x und y in die "Coordinate" Struktur an.
 */
void drawPixelSafe(int x, int y, uint16_t color) {
    // Display Größe Check (480x320)
    if (x >= 0 && x < 480 && y >= 0 && y < 320) {
        
        // WICHTIG: Hier erstellen wir die Struktur für die LCD Funktion
        Coordinate coord;
        coord.x = x;
        coord.y = y;

        // Aufruf mit genau 4 Parametern, wie in deiner LCD_GUI.h definiert:
        // 1. Coordinate, 2. Color, 3. PixelSize, 4. Style
        GUI_drawPoint(coord, color, DOT_PIXEL_1X1, DOT_FILL_AROUND);
    }
}

// --- HAUPTPROGRAMM ---

int main(void) {
    // 1. Initialisierung
    initInput();
    
    // Bildschirm löschen (mit Farbe SCHWARZ, wie in LCD_GUI.h definiert)
    GUI_clear(BLACK); 

    while (1) {
        // 2. Auf das nächste Bild warten
        openNextFile();

        // 3. Header einlesen
        if (readHeaders() != EOK) {
            continue; // Fehler im Header -> Nächstes Bild
        }

        BITMAPINFOHEADER info;
        getInfoHeader(&info);

        // Check: Nur RLE8 erlaubt
        if (info.biCompression != BI_RLE8) {
            LOOP_ON_ERR(true, "Nur RLE8 Kompression erlaubt!");
        }

        // 4. Farbpalette einlesen
        int colorsToRead = (info.biClrUsed == 0) ? 256 : info.biClrUsed;
        
        if (COMread((char*)palette, sizeof(RGBQUAD), colorsToRead) != colorsToRead) {
            LOOP_ON_ERR(true, "Fehler beim Lesen der Palette");
        }

        // Bildschirm löschen für das neue Bild
        GUI_clear(BLACK);

        // 5. RLE Dekodierung
        int x = 0;
        int y = info.biHeight - 1; // Start unten links
        bool endOfBitmap = false;

        while (!endOfBitmap) {
            int b1 = nextChar();
            int b2 = nextChar();

            if (b1 == EOF || b2 == EOF) break;

            if (b1 > 0) {
                // --- ENCODED MODE ---
                uint16_t color = convertColor(palette[b2]);
                for (int i = 0; i < b1; i++) {
                    drawPixelSafe(x, y, color);
                    x++;
                }
            } else {
                // --- ESCAPE MODE ---
                if (b2 == 0) { 
                    // End of Line
                    x = 0;
                    y--; 
                } 
                else if (b2 == 1) { 
                    // End of Bitmap
                    endOfBitmap = true;
                } 
                else if (b2 == 2) { 
                    // Delta
                    int dx = nextChar();
                    int dy = nextChar();
                    x += dx;
                    y -= dy;
                } 
                else { 
                    // Absolute Mode
                    for (int i = 0; i < b2; i++) {
                        int colorIndex = nextChar();
                        uint16_t color = convertColor(palette[colorIndex]);
                        drawPixelSafe(x, y, color);
                        x++;
                    }
                    // Padding bei ungerader Anzahl
                    if (b2 % 2 != 0) {
                        nextChar(); 
                    }
                }
            }
        }
    }
}