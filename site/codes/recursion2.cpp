// Recursive function
// Termination condition
void TestFunc(int _a)
{
	int i = 0;

	if (_a == 1)
		return;

	TestFunc(_a - 1);
}

// Fibonacci sequence
// 1 1 2 3 5 8 13 21 34 ...
int Fibonacci(int _N)
{
	if (_N == 1 || _N == 2)
	{
		return 1;
	}

	int n1 = 1;
	int n2 = 1;
	int Value = 0;

	for (int i = 0; i < _N - 2; ++i)
	{
		Value = n1 + n2;
		n1 = n2;
		n2 = Value;
	}
}

int Fibonacci_Re(int _N)
{
	if (_N == 1 || _N == 2)
		return 1;

	return Fibonacci_Re(_N - 1) + Fibonacci_Re(_N - 2);
}

// Tail recursion
// Compiler optimization converts recursion into a loop when possible
int main()
{
	TestFunc(3);

	int Data = Fibonacci_Re(3);

	return 0;
}