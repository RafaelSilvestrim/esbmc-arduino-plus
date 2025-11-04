#include "arduino_wire.h"

int main() {
    // Caso 1: uso correto
    Wire_begin();
    int n = Wire_requestFrom(0x50, 3);
    __ESBMC_assert(n == 3, "Wire_requestFrom: count incorreto");
    int v = Wire_read();
    __ESBMC_assert(v >= 0 && v <= 255,
                   "Wire_read: valor fora do intervalo");

    // Caso 2: uso incorreto
    // comente o bloco acima para demonstrar apenas o erro:
    // Wire_read();  // ESBMC deve apontar "Wire_read: precisa chamar Wire_begin antes"

    return 0;
}
