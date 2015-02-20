int main(int argc)
{
	int x = 1; // 1
	if(argc)
	{
		if(argc == 1)
		{
			x++; // 2
		}
		// 3
		do
		{
			x--; // 3
		}
		while(x);
	}
	else x = 666;
	// 4
	return 0;
}
