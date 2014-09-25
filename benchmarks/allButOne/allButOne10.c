/* Program with 1 (expensive?) infeasible path.
   Contains 2^10 syntactic paths. */
int main(int argc, char *argv[]) {
  int x = 0;
  int c1, c2, c3, c4, c5, c6, c7, c8, c9, c10;
  if (c1) { x++; }
  if (c2) { x++; }
  if (c3) { x++; }
  if (c4) { x++; }
  if (c5) { x++; }
  if (c6) { x++; }
  if (c7) { x++; }
  if (c8) { x++; }
  if (c9) { x++; }
  if (!(c1 && c2 && c3 && c4 && c5 && c6 && c7 && c8 && c9 && c10)) { x++; }
  return x;
}
