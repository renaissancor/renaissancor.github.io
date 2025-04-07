// 7! == 1 * 2 * 3 * 4 * 5 * 6 * 7;
int Factorial(int _N)
{
	int Value = 1;

	for (int i = 0; i < _N - 1; ++i)
	{
		Value *= (i + 2);
	}

	return Value;
}

// Recursive function for factorial
int Factorial_Recursion(int _N)
{
	if (_N == 1)
		return 1;

	return Factorial_Recursion(_N - 1) * _N;
}

int main()
{
	int i = 0;

	i = Factorial(11);
	i = Factorial_Recursion(11);

	return 0;
}
