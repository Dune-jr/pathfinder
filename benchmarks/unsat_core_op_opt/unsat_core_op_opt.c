volatile int x;
volatile int y;

int main()
{
	x = 0;
	if(y)
		x = 1;
	if(x)
	{
		y = 1330;
	}
	else
	{
		y = 42;
	}
	return 0;
}
