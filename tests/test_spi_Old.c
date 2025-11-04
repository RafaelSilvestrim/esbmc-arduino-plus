#include "arduino_spi.h"

int main() {
    // Caso 1: uso correto
    SPI_begin();
    int resp = SPI_transfer(100);
    __ESBMC_assert(resp >= 0 && resp <= 255,
                   "SPI_transfer: resposta fora de intervalo");

    // Caso 2: sem inicializar SPI → violação
    // comente o bloco acima para testar apenas o erro:
    // SPI_transfer(50);  // ESBMC deve apontar "SPI_transfer: precisa chamar SPI_begin antes"

    // Caso 3: valor inválido → violação
    // comente os demais e teste:
    // SPI_begin();
    // SPI_transfer(300); // ESBMC deve apontar "SPI_transfer: data fora de 0–255"

    return 0;
}
