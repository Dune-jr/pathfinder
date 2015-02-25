int main(int argc)
{
	int x = 1; // [SP-12]
	if(argc)
	{
		if(argc == 1)
			x++;
		do
		{
			x--;
		}
		while(x);
	}
	else
		x = 666;
	return 0;
}
