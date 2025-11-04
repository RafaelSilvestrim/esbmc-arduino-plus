#include "arduino_ethernet.h"

int main() {
    // Caso 1: uso correto
    Ethernet_begin();
    int ok = EthernetClient_connect("example.com", 80);
    __ESBMC_assert(ok == 1,
        "EthernetClient_connect: falha na conexão");
    ethernet_available = 5;  
    int n = EthernetClient_available();
    __ESBMC_assert(n == 5,
        "EthernetClient_available: valor incorreto");
    int byte = EthernetClient_read();
    __ESBMC_assert(byte >= 0 && byte <= 255,
        "EthernetClient_read: valor fora de 0–255");
    EthernetClient_stop();

    // Caso 2: sem inicializar Ethernet
    // comente o bloco acima e deixe só isto:
    // EthernetClient_connect("test", 80);

    // Caso 3: porta inválida
    // Ethernet_begin();
    // EthernetClient_connect("test", 70000);

    return 0;
}
