int main(int argc, char* argv[])
{
	int i, j, x = argc, y = 0;
	for(i = 0; i < 5; i++)
	{
		for(j = 0; j < 10; j++)
		{
			if(x == 0)
				y++;
			if(x != 0)
				y++;
		}
		if(y == 3)
			y = 4;
	}
	if(y == 5)
		y = 6;
	return 0;
}
