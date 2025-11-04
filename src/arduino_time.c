/**
 * arduino_time.c
 * ESBMC-Arduino Plus – Operational Models para funções de tempo
 */

#include "arduino_time.h"
#include <esbmc.h>  // para usar __ESBMC_assert e __ESBMC_assume

void delay(unsigned long ms) {
    // 1) valida entrada
    __ESBMC_assert(ms >= 0, "delay: tempo negativo");
    // 2) sem efeito simbólico (no-op)
}

unsigned long millis(void) {
    unsigned long t;
    // garante que o valor simbólico é não-negativo
    __ESBMC_assume(t >= 0);
    return t;
}
