#include <math.h>

volatile k;

int f1()
{
//	time(NULL);
	k = sqrt(0);
}
int f2()
{
//	time(NULL);
	k = sqrt(1);
}
int f3()
{
//	time(NULL);
	k = sqrt(2);
}
int f4()
{
//	time(NULL);
	k = sqrt(3);
}

int main(int argc, char** argv)
{
	int x = argc, y, z;
	if(x <= 10)
	{
		f1();
	}
	else {
		f2();
	}
	if(x <= 0)
	{	
		f3();
	}
	else {
		f4();
	}
	//printf("%d %d", y, z);
	//return x;
	return 0;
}
