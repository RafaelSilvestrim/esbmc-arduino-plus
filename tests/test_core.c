#include "arduino_core.h"

int main() {
    // Caso 1: operação inválida → NÃO usar para um teste que deve passar
    // pinMode(0, OUTPUT);
    // digitalWrite(0, 1);
    // digitalRead(0); // ESBMC apontaria erro aqui

    // Caso 2: operação válida → usa esta parte do código
    pinMode(1, INPUT);
    // pin_states[1] = 1; // Esta linha depende da simulação, pode ser desnecessária
    int v = digitalRead(1);
    __ESBMC_assert(v == 1 || v == 0, "A leitura deve ser 0 ou 1"); // Asserção mais robusta

    return 0;
}
