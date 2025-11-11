/**
 * arduino_core.h
 * ESBMC-Arduino Plus – Operational Models for core digital functions
 */

#ifndef ARDUINO_CORE_H
#define ARDUINO_CORE_H

#define INPUT   0
#define OUTPUT  1
#define NUM_PINS 14

/* estado global (definido em src/arduino_core.c) */
extern unsigned char pin_modes[NUM_PINS];
extern int pin_states[NUM_PINS];

/**
 * Configura o modo (INPUT/OUTPUT) de um pino.
 * Pré-condições: pin ∈ [0,NUM_PINS), mode ∈ {INPUT,OUTPUT}.
 */
void pinMode(int pin, int mode);

/**
 * Escreve HIGH/LOW em um pino configurado como OUTPUT.
 * Pré-condições: mesmo intervalo de pino; pinMode(pin)==OUTPUT.
 */
void digitalWrite(int pin, int value);

/**
 * Lê o estado de um pino configurado como INPUT.
 * Pré-condições: pinMode(pin)==INPUT.
 * Retorna o último valor escrito (pin_states[pin]).
 */
int digitalRead(int pin);

#endif // ARDUINO_CORE_H
