#ifndef ONEWIRE_H
#define ONEWIRE_H

#include <stdint.h>
#include <stdbool.h>

//Low-level bus control 
void onewire_line_low(void);
void onewire_line_release(void);
uint8_t onewire_read_line(void);

//Core protocol
bool onewire_reset(void);

//Bit operations 
void onewire_write_bit(uint8_t bit);
uint8_t onewire_read_bit(void);

//Byte operations 
void onewire_write_byte(uint8_t byte);
uint8_t onewire_read_byte(void);

//ROM
bool onewire_read_rom(uint8_t rom[8]);

#endif // ONEWIRE_H
