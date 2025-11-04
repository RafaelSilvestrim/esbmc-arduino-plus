#include "arduino_foobar.h"

int main() {
    int pin = 2;
    FooBar_init();
    int r_FooBar_do = FooBar_do(0);
    return 0;
}