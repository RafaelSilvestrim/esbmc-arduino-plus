#include "arduino_time.h"

int main() {
    // Caso 1: uso correto
    delay(1000);
    unsigned long t = millis();
    __ESBMC_assert(t >= 0, "millis: valor negativo");

    // Caso 2: uso incorreto
    delay((unsigned long)-1);  // ESBMC deve apontar "delay: tempo negativo"

    return 0;
}
