/**
 * arduino_eeprom.c
 * ESBMC-Arduino Plus – Modelos Operacionais para EEPROM
 */

#include "arduino_eeprom.h"
#include <esbmc.h>

// Array que modela a EEPROM
unsigned char eeprom_memory[EEPROM_SIZE];

unsigned char EEPROM_read(int address) {
    __ESBMC_assert(address >= 0 && address < EEPROM_SIZE,
                   "EEPROM_read: endereço inválido");
    return eeprom_memory[address];
}

void EEPROM_write(int address, unsigned char value) {
    __ESBMC_assert(address >= 0 && address < EEPROM_SIZE,
                   "EEPROM_write: endereço inválido");
    __ESBMC_assert(value <= 255,
                   "EEPROM_write: valor inválido (>255)");
    eeprom_memory[address] = value;
}
