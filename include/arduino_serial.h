/**
 * arduino_serial.h
 * ESBMC-Arduino Plus – Modelos Operacionais para Serial
 */

#ifndef ARDUINO_SERIAL_H
#define ARDUINO_SERIAL_H

/**
 * Inicializa a comunicação serial em determinada baudrate.
 * Pré-condição: baudrate > 0.
 */
void Serial_begin(int baudrate);

/**
 * Envia uma string pela Serial.
 * Pré-condição: Serial_begin() deve ter sido chamada antes.
 */
void Serial_print(const char* msg);

/**
 * Retorna a quantidade de bytes disponíveis no buffer.
 * Sempre ≥0.
 */
int Serial_available(void);

#endif // ARDUINO_SERIAL_H
