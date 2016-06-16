volatile int x, y;

int main()
{
	if(x > 5) // (a)
		y = 0; // (b)
	x = x + y;

	if(x > 5) // (c)
		return 1; // (d)
	else
		return 0; // (e)
	// (b), (e) infeasible path
}
