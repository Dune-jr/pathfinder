volatile int x; // = 20;

int div3(int x)
	{ return x/3; }

int main(void)
{
	if(div3(x))
		return div3(x);
	else
		return 0;
}
