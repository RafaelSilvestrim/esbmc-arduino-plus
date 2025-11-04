#include "arduino_servo.h"

int main() {
    // --- Caso 1: uso correto ---
    Servo_attach(3);
    Servo_write(3, 90);
    int ang = Servo_read(3);
    __ESBMC_assert(ang == 90,
        "Servo_read: ângulo incorreto");
    Servo_detach(3);

    // --- Caso 2: escrever antes de anexar (violação) ---
    // Servo_write(4, 45);

    // --- Caso 3: ângulo fora de intervalo (violação) ---
    // Servo_attach(5);
    // Servo_write(5, 200);

    // --- Caso 4: ler sem anexar (violação) ---
    // Servo_read(6);

    return 0;
}
