/**
 * arduino_time.h
 * ESBMC-Arduino Plus – Operational Models para funções de tempo
 */

#ifndef ARDUINO_TIME_H
#define ARDUINO_TIME_H

/**
 * Simula uma espera de ms milissegundos.
 * Pré-condição: ms ≥ 0.
 */
void delay(unsigned long ms);

/**
 * Retorna um valor simbólico de tempo (ms) desde o início.
 * Valor deve ser não-negativo.
 */
unsigned long millis(void);

#endif // ARDUINO_TIME_H
