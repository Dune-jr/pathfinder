#include <stdint.h>

/* Program with 2 "then" branches in a loop.
   Only 1 of these "then" can be taken during one iteration. */

unsigned int a[2];
int main(uint32_t seed, int limit) {
  int i;
  for(i = 1; i <= 101; i++) {
    seed = 1664525 * seed + 1013904223;
    int tcount = 0;
    if (tcount < limit && seed % 3 == 0) { a[0]++; tcount++; }
    if (tcount < limit && seed % 2 == 0) { a[1]++; tcount++; }
  }
  return a[seed % 2];
}

volatile uint32_t seed;
volatile int limit = 1;
void _start() {
  main(seed,limit);
}
