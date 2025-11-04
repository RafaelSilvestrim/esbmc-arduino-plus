/**
 * arduino_ethernet.h
 * ESBMC-Arduino Plus – Modelos Operacionais para Ethernet
 */

#ifndef ARDUINO_ETHERNET_H
#define ARDUINO_ETHERNET_H

#define ETHERNET_MAX_HOSTLEN 64
#define ETHERNET_MAX_BUFFER 128

// Estado interno
extern int ethernet_initialized;
extern int client_connected;
extern int ethernet_available;

/**
 * Inicializa o hardware Ethernet.
 * Pré-condições: — (sempre pode ser chamado).
 */
void Ethernet_begin(void);

/**
 * Tenta conectar ao servidor 'host' na porta 'port'.
 * Pré-condições:
 *   - ethernet_initialized == 1
 *   - host != NULL e comprimento ≤ ETHERNET_MAX_HOSTLEN
 *   - 1 ≤ port ≤ 65535
 * Retorna 1 se conectar, 0 caso contrário.
 */
int EthernetClient_connect(const char* host, int port);

/**
 * Retorna quantos bytes estão disponíveis para leitura.
 * Pré-condição:
 *   - client_connected == 1
 * Sempre ≥ 0 e ≤ ETHERNET_MAX_BUFFER.
 */
int EthernetClient_available(void);

/**
 * Lê um byte do buffer de recepção.
 * Pré-condição:
 *   - cliente conectado
 *   - available() > 0
 * Retorna valor simbólico em [0…255].
 */
int EthernetClient_read(void);

/**
 * Encerra a conexão.
 * Pré-condição: client_connected == 1
 */
void EthernetClient_stop(void);

#endif // ARDUINO_ETHERNET_H
