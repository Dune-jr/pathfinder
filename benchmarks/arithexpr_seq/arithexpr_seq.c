#include <stdio.h>

int main(int argc, char** argv)
{
  volatile int x = argc;

	x*=6;
  printf("0");

	x/=5;
  printf("0");

	x%=7;
  printf("0");
	return x;
}
