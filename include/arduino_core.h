/* include/arduino_core.h
 * Declarations for the core Arduino operational model
 */
#ifndef ARDUINO_CORE_H
#define ARDUINO_CORE_H

#include "esbmc.h"

/* Number of pins used in the model (override with -DNUM_PINS=N if needed) */
#ifndef NUM_PINS
#define NUM_PINS 14
#endif

/* Global hardware state (defined in src/arduino_core.c) */
extern unsigned char pin_modes[NUM_PINS]; /* 0 = INPUT, 1 = OUTPUT */
extern int pin_states[NUM_PINS];

/* Core API prototypes (implemented in src/arduino_core.c) */
void pinMode(int pin, int mode);
void digitalWrite(int pin, int value);
int digitalRead(int pin);

#endif /* ARDUINO_CORE_H */