#include "arduino_eeprom.h"

int main() {
    // Caso 1: escrita e leitura válidas
    EEPROM_write(10, 123);
    unsigned char v = EEPROM_read(10);
    __ESBMC_assert(v == 123,
                   "EEPROM_read: valor diferente do escrito");

    // Caso 2: acesso inválido
    EEPROM_read(EEPROM_SIZE);  // ESBMC deve apontar "EEPROM_read: endereço inválido"

    return 0;
}
