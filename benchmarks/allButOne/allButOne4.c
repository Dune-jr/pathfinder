/* Program with 1 (expensive?) infeasible path.
   Contains 2^4 syntactic paths. */
int main(int argc, char *argv[]) {
  int x = 0;
  int c1, c2, c3, c4;
  if (c1) { x++; }
  if (c2) { x++; }
  if (c3) { x++; }
  if (!(c1 && c2 && c3 && c4)) { x++; }
  return x;
}
