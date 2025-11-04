/**
 * src/arduino_core.c
 * ESBMC-Arduino Plus – Implementation stubs for core digital I/O
 * Rafael Guerra Silvestrim, 2025
 */

#include "arduino_core.h"

// Variáveis globais que simulam o hardware
int pin_modes[NUM_PINS];
int pin_states[NUM_PINS];

void pinMode(int pin, int mode) {
    // 1) valida pino
    __ESBMC_assert(pin >= 0 && pin < NUM_PINS,
                   "pinMode: pino inválido");
    // 2) valida modo
    __ESBMC_assert(mode == INPUT || mode == OUTPUT,
                   "pinMode: modo inválido");
    // 3) atualiza estado
    pin_modes[pin] = mode;
}

void digitalWrite(int pin, int value) {
    __ESBMC_assert(pin >= 0 && pin < NUM_PINS,
                   "digitalWrite: pino inválido");
    __ESBMC_assert(pin_modes[pin] == OUTPUT,
                   "digitalWrite: pino não configurado como OUTPUT");
    pin_states[pin] = value;
}

int digitalRead(int pin) {
    __ESBMC_assert(pin >= 0 && pin < NUM_PINS,
                   "digitalRead: pino inválido");
    __ESBMC_assert(pin_modes[pin] == INPUT,
                   "digitalRead: pino não configurado como INPUT");
    return pin_states[pin];
}
