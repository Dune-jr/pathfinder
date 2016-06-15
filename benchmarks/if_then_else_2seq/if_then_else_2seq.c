volatile k;

void f1()
{
}
void f2()
{
}
void f3()
{
}
void f4()
{
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
