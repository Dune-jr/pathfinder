#include <stdint.h>

/* Program with 8 "then" branches in a loop.
   Only 1 of these "then" can be taken during one iteration. */

unsigned int a[8];
int main(uint32_t seed, int limit) {
  int i;
  for(i = 1; i <= 101; i++) {
    seed = 1664525 * seed + 1013904223;
    int tcount = 0;
    if (tcount < limit && seed % 19 == 0) { a[0]++; tcount++; }
    if (tcount < limit && seed % 17 == 0) { a[1]++; tcount++; }
    if (tcount < limit && seed % 13 == 0) { a[2]++; tcount++; }
    if (tcount < limit && seed % 11 == 0) { a[3]++; tcount++; }
    if (tcount < limit && seed % 7 == 0) { a[4]++; tcount++; }
    if (tcount < limit && seed % 5 == 0) { a[5]++; tcount++; }
    if (tcount < limit && seed % 3 == 0) { a[6]++; tcount++; }
    if (tcount < limit && seed % 2 == 0) { a[7]++; tcount++; }
  }
  return a[seed % 8];
}

volatile uint32_t seed;
volatile int limit = 1;
void _start() {
  main(seed,limit);
}
