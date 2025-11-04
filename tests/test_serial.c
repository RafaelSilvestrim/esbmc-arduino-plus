#include "arduino_serial.h"

int main() {
    // Caso 1: inicialização e uso correto
    Serial_begin(9600);
    Serial_print("Hello");  
    int n = Serial_available();
    __ESBMC_assert(n >= 0, "Serial_available: valor negativo");

    // Caso 2: uso sem inicializar → deve violar!
    Serial_print("Fail");  // ESBMC deve apontar erro aqui

    return 0;
}
