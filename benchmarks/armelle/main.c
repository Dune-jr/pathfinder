volatile int x;

int main()
{
	int PI=0;
	if(x < 0)
	{
		while(x < 0)
			x += 2*PI; // (a)
	}	
	//{ x in {0, +inf] }
	if(x > 2*PI)
	{
		while(x > 2*PI)
			x -= 2*PI; // (b)
    }
	//{ x in [0, 2PI] }
	return x;
}

