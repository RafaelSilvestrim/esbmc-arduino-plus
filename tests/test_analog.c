#include "arduino_analog.h"

int main() {
    // Caso 1: leitura válida
    analog_values[2] = 512;
    int v = analogRead(2);
    __ESBMC_assert(v == 512, "analogRead: valor incorreto");

    // Caso 2: acesso inválido
    analogRead(10);  // ESBMC deve apontar "analogRead: pino inválido"

    return 0;
}
