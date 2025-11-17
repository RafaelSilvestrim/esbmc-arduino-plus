/**
 * src/arduino_core.c
 * ESBMC-Arduino Plus – Implementation stubs for core digital I/O
 */

#include "arduino_core.h"
#include <string.h>

/* single definition of the hardware state */
unsigned char pin_modes[NUM_PINS];
int pin_states[NUM_PINS];

/* optional initializer to set safe defaults */
void init_pin_state(void) __attribute__((constructor));
void init_pin_state(void) {
    /* default: all OUTPUT and state 0 */
    for (int i = 0; i < NUM_PINS; ++i) {
        pin_modes[i] = OUTPUT;
        pin_states[i] = 0;
    }
}

void pinMode(int pin, int mode) {
    __ESBMC_assert(pin >= 0 && pin < NUM_PINS, "pinMode: pino inválido");
    __ESBMC_assert(mode == INPUT || mode == OUTPUT, "pinMode: modo inválido");
    pin_modes[pin] = (unsigned char)(mode == INPUT ? 0 : 1);
}

void digitalWrite(int pin, int value) {
    __ESBMC_assert(pin >= 0 && pin < NUM_PINS, "digitalWrite: pino inválido");
    __ESBMC_assert(value == HIGH || value == LOW, "digitalWrite: valor inválido");
    __ESBMC_assert(pin_modes[pin] == OUTPUT, "digitalWrite: pino não configurado como OUTPUT");
    pin_states[pin] = value;
}

int digitalRead(int pin) {
    __ESBMC_assert(pin >= 0 && pin < NUM_PINS, "digitalRead: pino inválido");
    __ESBMC_assert(pin_modes[pin] == INPUT, "digitalRead: pino não configurado como INPUT");
    return pin_states[pin];
}