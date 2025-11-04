/**
 * arduino_spi.h
 * ESBMC-Arduino Plus – Modelos Operacionais para SPI
 */

#ifndef ARDUINO_SPI_H
#define ARDUINO_SPI_H

/**
 * Inicia o barramento SPI.
 */
void SPI_begin(void);

/**
 * Transfere um byte via SPI e retorna o valor recebido simultaneamente.
 * Pré-condições:
 *   – SPI_begin() foi chamado antes.
 *   – 0 ≤ data ≤ 255.
 * Retorna byte simbólico em [0..255].
 */
int SPI_transfer(int data);

#endif // ARDUINO_SPI_H
