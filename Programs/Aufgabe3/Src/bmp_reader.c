#include "bmp_reader.h"
#include "headers.h"
#include "input.h"
#include "errorhandler.h"
#include "LCD_GUI.h"     // Für Debug-Ausgabe
#include "fontsFLASH.h"  // Für Fonts
#include <stdio.h>       // Für snprintf
 
// -----------------------------------------------------------------------
// DEBUG & OVERRIDE SETTINGS
// -----------------------------------------------------------------------
// Setze dies auf 1, wenn du RLE erzwingen willst, obwohl der Header '0' sagt
#define FORCE_RLE_DECODE 0
 
// -----------------------------------------------------------------------
// STATISCHE VARIABLEN
// -----------------------------------------------------------------------
static int  g_width = 0;
static int  g_compression = 0;
static int  g_bitCount = 0;
static bool g_rle_eof = false;
 
// -----------------------------------------------------------------------
// RESET
// -----------------------------------------------------------------------
void bmp_reset(void) {
    g_width = 0;
    g_compression = 0;
    g_bitCount = 0;
    g_rle_eof = false;
}
 
// -----------------------------------------------------------------------
// START & HEADER ANALYSE
// -----------------------------------------------------------------------
int bmp_start(BITMAPFILEHEADER *fh, BITMAPINFOHEADER *ih, RGBQUAD *pal)
{
    bmp_reset();
    char debugBuf[64]; // Puffer für LCD-Text
 
    // 1. Standard-Header lesen (nutzt intern COMread, das ist ok für den Block)
    if (readHeaders() != EOK) {
        lcdErrorMsg("Header Read Error");
        return NOK;
    }
 
    getFileHeader(fh);
    getInfoHeader(ih);
 
    g_width       = ih->biWidth;
    g_compression = ih->biCompression;
    g_bitCount    = ih->biBitCount;
 
    // --- DEBUG AUSGABE AUF LCD ---
    // Wir schreiben direkt in das Bild, damit wir sehen, was los ist.
    // Format: "W:480 H:320 C:1 B:8" (C=1 heißt RLE, C=0 heißt RAW)
    GUI_clear(BLACK); // Sicherstellen, dass wir sauber starten
    snprintf(debugBuf, sizeof(debugBuf), "W:%d H:%d C:%d B:%d",
             (int)ih->biWidth, (int)ih->biHeight, (int)ih->biCompression, (int)ih->biBitCount);
   
    Coordinate pos = {0, 0};
    GUI_disStr(pos, debugBuf, &Font16, WHITE, RED);
   
    // Kurze Pause oder User muss Button drücken (in main), um Text zu lesen
    // Hier laufen wir weiter.
 
 
    // --- PALETTE LESEN (MIT NEXTCHAR) ---
    // Wir nutzen hier KEIN COMread, um absolute Kontrolle über den Stream zu haben.
   
    int colors = ih->biClrUsed;
    if (colors == 0 && g_bitCount == 8) colors = 256;
 
    // Palette liegt direkt nach dem InfoHeader
    for (int i = 0; i < colors; i++) {
        // Reihenfolge im BMP: Blau, Grün, Rot, Reserved
        int b = nextChar();
        int g = nextChar();
        int r = nextChar();
        int res = nextChar();
 
        if (b == EOF || g == EOF || r == EOF) {
            lcdErrorMsg("EOF in Palette");
            return NOK;
        }
 
        pal[i].rgbBlue     = (uint8_t)b;
        pal[i].rgbGreen    = (uint8_t)g;
        pal[i].rgbRed      = (uint8_t)r;
        pal[i].rgbReserved = (uint8_t)res;
    }
 
    // --- GAP SKIPPING (bfOffBits) ---
    // Wir müssen exakt dorthin springen, wo die Pixeldaten anfangen.
    // Berechnete Position: FileHeader(14) + InfoHeader(40) + Palette(4 * colors)
    int currentBytesRead = 14 + 40 + (colors * 4);
    int targetOffset = fh->bfOffBits;
   
    int gap = targetOffset - currentBytesRead;
   
    if (gap < 0) {
        // Das sollte bei validen BMPs nicht passieren, außer Header ist kaputt
        lcdErrorMsg("Err: Negatives Gap");
        return NOK;
    }
 
    // Überspringen von Gap-Bytes (Meta-Daten, ICC Profile, Junk)
    for (int i = 0; i < gap; i++) {
        if (nextChar() == EOF) {
            lcdErrorMsg("EOF in Gap");
            return NOK;
        }
    }
 
    // JETZT steht der Stream-Pointer exakt auf dem ersten Pixel-Byte (oder RLE-Opcode).
    return EOK;
}
 
// -----------------------------------------------------------------------
// RLE8 DECODER (KORRIGIERT)
// -----------------------------------------------------------------------
static int read_row_rle(uint8_t *row, int width) {
    // Zeile vorab mit 0 init (optional, aber sauberer bei Transparenz/Jumps)
    // Man könnte auch mit Hintergrundfarbe füllen.
    // memset(row, 0, width);
 
    if (g_rle_eof) return 0;
 
    int x = 0;
   
    // WICHTIG: Endlosschleife! Wir brechen erst ab, wenn das RLE-Kommando
    // sagt "Zeile zu Ende", NICHT wenn x >= width ist.
    while (1) {
        int b1 = nextChar();
        int b2 = nextChar();
 
        if (b1 == EOF || b2 == EOF) return -1;
 
        if (b1 > 0) {
            // --- ENCODED MODE ---
            int count = b1;
            int colorIndex = b2;
           
            for (int i = 0; i < count; i++) {
                // Nur schreiben, wenn wir noch im Bildbereich sind
                if (x < width) {
                    row[x] = (uint8_t)colorIndex;
                }
                x++; // x zählen wir trotzdem hoch, damit wir wissen, wo wir sind
            }
        } else {
            // --- ESCAPE MODE (b1 == 0) ---
            if (b2 == 0) {
                // End of Line: DAS ist der einzige korrekte Ausstieg für eine Zeile
                return 0;
            }
            else if (b2 == 1) {
                // End of Bitmap
                g_rle_eof = true;
                return 0;
            }
            else if (b2 == 2) {
                // Delta (dx, dy)
                int dx = nextChar();
                int dy = nextChar();
               
                // Cursor verschieben (Pixel werden übersprungen -> behalten alten Wert)
                x += dx;
               
                // Hinweis: dy > 0 würde eigentlich bedeuten, dass wir Zeilen überspringen.
                // In dieser einfachen Implementierung ignorieren wir das vertikale Springen,
                // da unsere Main-Loop zeilenweise anfordert.
                // Ein korrektes Handling müsste hier 'dy' Zeilen Puffer überspringen.
            }
            else {
                // Absolute Mode
                int count = b2;
                for (int i = 0; i < count; i++) {
                    int val = nextChar();
                    if (x < width) {
                        row[x] = (uint8_t)val;
                    }
                    x++;
                }
                // Alignment Padding (muss auf Word-Grenze liegen)
                if (count % 2 != 0) {
                    nextChar();
                }
            }
        }
    }
}
 
// -----------------------------------------------------------------------
// RAW READER (Unkomprimiert)
// -----------------------------------------------------------------------
static int read_row_raw(uint8_t *row, int width) {
    // 1. Pixel lesen
    for (int i = 0; i < width; i++) {
        int c = nextChar();
        if (c == EOF) return -1;
        row[i] = (uint8_t)c;
    }
 
    // 2. Padding berechnen
    // Bei 8 Bit: 1 Pixel = 1 Byte.
    // Zeilenlänge muss durch 4 teilbar sein.
    int bytesPerRow = width;
    int padding = 0;
   
    // Einfache Modulo Logik
    if (bytesPerRow % 4 != 0) {
        padding = 4 - (bytesPerRow % 4);
    }
   
    // Padding Bytes lesen und verwerfen
    for (int i = 0; i < padding; i++) {
        if (nextChar() == EOF) return -1;
    }
    return 0;
}
 
// -----------------------------------------------------------------------
// MAIN READ FUNCTION
// -----------------------------------------------------------------------
int bmp_read_row(uint8_t *row, int width) {
    if (g_compression == BI_RLE8) {
        return read_row_rle(row, width);
    } else {
        return read_row_raw(row, width);
    }
}