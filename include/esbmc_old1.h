#ifndef ESBMC_H
#define ESBMC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Assinaturas compatíveis com esbmc_intrinsics.h */
extern void __ESBMC_assert(_Bool cond, const char *msg);
extern void __ESBMC_assume(_Bool cond);

#ifdef __cplusplus
}
#endif

#endif /* ESBMC_H */
