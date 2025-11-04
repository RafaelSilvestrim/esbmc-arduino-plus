#include "arduino_foobar.h"

int main() {
    int pin = 99; /* intentionally invalid to trigger precondition */
    FooBar_init();
    __ESBMC_assert(0, "force fail to show counterexample");
    int r_FooBar_do = FooBar_do(0);
    __ESBMC_assert(0, "force fail to show counterexample");
    return 0;
}