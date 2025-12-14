#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "onewire.h"
#include "crc.h"

int main(void)
{
    uint8_t rom[8];

    //System init
    // init_ports();
    // init_timer();
    // init_display();

    printf("GS Aufgabe 4 – Schritt 1\n");

    //Presence detect
    if (!onewire_reset()) {
        printf("ERROR: No 1-Wire device detected\n");
        while (1);
    }

    printf("1-Wire device detected\n");

    //Read ROM
    if (!onewire_read_rom(rom)) {
        printf("ERROR: ROM read failed\n");
        while (1);
    }

    printf("ROM read:\n");
    for (int i = 0; i < 8; i++) {
        printf("%02X ", rom[i]);
    }
    printf("\n");

    // CRC check
    if (checkCRC(8, rom)) {
        printf("CRC OK\n");
    } else {
        printf("CRC ERROR\n");
    }

    //Idle loop
    while (1) {
        /* Schritt 1 ends here */
    }
}
