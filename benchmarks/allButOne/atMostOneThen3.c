/* Program with 3+1 (cheap?) feasible path.
   Contains 2^3 syntactic paths. */
int main(int argc, char *argv[]) {
  int x = 0;
  int tt = 0;
  int c1, c2, c3;
  if (c1 && !tt) { x=1; tt=1; }
  if (c2 && !tt) { x=2; tt=1; }
  if (c3 && !tt) { x=3; tt=1; }
  return x;
}
