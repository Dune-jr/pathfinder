#include <stdint.h>

/* Program with 4 "then" branches in a loop.
   Only 1 of these "then" can be taken during one iteration. */

unsigned int a[4];
int main(uint32_t seed, int limit) {
  int i;
    seed = 1664525 * seed + 1013904223;
    int tcount = 0;
    if (tcount < limit && seed % 7 == 0) { a[0]++; tcount++; }
    if (tcount < limit && seed % 5 == 0) { a[1]++; tcount++; }
    if (tcount < limit && seed % 3 == 0) { a[2]++; tcount++; }
    if (tcount < limit && seed % 2 == 0) { a[3]++; tcount++; }
  return a[seed % 4];
}

volatile uint32_t seed;
volatile int limit = 1;
void _start() {
  main(seed,limit);
}
