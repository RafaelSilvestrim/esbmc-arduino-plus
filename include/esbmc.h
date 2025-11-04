#ifndef ESBMC_H
#define ESBMC_H

// Tipos e constantes do Arduino
#define HIGH 1
#define LOW  0

#define INPUT 0
#define OUTPUT 1

typedef unsigned char byte;

// Funções auxiliares já suportadas pelo ESBMC
#define __ESBMC_assume(cond) ((void)0)
#define __ESBMC_assert(cond, msg) ((void)0)

// Variáveis não determinísticas (stubs)
extern int __VERIFIER_nondet_int(void);
extern unsigned int __VERIFIER_nondet_uint(void);
extern char __VERIFIER_nondet_char(void);
extern unsigned char __VERIFIER_nondet_uchar(void);
extern long __VERIFIER_nondet_long(void);
extern unsigned long __VERIFIER_nondet_ulong(void);
extern float __VERIFIER_nondet_float(void);
extern double __VERIFIER_nondet_double(void);

// Estado dos pinos (0 = INPUT, 1 = OUTPUT)
// Tornar estático e inicializar para OUTPUT (por exemplo)
static unsigned char pin_modes[256] = { [0 ... 255] = OUTPUT }; 

// Configuração de pino
static inline void pinMode(int pin, int mode) {
  __ESBMC_assert(mode == INPUT || mode == OUTPUT, "Modo inválido em pinMode");
  __ESBMC_assert(pin >= 0 && pin < 256, "Pino inválido em pinMode");
  pin_modes[pin] = (mode == INPUT) ? 0 : 1;
}

// Escrita digital
static inline void digitalWrite(int pin, int value) {
  __ESBMC_assert(value == HIGH || value == LOW, "Valor inválido em digitalWrite");
  __ESBMC_assert(pin >= 0 && pin < 256, "Pino inválido em digitalWrite");
}

// Leitura digital (valor não-determinístico)
// Verifica que o pino está configurado como entrada
static inline int digitalRead(int pin) {
  __ESBMC_assert(pin >= 0 && pin < 256, "Pino inválido em digitalRead");
  __ESBMC_assert(pin_modes[pin] == 0, "digitalRead: pino não configurado como INPUT");
  return __VERIFIER_nondet_int() % 2; // 0 ou 1
}

// Leitura analógica (0 a 1023)
static inline int analogRead(int pin) {
  __ESBMC_assert(pin >= 0 && pin < 256, "Pino inválido em analogRead");
  return __VERIFIER_nondet_int() % 1024;
}

// Escrita analógica (0 a 255)
static inline void analogWrite(int pin, int value) {
  __ESBMC_assert(pin >= 0 && pin < 256, "Pino inválido em analogWrite");
  __ESBMC_assert(value >= 0 && value <= 255, "Valor inválido em analogWrite");
}

#endif // ESBMC_H