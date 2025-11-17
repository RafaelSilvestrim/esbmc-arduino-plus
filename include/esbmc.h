#ifndef ESBMC_H
#define ESBMC_H

/* Tipos e constantes comuns */
#define HIGH 1
#define LOW  0
#define INPUT 0
#define OUTPUT 1
typedef unsigned char byte;

/* ESBMC intrinsics / stubs */
#ifdef __cplusplus
extern "C" {
#endif

/* intrínsecos que o ESBMC fornece / simulamos como protótipos */
extern void __ESBMC_assert(_Bool cond, const char *msg);
extern void __ESBMC_assume(_Bool cond);

/* nondet helpers (declarados extern para o verificador) */
extern int __VERIFIER_nondet_int(void);
extern unsigned int __VERIFIER_nondet_uint(void);
extern char __VERIFIER_nondet_char(void);
extern unsigned char __VERIFIER_nondet_uchar(void);
extern long __VERIFIER_nondet_long(void);
extern unsigned long __VERIFIER_nondet_ulong(void);
extern float __VERIFIER_nondet_float(void);
extern double __VERIFIER_nondet_double(void);

#ifdef __cplusplus
}
#endif

#endif /* ESBMC_H */