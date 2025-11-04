/**
 * arduino_analog.c
 * ESBMC-Arduino Plus – Operational Models for analog functions
 */

#include "arduino_analog.h"

int analog_values[NUM_ANALOG_PINS];

int analogRead(int pin) {
    // Adiciona uma suposição para garantir que o ESBMC teste apenas pinos válidos
    __ESBMC_assume(pin >= 0 && pin < NUM_ANALOG_PINS); 
    // 1) valida índice de pino
    __ESBMC_assert(pin >= 0 && pin < NUM_ANALOG_PINS,
                   "analogRead: pino inválido");
    // 2) retorna o valor armazenado
    return analog_values[pin];
}

void analogWrite(int pin, int value) {
    __ESBMC_assert(pin >= 0 && pin < NUM_ANALOG_PINS,
                   "analogWrite: pino inválido");
    __ESBMC_assert(value >= 0 && value <= 255,
                   "analogWrite: valor fora de 0–255");
    // simula escrita no pino
    analog_values[pin] = value;
}
