/**
 * arduino_wire.h
 * ESBMC-Arduino Plus – Modelos Operacionais para I2C (Wire)
 */

#ifndef ARDUINO_WIRE_H
#define ARDUINO_WIRE_H

/**
 * Inicializa o barramento I2C.
 */
void Wire_begin(void);

/**
 * Requisita 'count' bytes do dispositivo no endereço 'address'.
 * Pré-condições:
 *   – Wire_begin() deve ter sido chamado antes.
 *   – count > 0.
 * Retorna o número de bytes efetivamente requisitados (count).
 */
int Wire_requestFrom(int address, int count);

/**
 * Lê um byte do buffer I2C.
 * Pré-condições:
 *   – Wire_begin() deve ter sido chamado antes.
 *   – deve haver ao menos 1 byte pendente (requestFrom > 0).
 * Retorna valor simbólico em [0…255].
 */
int Wire_read(void);

#endif // ARDUINO_WIRE_H
