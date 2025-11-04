/**
 * arduino_eeprom.h
 * ESBMC-Arduino Plus – Modelos Operacionais para EEPROM
 */

#ifndef ARDUINO_EEPROM_H
#define ARDUINO_EEPROM_H

#define EEPROM_SIZE 1024   // Tamanho em bytes da EEPROM simulada

extern unsigned char eeprom_memory[EEPROM_SIZE];

/**
 * Lê um byte da EEPROM em 'address'.
 * Pré-condição: 0 ≤ address < EEPROM_SIZE.
 */
unsigned char EEPROM_read(int address);

/**
 * Escreve um byte 'value' em 'address'.
 * Pré-condições: 
 *   0 ≤ address < EEPROM_SIZE, 
 *   0 ≤ value ≤ 255.
 */
void EEPROM_write(int address, unsigned char value);

#endif // ARDUINO_EEPROM_H
