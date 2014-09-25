/* Program with 1 (expensive?) infeasible path.
   Contains 2^2 syntactic paths. */
int main(int argc, char *argv[]) {
  int x = 0;
  int c1, c2;
  if (c1) { x++; }
  if (!(c1 && c2)) { x++; }
  return x;
}
