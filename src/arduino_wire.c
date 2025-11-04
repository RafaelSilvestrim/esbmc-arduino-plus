/**
 * arduino_wire.c
 * ESBMC-Arduino Plus – Modelos Operacionais para I2C (Wire)
 */

#include "arduino_wire.h"
#include <esbmc.h>

static int wire_initialized = 0;
static int wire_count = 0;

void Wire_begin(void) {
    // inicializa I2C
    wire_initialized = 1;
}

int Wire_requestFrom(int address, int count) {
    __ESBMC_assert(wire_initialized,
        "Wire_requestFrom: precisa chamar Wire_begin antes");
    __ESBMC_assert(count > 0,
        "Wire_requestFrom: count deve ser > 0");
    wire_count = count;
    return count;
}

int Wire_read(void) {
    __ESBMC_assert(wire_initialized,
        "Wire_read: precisa chamar Wire_begin antes");
    __ESBMC_assert(wire_count > 0,
        "Wire_read: nenhuma requisição pendente");
    int v = __VERIFIER_nondet_uchar(); // Inicializa 'v' com um valor não determinístico (0-255) - TRECHO COM A CORREÇÃO
    // retorna valor simbólico de um byte
    __ESBMC_assume(v >= 0 && v <= 255); // TRECHO COM A CORREÇÃO
    return v;
}
