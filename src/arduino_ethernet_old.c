/**
 * arduino_ethernet.c
 * ESBMC-Arduino Plus – Modelos Operacionais para Ethernet
 */

#include "arduino_ethernet.h"
#include <string.h>
#include <esbmc.h>

int ethernet_initialized = 0;
int client_connected      = 0;
int ethernet_available    = 0;

void Ethernet_begin(void) {
    // Inicializa o controlador
    ethernet_initialized = 1;
}

int EthernetClient_connect(const char* host, int port) {
    __ESBMC_assert(ethernet_initialized,
        "EthernetClient_connect: Ethernet não inicializado");
    __ESBMC_assert(host != NULL,
        "EthernetClient_connect: host NULL");
    __ESBMC_assert(strlen(host) <= ETHERNET_MAX_HOSTLEN,
        "EthernetClient_connect: host muito longo");
    __ESBMC_assert(port >= 1 && port <= 65535,
        "EthernetClient_connect: porta inválida");
    // Simula tentativa de conexão
    client_connected = 1;
    return 1;
}

int EthernetClient_available(void) {
    __ESBMC_assert(client_connected,
        "EthernetClient_available: cliente não conectado");
    // Gera valor simbólico de bytes disponíveis
    __ESBMC_assume(ethernet_available >= 0 &&
                   ethernet_available <= ETHERNET_MAX_BUFFER);
    return ethernet_available;
}

int EthernetClient_read(void) {
    __ESBMC_assert(client_connected,
        "EthernetClient_read: cliente não conectado");
    __ESBMC_assert(ethernet_available > 0,
        "EthernetClient_read: nenhum byte disponível");
    int b;
    __ESBMC_assume(b >= 0 && b <= 255);
    ethernet_available--;  // consome um byte
    return b;
}

void EthernetClient_stop(void) {
    __ESBMC_assert(client_connected,
        "EthernetClient_stop: cliente não conectado");
    client_connected = 0;
}
