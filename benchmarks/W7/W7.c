#include <stdio.h>

int f1()
{
	printf("abcbcb");
}
int f2()
{
	printf("bccdd");
}
int f3()
{
	printf("efefe");
}
int f4()
{
	printf("ghghg");
}

int main(int argc, char** argv)
{
	int x = argc, y, z;
	if(x >= 10)
	{
		f1();
	}
	else {
		f2();
	}
	if(x >= 0)
	{	
		f3();
	}
	else {
		f4();
	}
	printf("%d %d", y, z);
	//return x;
	return 0;
}
