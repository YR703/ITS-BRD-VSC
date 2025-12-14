#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>

#include "onewire.h"

/* =========================================================
 * PIN DEFINITIONS – ADAPT HERE IF NEEDED
 * ========================================================= */
#define ONEWIRE_DDR   DDRD
#define ONEWIRE_PORT  PORTD
#define ONEWIRE_PINR  PIND
#define ONEWIRE_BIT   PD0   // 1-Wire data line

/* =========================================================
 * INTERNAL DELAY (Aufgabe 4 independent)
 * ========================================================= */
static inline void delay_us(uint16_t us)
{
    while (us--) {
        _delay_us(1);
    }
}

/* =========================================================
 * LOW-LEVEL BUS CONTROL
 * ========================================================= */

/* Drive bus LOW (open-drain behavior) */
void onewire_line_low(void)
{
    ONEWIRE_DDR  |=  (1 << ONEWIRE_BIT);   // output
    ONEWIRE_PORT &= ~(1 << ONEWIRE_BIT);   // low
}

/* Release bus (Hi-Z, pull-up takes over) */
void onewire_line_release(void)
{
    ONEWIRE_DDR  &= ~(1 << ONEWIRE_BIT);   // input
    // no pull-up here (external pull-up on bus)
}

/* Read current bus level */
uint8_t onewire_read_line(void)
{
    return (ONEWIRE_PINR & (1 << ONEWIRE_BIT)) ? 1 : 0;
}

/* =========================================================
 * RESET & PRESENCE DETECT
 * ========================================================= */
bool onewire_reset(void)
{
    /* Master reset pulse */
    onewire_line_low();
    delay_us(480);

    onewire_line_release();
    delay_us(70);

    /* Presence pulse from slave = LOW */
    bool presence = !onewire_read_line();

    delay_us(410);
    return presence;
}

/* =========================================================
 * BIT OPERATIONS (LSB FIRST)
 * ========================================================= */
void onewire_write_bit(uint8_t bit)
{
    onewire_line_low();

    if (bit) {
        /* Write '1' */
        delay_us(6);
        onewire_line_release();
        delay_us(64);
    } else {
        /* Write '0' */
        delay_us(60);
        onewire_line_release();
        delay_us(10);
    }
}

uint8_t onewire_read_bit(void)
{
    uint8_t bit;

    onewire_line_low();
    delay_us(6);

    onewire_line_release();
    delay_us(9);

    bit = onewire_read_line();
    delay_us(55);

    return bit;
}

/* =========================================================
 * BYTE OPERATIONS
 * ========================================================= */
void onewire_write_byte(uint8_t byte)
{
    for (uint8_t i = 0; i < 8; i++) {
        onewire_write_bit(byte & 0x01);
        byte >>= 1;   // LSB first
    }
}

uint8_t onewire_read_byte(void)
{
    uint8_t byte = 0;

    for (uint8_t i = 0; i < 8; i++) {
        byte >>= 1;
        if (onewire_read_bit()) {
            byte |= 0x80;
        }
    }
    return byte;
}

/* =========================================================
 * READ 64-BIT ROM (Schritt 1 Ziel)
 * ========================================================= */
bool onewire_read_rom(uint8_t rom[8])
{
    if (!onewire_reset()) {
        return false;
    }

    onewire_write_byte(0x33); // READ ROM command

    for (uint8_t i = 0; i < 8; i++) {
        rom[i] = onewire_read_byte();
    }

    return true;
}
