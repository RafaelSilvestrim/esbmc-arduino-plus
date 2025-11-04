/**
 * arduino_spi.c
 * ESBMC-Arduino Plus – Modelos Operacionais para SPI
 */

#include "arduino_spi.h"
#include <esbmc.h>

static int spi_initialized = 0;

void SPI_begin(void) {
    // inicializa SPI
    spi_initialized = 1;
}

int SPI_transfer(int data) {
    __ESBMC_assert(spi_initialized,
        "SPI_transfer: precisa chamar SPI_begin antes");
    __ESBMC_assert(data >= 0 && data <= 255,
        "SPI_transfer: data fora de 0–255");
    int response;
    // recebe byte simbólico
    __ESBMC_assume(response >= 0 && response <= 255);
    return response;
}
