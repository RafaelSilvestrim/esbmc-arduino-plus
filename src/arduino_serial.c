/**
 * arduino_serial.c
 * ESBMC-Arduino Plus – Modelos Operacionais para Serial
 */

#include "arduino_serial.h"
#include <esbmc.h>

static int serial_initialized = 0;
static int serial_buffer = 0;

void Serial_begin(int baudrate) {
    __ESBMC_assert(baudrate > 0,
                   "Serial_begin: baudrate deve ser > 0");
    serial_initialized = 1;
}

void Serial_print(const char* msg) {
    __ESBMC_assert(serial_initialized,
                   "Serial_print: precisa chamar Serial_begin antes");
    // Simula envio: nada a fazer no modelo
}

int Serial_available(void) {
    // Gera valor simbólico para o buffer, mas ≥0
    __ESBMC_assume(serial_buffer >= 0);
    return serial_buffer;
}
