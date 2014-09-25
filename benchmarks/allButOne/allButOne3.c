/* Program with 1 (expensive?) infeasible path.
   Contains 2^3 syntactic paths. */
int main(int argc, char *argv[]) {
  int x = 0;
  int c1, c2, c3;
  if (c1) { x++; }
  if (c2) { x++; }
  if (!(c1 && c2 && c3)) { x++; }
  return x;
}
