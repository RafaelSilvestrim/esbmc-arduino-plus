/**
 * arduino_servo.h
 * ESBMC-Arduino Plus – Modelos Operacionais para Servo
 */

#ifndef ARDUINO_SERVO_H
#define ARDUINO_SERVO_H

#define MIN_PULSE_WIDTH    544    // μs
#define MAX_PULSE_WIDTH   2400    // μs
#define DEFAULT_ANGLE     90      // grau neutro

/**
 * Anexa o servo ao pino 'pin'.
 * Pré-condição: 0 ≤ pin < NUM_PINS.
 */
void Servo_attach(int pin);

/**
 * Escreve um ângulo [0–180] graus no servo.
 * Pré-condições:
 *    Servo_attach(pin) foi chamado;
 *    0 ≤ angle ≤ 180.
 */
void Servo_write(int pin, int angle);

/**
 * Lê o último ângulo escrito.
 * Pré-condição: Servo_attach(pin) foi chamado.
 * Retorna o ângulo armazenado.
 */
int Servo_read(int pin);

/**
 * Desanexa o servo.
 * Pré-condição: Servo_attach(pin) foi chamado.
 */
void Servo_detach(int pin);

#endif // ARDUINO_SERVO_H
