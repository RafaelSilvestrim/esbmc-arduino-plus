#ifndef ESBMC_H
#define ESBMC_H

/* Tipos e constantes do Arduino (sem definir NUM_PINS aqui) */
#define HIGH 1
#define LOW  0
#define INPUT 0
#define OUTPUT 1
typedef unsigned char byte;

/* Default number of pins (can be overridden by module headers or -DNUM_PINS=...)
 * Use Arduino Uno default 14, or set higher if you want model more pins.
 */
#ifndef NUM_PINS
#define NUM_PINS 14
#endif

/* Intrínsecas / protótipos compatíveis com o ESBMC */
#ifdef __cplusplus
extern "C" {
#endif

extern void __ESBMC_assert(_Bool cond, const char *msg);
extern void __ESBMC_assume(_Bool cond);

/* Non-deterministic helpers (verifiers), mantidos como extern */
extern int __VERIFIER_nondet_int(void);
extern unsigned int __VERIFIER_nondet_uint(void);
extern char __VERIFIER_nondet_char(void);
extern unsigned char __VERIFIER_nondet_uchar(void);
extern long __VERIFIER_nondet_long(void);
extern unsigned long __VERIFIER_nondet_ulong(void);
extern float __VERIFIER_nondet_float(void);
extern double __VERIFIER_nondet_double(void);

/* Estado dos pinos - declaração apenas (definição no src) */
extern unsigned char pin_modes[NUM_PINS];
extern int pin_states[NUM_PINS];

#ifdef __cplusplus
}
#endif

#endif /* ESBMC_H */
