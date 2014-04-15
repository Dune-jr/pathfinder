#include <stdio.h>

int main(int x)
{
	// __asm__("mov %ebx, %eax;");
	int a, b;
	//asm("mul %[result], %[value], [value2]" : [result] "=r" (x) : [value] "r" (b) );
	x *= 0b1010101010101010;
	x%=11;
	x++;
	x*=6;
	x-=7;

	return x;
}
