// Comments

// Functions
// Modularization
// Breaks down functionality into small components like parts.
// Combines small functions to easily build larger functionalities.
// Increases code reusability.

// Return type, function name, input arguments
// Default arguments: if no input is provided, the value written in the default argument is used.
// Default arguments can only be set from the last argument backwards; skipping in between is not allowed.
int Add(int _a, int _b, int _c = 0)
{
	int c = _a + _b;

	// Returns the value that matches the return type back to the caller
	return c;
}

void TestFunc()
{
	// In a function with return type void, using return; means the function will terminate immediately
	return;
}

int main()
{
	int Data = Add(10, 20);

	return 0;
}