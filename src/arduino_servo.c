/**
 * arduino_servo.c
 * ESBMC-Arduino Plus – Modelos Operacionais para Servo
 */

#include "arduino_servo.h"
#include <esbmc.h>

// Estado global do servo
static int servo_attached = 0;
static int servo_angle    = DEFAULT_ANGLE;

void Servo_attach(int pin) {
    __ESBMC_assert(pin >= 0 && pin < NUM_PINS,
        "Servo_attach: pin inválido");
    servo_attached = 1;
    servo_angle    = DEFAULT_ANGLE;
}

void Servo_write(int pin, int angle) {
    __ESBMC_assert(servo_attached,
        "Servo_write: Servo não anexado");
    __ESBMC_assert(angle >= 0 && angle <= 180,
        "Servo_write: ângulo inválido");
    servo_angle = angle;
}

int Servo_read(int pin) {
    __ESBMC_assert(servo_attached,
        "Servo_read: Servo não anexado");
    return servo_angle;
}

void Servo_detach(int pin) {
    __ESBMC_assert(servo_attached,
        "Servo_detach: Servo não anexado");
    servo_attached = 0;
}
