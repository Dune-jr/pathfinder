volatile int x; // = 20;

int div3(int x)
	{ return x/3; }

int main(void)
{
	if(x > 0)
		if(div3(x) > x)
			return 1;
	return 0;
}
