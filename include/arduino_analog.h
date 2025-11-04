/**
 * arduino_analog.h
 * ESBMC-Arduino Plus – Operational Models for analog functions
 */

#ifndef ARDUINO_ANALOG_H
#define ARDUINO_ANALOG_H

#define NUM_ANALOG_PINS 6   // A0–A5

// Estado interno dos pinos analógicos
extern int analog_values[NUM_ANALOG_PINS];

/**
 * Lê o valor de um pino analógico.
 * Pré-condições: 0 ≤ pin < NUM_ANALOG_PINS.
 * Retorna o valor em analog_values[pin].
 */
int analogRead(int pin);

/**
 * Escreve um valor PWM em um pino analógico (0–255).
 * Pré-condições: 0 ≤ pin < NUM_ANALOG_PINS e 0 ≤ value ≤ 255.
 */
void analogWrite(int pin, int value);

#endif // ARDUINO_ANALOG_H
