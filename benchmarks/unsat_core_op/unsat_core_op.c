// volatile int x;
// volatile int y;

int main()
{
	int x = 0, y, z;
	if(z)
		return 0;
	if(!y)
		x = 1;
	if(!x)
		y = 30;
	else
		y = 42;
	return 0;
}

// 1->3->5->6->8 : !z, y, !x
