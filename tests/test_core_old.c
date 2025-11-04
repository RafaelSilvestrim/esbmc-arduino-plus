#include "arduino_core.h"

int main() {
    // Caso 1: operação inválida → deve violar
    pinMode(0, OUTPUT);
    digitalWrite(0, 1);
    digitalRead(0); // ESBMC deve apontar erro aqui

    // Caso 2: operação válida → não chegaria aqui se o erro for detectado
    // pinMode(1, INPUT);
    // pin_states[1] = 1;
    // int v = digitalRead(1);
    // __ESBMC_assert(v == 1, "Leitura deve ser 1");

    return 0;
}
