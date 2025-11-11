#include "arduino_core.h"

int main() {
  int v = digitalRead(0); // não chamamos pinMode -> deve violar a asserção do stub
  (void)v;
  return 0;
}
